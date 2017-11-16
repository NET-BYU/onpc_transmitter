#include <stdlib.h>
#include <string.h>

#include "common/cs_file.h"
#include "common/json_utils.h"
#include "common/mg_str.h"
#include "mgos_config_util.h"
#include "mgos_sys_config.h"
#include "mgos_vfs.h"

#include "mgos_app.h"

enum mgos_app_init_result mgos_app_init(void) {
    return MGOS_APP_INIT_SUCCESS;
}

const char *file_name = "data";
int max_length = 100;
int current_length = NULL;
FILE *fp = NULL;

typedef void (*peek_callback)(char *data, void *userdata);


bool start_queue() {
    // TODO: Currently assuming the file is not there
    fp = fopen(file_name, "w+b");

    if (fp == NULL) {
        LOG(LL_ERROR, ("Can not open file %s", file_name));
        return false;
    }

    int head_pos = 4;

    // TOOD: Assuming that we are starting at the beginning of the file

    if (fwrite(&head_pos, sizeof(head_pos), 1, fp) != 1) {
        LOG(LL_ERROR, ("Failed to write header to file: %s", file_name));
        return false;
    }

    fclose(fp);
    return true;
}


bool queue_put(char *data) {
    fp = fopen(file_name, "r+b");

    // Make sure no errors occurred
    if (fp == NULL) {
        LOG(LL_ERROR, ("Can not open file %s", file_name));
        return false;
    }

    // Go to beginning of the file
    fseek(fp, 0, SEEK_SET);

    int head_pos = NULL;
    if (fread(&head_pos, sizeof(head_pos), 1, fp) != 1) {
        LOG(LL_ERROR, ("Failed to read head position of file: %s", file_name));
        return false;
    }

    LOG(LL_INFO, ("Head Position: %d", head_pos));

    // Adjust the position of file
    fseek(fp, head_pos, SEEK_SET);

    LOG(LL_INFO, ("Writing data %s", data));
    struct mg_str str_data = mg_mk_str(data);

    // Write actual data
    if (fwrite(str_data.p, 1, str_data.len, fp) != str_data.len) {
        LOG(LL_ERROR, ("Failed to write data to file: %s", file_name));
        return false;
    }

    // Write length of data
    if (fwrite(&str_data.len, sizeof(str_data.len), 1, fp) != 1) {
        LOG(LL_ERROR, ("Failed to write meta data to file: %s", file_name));
        return false;
    }

    // Go to beginning of the file again
    fseek(fp, 0, SEEK_SET);

    // Update head position
    head_pos += (sizeof(str_data.len) + str_data.len);
    if (fwrite(&head_pos, sizeof(head_pos), 1, fp) != 1) {
        LOG(LL_ERROR, ("Failed to write position header to file: %s", file_name));
        return false;
    }

    fclose(fp);
    return true;
}


void queue_peek(peek_callback cb, void *cb_arg) {
    fp = fopen(file_name, "r+b");

    // Make sure no errors occurred
    if (fp == NULL) {
        LOG(LL_ERROR, ("Can not open file %s", file_name));
        cb(NULL, cb_arg);
        return;
    }

    // Go to beginning of the file
    fseek(fp, 0, SEEK_SET);

    int head_pos = NULL;
    if (fread(&head_pos, sizeof(head_pos), 1, fp) != 1) {
        LOG(LL_ERROR, ("Failed to read head position of file: %s", file_name));
        cb(NULL, cb_arg);
        return;
    }

    // TODO: Check if we are already at the beginning of the file

    // Read length data
    unsigned int length = NULL;
    fseek(fp, head_pos - sizeof(length), SEEK_SET);
    if (fread(&length, sizeof(length), 1, fp) != 1) {
        LOG(LL_ERROR, ("Failed to read length of data: %s", file_name));
        cb(NULL, cb_arg);
        return;
    }
    LOG(LL_INFO, ("Head position %d", head_pos));
    LOG(LL_INFO, ("Length of data %d", length));
    LOG(LL_INFO, ("Reading from %d to %d", head_pos - sizeof(length) - length, head_pos - sizeof(length)));

    // Read data
    char *data = NULL;
    data = (char *) malloc(length + 1);

    if (data == NULL) {
        LOG(LL_INFO, ("Out of memory %s", file_name));
        cb(NULL, cb_arg);
        return;
    }

    fseek(fp, head_pos - sizeof(length) - length, SEEK_SET);
    if (fread(data, 1, length, fp) != length) {
        LOG(LL_ERROR, ("Failed to read data: %s", file_name));

        if (data != NULL) {
            free(data);
        }

        cb(NULL, cb_arg);
        return;
    }

    data[length] = '\0'; // Null terminate
    LOG(LL_INFO, ("Data: %s", data));
    cb(data, cb_arg);

    free(data);
    fclose(fp);
}

bool queue_delete() {
    fp = fopen(file_name, "r+b");

    // Make sure no errors occurred
    if (fp == NULL) {
        LOG(LL_ERROR, ("Can not open file %s", file_name));
        return false;
    }

    // Go to beginning of the file
    fseek(fp, 0, SEEK_SET);

    int head_pos = NULL;
    if (fread(&head_pos, sizeof(head_pos), 1, fp) != 1) {
        LOG(LL_ERROR, ("Failed to read head position of file: %s", file_name));
        return false;
    }

    // TODO: Check if we are already at the beginning of the file

    // Read length data
    unsigned int length = NULL;
    fseek(fp, head_pos - sizeof(length), SEEK_SET);
    if (fread(&length, sizeof(length), 1, fp) != 1) {
        LOG(LL_ERROR, ("Failed to read length of data: %s", file_name));
        return false;
    }
    LOG(LL_INFO, ("Head position %d", head_pos));
    LOG(LL_INFO, ("Length of data %d", length));

    // Update head position
    head_pos -= sizeof(length) + length;
    if (fwrite(&head_pos, sizeof(head_pos), 1, fp) != 1) {
        LOG(LL_ERROR, ("Failed to write position header to file: %s", file_name));
        return false;
    }

    return true;
}

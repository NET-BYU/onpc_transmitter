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


bool start_queue() {
    // TODO: Currently assuming the file is not there
    fp = fopen(file_name, "a+b");

    if (fp == NULL) {
        LOG(LL_ERROR, ("Can not open file %s", file_name));
        return false;
    }

    int head_pos = 8;
    int tail_pos = 8;

    // TOOD: Assuming that we are starting at the beginning of the file

    if (fwrite(&head_pos, sizeof(head_pos), 1, fp) != 1) {
        LOG(LL_ERROR, ("Failed to write header to file: %s", file_name));
        return false;
    }

    if (fwrite(&tail_pos, sizeof(tail_pos), 1, fp) != 1) {
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

    // Write length of data
    if (fwrite(&str_data.len, sizeof(str_data.len), 1, fp) != 1) {
        LOG(LL_ERROR, ("Failed to write meta data to file: %s", file_name));
        return false;
    }
    // Write actual data
    if (fwrite(str_data.p, 1, str_data.len, fp) != str_data.len) {
        LOG(LL_ERROR, ("Failed to write data to file: %s", file_name));
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

bool queue_peek() {
    return true;
}

bool queue_delete() {
    return true;
}

bool queue_flush() {
    return true;
}

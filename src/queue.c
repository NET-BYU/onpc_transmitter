#include "queue.h"

const char *file_name = "data";

bool start_queue() {
    FILE *fp = NULL;
    bool result = false;

    LOG(LL_INFO, ("Opening queue file: %s", file_name));
    fp = fopen(file_name, "r+b");

    if (fp == NULL) {
        // The file does not exist
        LOG(LL_INFO, ("File does not exist. Trying to open file in write mode."));
        fp = fopen(file_name, "w+b");
        if (fp == NULL) {
            LOG(LL_ERROR, ("Can not open file %s", file_name));
            goto done;
        }

        int head_pos = 4;
        if (fwrite(&head_pos, sizeof(head_pos), 1, fp) != 1) {
            LOG(LL_ERROR, ("Failed to write header to file: %s", file_name));
            goto done;
        }
    }

    // No errors
    result = true;

    done:
        if (fp != NULL) {
            fclose(fp);
        }

        return result;
}


bool queue_put(char *data) {
    FILE *fp = NULL;
    bool success = false;

    fp = fopen(file_name, "r+b");
    if (fp == NULL) {
        LOG(LL_ERROR, ("Can not open file %s", file_name));
        return false;
    }

    // Go to beginning of the file
    fseek(fp, 0, SEEK_SET);

    int head_pos = NULL;
    if (fread(&head_pos, sizeof(head_pos), 1, fp) != 1) {
        LOG(LL_ERROR, ("Failed to read head position of file: %s", file_name));
        goto done;
    }
    LOG(LL_INFO, ("Head Position: %d", head_pos));

    // Adjust the position of file
    fseek(fp, head_pos, SEEK_SET);

    LOG(LL_INFO, ("Writing data %s", data));
    struct mg_str str_data = mg_mk_str(data);

    // Write actual data
    if (fwrite(str_data.p, 1, str_data.len, fp) != str_data.len) {
        LOG(LL_ERROR, ("Failed to write data to file: %s", file_name));
        goto done;
    }

    // Write length of data
    if (fwrite(&str_data.len, sizeof(str_data.len), 1, fp) != 1) {
        LOG(LL_ERROR, ("Failed to write meta data to file: %s", file_name));
        goto done;
    }

    // Go to beginning of the file again
    fseek(fp, 0, SEEK_SET);

    // Update head position
    head_pos += (sizeof(str_data.len) + str_data.len);
    if (fwrite(&head_pos, sizeof(head_pos), 1, fp) != 1) {
        LOG(LL_ERROR, ("Failed to write position header to file: %s", file_name));
        goto done;
    }

    // No errors
    success = true;

    done:
        if (fp != NULL) {
            fclose(fp);
        }

        return success;
}


void queue_peek(peek_callback cb, void *cb_arg) {
    FILE *fp = NULL;
    char *data = NULL;

    fp = fopen(file_name, "r+b");
    if (fp == NULL) {
        LOG(LL_ERROR, ("Can not open file %s", file_name));
        goto done;
    }

    // Go to beginning of the file
    fseek(fp, 0, SEEK_SET);

    int head_pos = NULL;
    if (fread(&head_pos, sizeof(head_pos), 1, fp) != 1) {
        LOG(LL_ERROR, ("Failed to read head position of file: %s", file_name));
        goto done;
    }

    LOG(LL_INFO, ("Head position: %d", head_pos));

    if (head_pos == 4) {
        LOG(LL_INFO, ("No data to peek at"));
        goto done;
    }

    // Read length data
    unsigned int length = NULL;
    fseek(fp, head_pos - sizeof(length), SEEK_SET);
    if (fread(&length, sizeof(length), 1, fp) != 1) {
        LOG(LL_ERROR, ("Failed to read length of data: %s", file_name));
        goto done;
    }

    LOG(LL_INFO, ("Length of data %d", length));
    LOG(LL_INFO, ("Reading from %d to %d", head_pos - sizeof(length) - length, head_pos - sizeof(length)));

    // Read data
    data = (char *) malloc(length + 1);
    if (data == NULL) {
        LOG(LL_INFO, ("Out of memory %s", file_name));
        goto done;
    }

    fseek(fp, head_pos - sizeof(length) - length, SEEK_SET);
    if (fread(data, 1, length, fp) != length) {
        LOG(LL_ERROR, ("Failed to read data: %s", file_name));
        goto done;
    }

    data[length] = '\0'; // Null terminate
    LOG(LL_INFO, ("Data: %s", data));

    done:
        cb(data, cb_arg);

        if (data != NULL) {
            free(data);
        }

        if (fp != NULL) {
            fclose(fp);
        }
}


bool queue_delete() {
    FILE *fp = NULL;
    bool success = false;

    fp = fopen(file_name, "r+b");
    if (fp == NULL) {
        LOG(LL_ERROR, ("Can not open file %s", file_name));
        goto done;
    }

    // Go to beginning of the file
    fseek(fp, 0, SEEK_SET);

    int head_pos = NULL;
    if (fread(&head_pos, sizeof(head_pos), 1, fp) != 1) {
        LOG(LL_ERROR, ("Failed to read head position of file: %s", file_name));
        goto done;
    }
    LOG(LL_INFO, ("Head position %d", head_pos));

    if (head_pos == 4) {
        LOG(LL_INFO, ("Already at the end of the stack"));
        success = true;
        goto done;
    }

    // Read length data
    unsigned int length = NULL;
    fseek(fp, head_pos - sizeof(length), SEEK_SET);
    if (fread(&length, sizeof(length), 1, fp) != 1) {
        LOG(LL_ERROR, ("Failed to read length of data: %s", file_name));
        goto done;
    }
    LOG(LL_INFO, ("Length of data %d", length));

    // Go to beginning of the file
    fseek(fp, 0, SEEK_SET);

    // Update head position
    head_pos -= sizeof(length) + length;
    LOG(LL_INFO, ("New head position %d", head_pos));
    if (fwrite(&head_pos, sizeof(head_pos), 1, fp) != 1) {
        LOG(LL_ERROR, ("Failed to write position header to file: %s", file_name));
        goto done;
    }

    // No errors
    success = true;

    done:
        if (fp != NULL) {
            fclose(fp);
        }

        return success;
}

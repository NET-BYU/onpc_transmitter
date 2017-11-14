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

char *cur_file = NULL;

void find_cur_file() {
    cur_file = "data.0";
}

bool queue_put(char *data) {
    if (cur_file == NULL) {
        find_cur_file();
    }

    fp = fopen(cur_file, "ab");

    if (fp == NULL) {
        LOG(LL_ERROR, ("Can not open file %s", cur_file));
        return false;
    }

    struct mg_str str_data = mg_mk_str(data);
    LOG(LL_INFO, ("Current file: %s", cur_file));
    LOG(LL_INFO, ("Writing data %s", str_data));

    // Write length of data
    if (fwrite(&str_data.len, sizeof(size_t), 1, fp) != sizeof(size_t)) {
        LOG(LL_ERROR, ("Failed to write to file: %s", filename));
        return false;
    }

    // Write actual data
    if (fwrite(str_data.p, 1, str_data.len, fp) != str_data.len) {
        LOG(LL_ERROR, ("Failed to write to file: %s", filename));
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

void write_measurement(char *data) {
    char *filename = "test";
    struct mg_str str_data = mg_mk_str(data);
    FILE *fp = NULL;
    fp = fopen(filename, "ab");

    if (fp == NULL) {
        LOG(LL_ERROR, ("Can not open file %s", filename));
    }

    if (fwrite(str_data.p, 1, str_data.len, fp) != str_data.len) {
        LOG(LL_ERROR, ("Failed to write data %s", filename));
    }

    if (fp != NULL) {
        fclose(fp);
    }
}

#ifndef PHILIPBL_QUEUE_H_
#define PHILIPBL_QUEUE_H_

#include <stdlib.h>
#include <string.h>

#include "common/cs_file.h"
#include "common/json_utils.h"
#include "common/mg_str.h"
#include "mgos_config_util.h"
#include "mgos_sys_config.h"
#include "mgos_vfs.h"

typedef void (*peek_callback)(char *data, void *userdata);

bool start_queue();
bool queue_put(char *data);
void queue_peek(peek_callback cb, void *cb_arg);
bool queue_delete();

#endif /* PHILIPBL_QUEUE_H_ */

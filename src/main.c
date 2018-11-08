#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef RTOS_SDK
#include <esp_common.h>
#else
#include <user_interface.h>
#include <wpa2_enterprise.h>
#endif

#include "common/cs_dbg.h"

#include "mgos_app.h"
#include "mgos_gpio.h"
#include "mgos_mongoose.h"
#include "mgos_mqtt.h"
#include "mgos_sys_config.h"
#include "mgos_system.h"
#include "mgos_timers.h"
#include "mgos_utils.h"
#include "mgos_wifi.h"

#include "mgos_service_config.h"


uint8_t frame[1600] = {
    0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xc0, 0x6c,
    0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00,
    0x64, 0x00, 0x01, 0x04, 0x00, 0x06, 0x72, 0x72,
    0x72, 0x72, 0x72, 0x72, 0x01, 0x08, 0x82, 0x84,
    0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03, 0x01,
    0x0b, 0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x06, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xc0,
    0x6c, 0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00,
    0x00, 0x64, 0x00, 0x01, 0x04, 0x00, 0x06, 0x72,
    0x72, 0x72, 0x72, 0x72, 0x72, 0x01, 0x08, 0x82,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xc0, 0x6c,
    0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00,
    0x64, 0x00, 0x01, 0x04, 0x00, 0x06, 0x72, 0x72,
    0x72, 0x72, 0x72, 0x72, 0x01, 0x08, 0x82, 0x84,
    0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03, 0x01,
    0x0b, 0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x06, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xc0,
    0x6c, 0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00,
    0x00, 0x64, 0x00, 0x01, 0x04, 0x00, 0x06, 0x72,
    0x72, 0x72, 0x72, 0x72, 0x72, 0x01, 0x08, 0x82,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xc0, 0x6c,
    0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00,
    0x64, 0x00, 0x01, 0x04, 0x00, 0x06, 0x72, 0x72,
    0x72, 0x72, 0x72, 0x72, 0x01, 0x08, 0x82, 0x84,
    0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03, 0x01,
    0x0b, 0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x06, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xc0,
    0x6c, 0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00,
    0x00, 0x64, 0x00, 0x01, 0x04, 0x00, 0x06, 0x72,
    0x72, 0x72, 0x72, 0x72, 0x72, 0x01, 0x08, 0x82,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xc0, 0x6c,
    0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00,
    0x64, 0x00, 0x01, 0x04, 0x00, 0x06, 0x72, 0x72,
    0x72, 0x72, 0x72, 0x72, 0x01, 0x08, 0x82, 0x84,
    0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03, 0x01,
    0x0b, 0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x01, 0x02, 0x03, 0x04, 0x05,
    0x06, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xc0,
    0x6c, 0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00,
    0x00, 0x64, 0x00, 0x01, 0x04, 0x00, 0x06, 0x72,
    0x72, 0x72, 0x72, 0x72, 0x72, 0x01, 0x08, 0x82,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03,
    0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03
};

// taps: [7]
// size: 1023
uint8_t symbol_1[1023] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0,
    1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0,
    0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0,
    1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1,
    0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1,
    1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0,
    0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1,
    0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0,
    0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0,
    1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1,
    1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0,
    0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0,
    1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1,
    1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1,
    1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0,
    1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0,
    1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1,
    1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0,
    0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1,
    0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1,
    1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0,
    0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1,
    1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1,
    1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1,
    0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0,
    1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0,
    0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
    0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1,
    1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0
};

// taps: [9, 8, 5]
// size: 1023
uint8_t symbol_2[1023] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1,
    1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1,
    0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0,
    1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0,
    0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0,
    1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1,
    1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0,
    0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0,
    0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1,
    0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1,
    0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0,
    0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0,
    0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0,
    1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1,
    1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0,
    1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0,
    1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0,
    0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0,
    1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1,
    0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1,
    0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0,
    0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0,
    1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0,
    1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1,
    0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0,
    0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0,
    1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0,
    0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0
};

// taps: [9, 7, 6]
// size: 1023
uint8_t symbol_3[1023] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0,
    1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0,
    0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0,
    1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1,
    1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0,
    1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0,
    0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0,
    0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0,
    0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0,
    1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0,
    0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0,
    0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1,
    0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1,
    0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0,
    0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1,
    0, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1,
    1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0,
    1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1,
    0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0,
    1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1,
    0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0,
    1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0,
    0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0,
    0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0,
    1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0
};

uint8_t *symbol = NULL;

static void check_wifi(void *arg);
static void stop_onpc(void *arg);
static void connect_to_wifi(void *arg);
struct mg_connection* onpc_udp_connect();

unsigned int symbol_length = sizeof(symbol_1)/sizeof(symbol_1[0]);
unsigned int symbol_index = 0;
unsigned int num_symbols = 0;

unsigned int pause_time = 13424;
unsigned int frame_size = 1400;

mgos_timer_id hw_timer_id = 0;
mgos_timer_id check_timer_id = 0;
unsigned int disconnected_counter = 0;

unsigned int ONPC_SYMBOLS = 0;
unsigned int DISCONNECT_DURATION = 0;
unsigned int DISCONNECT_DURATION_MIN = 0;
unsigned int DISCONNECT_DURATION_MAX = 0;

unsigned int LED = -1;
unsigned int SEND_DATA = 0;


static void send_symbol(void *arg) {
    if (symbol_index == 0) {
        // We are starting a new symbol
        printf("Starting symbol %d (%d)\n", num_symbols + 1, ONPC_SYMBOLS);

        if (num_symbols >= ONPC_SYMBOLS) {
            // We are done sending symbols
            printf("Done with symbols. Stopping...\n");

            // Stop callback
            stop_onpc(NULL);

            // Reset state
            num_symbols = 0;

            // Try to connect to WiFi in one second
            mgos_set_timer(1000, 0, connect_to_wifi, NULL);

            return;
        }
        else {
            num_symbols += 1;
        }
    }

    if(symbol[symbol_index]) {
        wifi_send_pkt_freedom(frame, frame_size, 0);
    }

    symbol_index = (symbol_index + 1) % symbol_length;

    if (symbol_index % 4 == 0) {
        mgos_gpio_toggle(LED);
    }

    (void) arg;
}

static void stop_onpc(void *arg) {
    printf("Stopping ONPC (%d)...\n", hw_timer_id);

    if(hw_timer_id != 0) {
        mgos_clear_timer(hw_timer_id);
        hw_timer_id = 0;
    }

    (void) arg;
}

static void start_onpc() {
    printf("Starting ONPC...\n");
    hw_timer_id = mgos_set_hw_timer(pause_time, MGOS_TIMER_REPEAT, send_symbol, NULL);
}

static void connect_to_wifi(void *arg) {
    printf("Trying to connect to WiFi again and start WiFi check timer...\n");

    DISCONNECT_DURATION = mgos_rand_range(DISCONNECT_DURATION_MIN, DISCONNECT_DURATION_MAX);
    printf("Disconnect duration: %d\n", DISCONNECT_DURATION);

    mgos_wifi_connect();
    check_timer_id = mgos_set_timer(1000, MGOS_TIMER_REPEAT, check_wifi, NULL);

    (void) arg;
}

static void disconnect_from_wifi() {
    printf("Disconnecting from WiFi and stopping WiFi check timer...\n");
    mgos_wifi_disconnect();
    mgos_clear_timer(check_timer_id);
    check_timer_id = 0;
}

static void run_onpc() {
    printf("Running ONPC for %d symbols...\n", ONPC_SYMBOLS);

    // Disconnect from WiFi (stop the system from retrying)
    disconnect_from_wifi();

    // Run ONPC!
    start_onpc();

    // Set up callback to stop ONPC
    // mgos_set_timer(duration, 0, stop_onpc, NULL);

    // Set up callback to try to connect back to WiFi
    // mgos_set_timer(duration + 1000, 0, connect_to_wifi, NULL);
}

static void check_wifi(void *arg) {
    int rssi = mgos_wifi_sta_get_rssi();
    char *status_str = mgos_wifi_get_status_str();
    enum mgos_wifi_status status = mgos_wifi_get_status();

    if(status == MGOS_WIFI_DISCONNECTED || status == MGOS_WIFI_CONNECTING || rssi == 0) {
        disconnected_counter += 1;
        mgos_gpio_toggle(LED);
    }
    else {
        disconnected_counter = 0;
        mgos_gpio_write(LED, false);
    }

    if(SEND_DATA && status == MGOS_WIFI_IP_ACQUIRED) {
        struct mg_connection *c = mgos_mqtt_get_global_conn();

        if(c != NULL) {
            printf("Sending data\n");
            mg_mqtt_publish(c, mgos_sys_config_get_mqtt_pub(), mgos_mqtt_get_packet_id(),
                            MG_MQTT_QOS(0), "stayin' alive", 13);
        }
        else {
            printf("Unable to send data\n");
        }
    }

    printf("RSSI: %d\n", rssi);
    printf("Status: %s\n", status_str);
    printf("Disconnected timer: %d (%d)\n", disconnected_counter, DISCONNECT_DURATION);

    if(disconnected_counter >= DISCONNECT_DURATION) {
        disconnected_counter = 0;
        run_onpc();
    }

    printf("\n");
    free(status_str);
    (void) arg;
}

static void start(void *arg) {
    printf("Starting...\n");

    DISCONNECT_DURATION = mgos_rand_range(DISCONNECT_DURATION_MIN, DISCONNECT_DURATION_MAX);
    printf("Disconnect duration: %d\n", DISCONNECT_DURATION);

    check_timer_id = mgos_set_timer(1000, MGOS_TIMER_REPEAT, check_wifi, NULL);

    (void) arg;
}


static void ev_handler(struct mg_connection *c, int ev, void *p,
                       void *user_data) {
    struct mg_mqtt_message *msg = (struct mg_mqtt_message *) p;

    // if (ev != MG_EV_POLL) {
    //     LOG(LL_INFO, ("MQTT ev: %d", ev));
    // }

    (void) c;
    (void) ev;
    (void) msg;
    (void) user_data;
}

enum mgos_app_init_result mgos_app_init(void) {
    // Set up RPC for config changes
    mgos_rpc_service_config_init();

    // Get ONPC configuration options
    ONPC_SYMBOLS = mgos_sys_config_get_onpc_symbols();
    unsigned int transmitter_symbol = mgos_sys_config_get_onpc_symbol();
    int tx_power = mgos_sys_config_get_onpc_tx_power();

    if(tx_power > 82) {
        tx_power = 82;
    }
    else if(tx_power < 0) {
        tx_power = 0;
    }
    system_phy_set_max_tpw(tx_power);

    // Set up MAC
    if (mgos_sys_config_get_onpc_mac_enable()) {
        DISCONNECT_DURATION_MIN = mgos_sys_config_get_onpc_mac_disconnect_duration_min();
        DISCONNECT_DURATION_MAX = mgos_sys_config_get_onpc_mac_disconnect_duration_max();
    }
    else {
        DISCONNECT_DURATION_MIN = mgos_sys_config_get_onpc_disconnect_duration();
        DISCONNECT_DURATION_MAX = mgos_sys_config_get_onpc_disconnect_duration();
    }

    // Set up UDP options
    SEND_DATA = mgos_sys_config_get_mqtt_enable();

    // Set up LED
    LED = mgos_sys_config_get_onpc_status_led();
    mgos_gpio_set_mode(LED, MGOS_GPIO_MODE_OUTPUT);
    mgos_gpio_write(LED, false);

    printf("-----------------------------------\n");
    if(transmitter_symbol == 1) {
        printf("Using symbol 1\n");
        symbol = symbol_1;
    }
    else if(transmitter_symbol == 2) {
        printf("Using symbol 2\n");
        symbol = symbol_2;
    }
    else if(transmitter_symbol == 3) {
        printf("Using symbol 3\n");
        symbol = symbol_3;
    }
    else {
        printf("Unknown symbol index\n");
        return MGOS_APP_INIT_ERROR;
    }

    printf("Pause time: %d us\n", pause_time);
    printf("Beacon size: %d bytes\n", frame_size);
    printf("ONPC symbols: %d\n", ONPC_SYMBOLS);
    printf("Min disconnect duration: %d s\n", DISCONNECT_DURATION_MIN);
    printf("Max disconnect duration: %d s\n", DISCONNECT_DURATION_MAX);
    printf("Send data: %d\n", SEND_DATA);
    printf("TX Power: %d\n", tx_power);
    printf("-----------------------------------\n");

    mgos_mqtt_add_global_handler(ev_handler, NULL);

    // Allow system to finish connecting
    mgos_set_timer(mgos_sys_config_get_onpc_delay() * 1000, 0, start, NULL);

    return MGOS_APP_INIT_SUCCESS;
}

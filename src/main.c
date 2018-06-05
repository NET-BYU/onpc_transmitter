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
#include "mgos_sys_config.h"
#include "mgos_system.h"
#include "mgos_timers.h"
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

// taps: None
uint8_t symbol[1023] = {
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


static void check_wifi(void *arg);

unsigned int symbol_length = sizeof(symbol)/sizeof(symbol[0]);
unsigned int symbol_index = 0;

unsigned int pause_time = 13424;
unsigned int frame_size = 1400;

mgos_timer_id hw_timer_id = 0;
mgos_timer_id check_timer_id = 0;
unsigned int disconnected_counter = 0;

unsigned int ONPC_DURATION = 0;
unsigned int DISCONNECT_DURATION = 0;

unsigned int LED = -1;


static void send_symbol(void *arg) {
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

static void run_onpc(int duration) {
    printf("Running ONPC for %d ms...\n", duration);

    // Disconnect from WiFi (stop the system from retrying)
    disconnect_from_wifi();

    // Run ONPC!
    start_onpc();

    // Set up callback to stop ONPC
    mgos_set_timer(duration, 0, stop_onpc, NULL);

    // Set up callback to try to connect back to WiFi
    mgos_set_timer(duration + 1000, 0, connect_to_wifi, NULL);
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

    printf("RSSI: %d\n", rssi);
    printf("Status: %s\n", status_str);
    printf("Disconnected timer: %d\n", disconnected_counter);

    if(disconnected_counter >= DISCONNECT_DURATION) {
        disconnected_counter = 0;
        run_onpc(ONPC_DURATION * 1000);
    }

    printf("\n");
    free(status_str);
    (void) arg;
}

static void start(void *arg) {
    printf("Starting...\n");
    check_timer_id = mgos_set_timer(1000, MGOS_TIMER_REPEAT, check_wifi, NULL);

    (void) arg;
}

enum mgos_app_init_result mgos_app_init(void) {
    mgos_gpio_write(LED, false);
    mgos_rpc_service_config_init();

    ONPC_DURATION = mgos_sys_config_get_onpc_duration();
    DISCONNECT_DURATION = mgos_sys_config_get_onpc_disconnect_duration();
    LED = mgos_sys_config_get_onpc_status_led();

    mgos_gpio_set_mode(LED, MGOS_GPIO_MODE_OUTPUT);

    printf("-----------------------------------\n");
    printf("Pause time: %d us\n", pause_time);
    printf("Beacon size: %d bytes\n", frame_size);
    printf("ONPC duration: %d s\n", ONPC_DURATION);
    printf("Disconnect duration: %d s\n", DISCONNECT_DURATION);
    printf("-----------------------------------\n");

    // Allow system to finish connecting
    mgos_set_timer(10000, 0, start, NULL);

    return MGOS_APP_INIT_SUCCESS;
}

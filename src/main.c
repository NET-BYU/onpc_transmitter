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
#include "mgos_sys_config.h"
#include "mgos_system.h"
#include "mgos_timers.h"


uint8_t packet[128] = { 0x80, 0x00, 0x00, 0x00,
                /*4*/   0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
                /*10*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
                /*16*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
                /*22*/  0xc0, 0x6c,
                /*24*/  0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00,
                /*32*/  0x64, 0x00,
                /*34*/  0x01, 0x04,
                /* SSID */
                /*36*/  0x00, 0x06, 0x72, 0x72, 0x72, 0x72, 0x72, 0x72,
                        0x01, 0x08, 0x82, 0x84,
                        0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03, 0x01,
                /*56*/  0x0b};


uint8_t symbol[64] = {1, 1, 1, 1, 1, 1, 0, 1,
                      0, 1, 0, 1, 1, 0, 0, 1,
                      1, 0, 1, 1, 1, 0, 1, 1,
                      0, 1, 0, 0, 1, 0, 0, 1,
                      1, 1, 0, 0, 0, 1, 0, 1,
                      1, 1, 1, 0, 0, 1, 0, 1,
                      0, 0, 0, 1, 1, 0, 0, 0,
                      0, 1, 0, 0, 0, 0, 0};

uint8_t s_index = 0;


static void inject_timer_cb(void *arg) {
    int beacon_size = (int)arg;

    if(s_index >= 64) {
        // Nothing left to do
        return;
    }

    if(symbol[s_index] == 1) {
        wifi_send_pkt_freedom(packet, beacon_size, 0);
    }

    s_index += 1;
}

enum mgos_app_init_result mgos_app_init(void) {
    int pause_time = mgos_sys_config_get_onpc_pause_time();
    int beacon_size = mgos_sys_config_get_onpc_beacon_size();
    printf("Entering inject mode!\n");
    printf("Pause time: %d\n", pause_time);
    printf("Beacon size: %d\n", beacon_size);
    wifi_set_opmode(STATION_MODE);
    wifi_set_channel(11);

    mgos_set_hw_timer(pause_time, MGOS_TIMER_REPEAT, inject_timer_cb, (void *)beacon_size);
    printf("Done inject mode!\n");

    return MGOS_APP_INIT_SUCCESS;
}

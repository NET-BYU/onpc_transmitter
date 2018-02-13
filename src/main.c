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


uint8_t one_packet[57] = {0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
                          0xff, 0xff, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
                          0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xc0, 0x6c,
                          0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00,
                          0x64, 0x00, 0x01, 0x04, 0x00, 0x06, 0x72, 0x72,
                          0x72, 0x72, 0x72, 0x72, 0x01, 0x08, 0x82, 0x84,
                          0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03, 0x01,
                          0x0b};

uint8_t two_packet[114] = {0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
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
                           0x01, 0x0b};

uint8_t symbol[63] = {1, 1, 0, 1, 0, 1, 0, 1,
                      0, 1, 0, 1, 1, 0, 0, 1,
                      1, 0, 1, 1, 0, 0, 1, 1,
                      0, 1, 0, 0, 1, 0, 0, 0,
                      1, 1, 0, 0, 0, 1, 0, 1,
                      1, 0, 1, 0, 0, 1, 0, 1,
                      0, 0, 0, 1, 1, 0, 0, 0,
                      0, 1, 0, 0, 0, 0, 1};

uint8_t s_index = 0;
unsigned int pause_time = 700;
bool send = true;


// static void send_symbol(void *arg) {
//     if(s_index >= 63) {
//         // Nothing left to do
//         return;
//     }

//     // Check for two 1 in a row
//     if(s_index < 61 && symbol[s_index] == 1 && symbol[s_index + 1] == 1) {
//         // printf("Sending two packets: %u\n", s_index);
//         int result = wifi_send_pkt_freedom(two_packet, 114, 0);

//         if(result == -1) {
//             printf("Unable to send two packet: %u\n", s_index);
//         }

//         // Increment by 2 because we sent two frames
//         s_index += 2;
//     }
//     // Check for one 1
//     else if(symbol[s_index] == 1) {
//         // printf("Sending one packet: %u\n", s_index);
//         int result = wifi_send_pkt_freedom(one_packet, 57, 0);

//         if(result == -1) {
//             printf("Unable to send one packet: %u\n", s_index);
//         }

//         // Just sent one frame
//         s_index += 1;
//     }
//     else {
//         // Zero -- do nothing
//         s_index += 1;
//     }

//     (void) arg;
// }

static void send_on_off(void *arg) {
    if(send) {
        int result = wifi_send_pkt_freedom(one_packet, 57, 0);

        // if(result == -1) {
        //     printf("Unable to send one packet: %u\n", s_index);
        // }

        (void) result;
    }

    send = !send;
    (void) arg;
}


static void start(void *arg) {
    printf("Starting...\n");
    mgos_set_hw_timer(pause_time, MGOS_TIMER_REPEAT, send_on_off, NULL);

    (void) arg;
}

enum mgos_app_init_result mgos_app_init(void) {
    int pause_time = mgos_sys_config_get_onpc_pause_time();
    int beacon_size = mgos_sys_config_get_onpc_beacon_size();

    wifi_set_opmode(STATION_MODE);
    wifi_set_channel(11);

    printf("Entering inject mode!\n");
    printf("Pause time: %d\n", pause_time);
    printf("Beacon size: %d\n", beacon_size);

    mgos_set_timer(3000, 0, start, NULL);

    return MGOS_APP_INIT_SUCCESS;
}

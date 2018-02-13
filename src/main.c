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

uint8_t symbol[63] = {1, 1, 1, 1, 0, 1, 0, 1,
                      0, 1, 0, 1, 1, 0, 0, 1,
                      1, 0, 1, 1, 0, 0, 1, 1,
                      0, 1, 0, 0, 1, 0, 0, 0,
                      1, 1, 0, 0, 0, 1, 0, 1,
                      1, 0, 1, 0, 0, 1, 0, 1,
                      0, 0, 0, 1, 1, 1, 0, 0,
                      0, 1, 0, 0, 0, 0, 1};
unsigned int symbol_length = sizeof(symbol)/sizeof(symbol[0]);

uint8_t data[1] = {1};
unsigned int data_length = sizeof(data)/sizeof(data[0]);

unsigned int symbol_index = 0;
unsigned int data_index = 0;
unsigned int sequence_length = 0;

unsigned int pause_time = 700;

uint8_t MAX_SEQUENCE_LENGTH = 2;


static void send_symbol(void *arg) {
    if(data_index >= data_length) {
        // Sent all data -- nothing left to do
        return;
    }

    if(sequence_length > 0) {
        // Already sent a big frame -- do nothing
        // printf("~~~~~~~~~~~~~~~~~~~~~Already sent big frame: %d %d\n", symbol_index, data_index);
        sequence_length -= 1;
    }
    else if(symbol[symbol_index] != data[data_index]) {
        // Zero -- do nothing
        // printf("~~~~~~~~~~~~~~~~~~~~~Zero: %d %d\n", symbol_index, data_index);
    }
    else {
        // Send a frame
        // printf("Sending a frame: %d %d\n", symbol_index, data_index);

        // Determine how big of a frame
        unsigned int symbol_sequence_index = symbol_index;
        while((symbol_sequence_index < symbol_length) &&
              (symbol[symbol_sequence_index] == data[data_index])) {
            symbol_sequence_index += 1;
        }

        sequence_length = symbol_sequence_index - symbol_index;
        if(sequence_length > MAX_SEQUENCE_LENGTH) {
            sequence_length = MAX_SEQUENCE_LENGTH;
        }

        // printf("symbol_sequence_index: %d\n", symbol_sequence_index);
        // printf("sequence_length: %d\n", sequence_length);
        // printf("~~~~~~~~~~~~~~~~~~~~~Sending frame: %d\n", 57 * sequence_length);

        // Sequence length will always be at least 1
        wifi_send_pkt_freedom(two_packet, 57 * sequence_length, 0);
        sequence_length -= 1;
    }

    // Take care of incrementing indexes
    symbol_index += 1;

    if(symbol_index >= symbol_length) {
        // Done with that symbol so time to go to next bit
        symbol_index = 0;
        data_index += 1;
    }

    // printf("Done: %d %d\n", symbol_index, data_index);

    (void) arg;
}


static void start(void *arg) {
    printf("Starting...\n");
    mgos_set_hw_timer(pause_time, MGOS_TIMER_REPEAT, send_symbol, NULL);

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
    printf("Symbol size: %d\n", symbol_length);
    printf("Data size: %d\n", data_length);

    mgos_set_timer(3000, 0, start, NULL);

    return MGOS_APP_INIT_SUCCESS;
}

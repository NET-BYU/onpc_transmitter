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


static void promisc_cb(uint8_t *buf, uint16_t len) {
  printf("got packet: %d bytes\n", (int) len);
  (void) buf;
}

void promiscuous_mode(void) {
    wifi_set_opmode(STATION_MODE);
    wifi_set_channel(11);
    wifi_promiscuous_enable(0);
    wifi_set_promiscuous_rx_cb(promisc_cb);
    wifi_set_channel(11);
    wifi_promiscuous_enable(1);
}


static void inject_timer_cb(void *arg) {
    int beacon_size = (int)arg;
    int result = wifi_send_pkt_freedom(packet, beacon_size, 0);

    if(result == -1) {
        printf("ERROR! Unable to inject packet!");
    }
}

static void setup_inject_mode_timer_cb(void *arg) {
    int pause_time = mgos_sys_config_get_onpc_pause_time();
    int beacon_size = mgos_sys_config_get_onpc_beacon_size();
    printf("Entering inject mode!\n");
    printf("Pause time: %d\n", pause_time);
    printf("Beacon size: %d\n", beacon_size);
    wifi_set_opmode(STATION_MODE);
    wifi_set_channel(11);

    mgos_set_hw_timer(pause_time, MGOS_TIMER_REPEAT, inject_timer_cb, (void *)beacon_size);
    printf("Done inject mode!\n");
    (void) arg;
}

void start_inject_mode(void) {
    printf("Starting inject mode!\n");
    mgos_set_timer(10000, 0, setup_inject_mode_timer_cb, NULL);
}

static void system_monitor_cb(void *arg) {
    printf("%u\n", mgos_get_free_heap_size());

    (void) arg;
}

void start_system_monitor(void) {
    printf("Starting system monitoring!\n");
    mgos_set_timer(1000, MGOS_TIMER_REPEAT, system_monitor_cb, NULL);
}

enum mgos_app_init_result mgos_app_init(void) {
    // promiscuous_mode();
    printf("Starting!\n");
    start_inject_mode();
    start_system_monitor();
    printf("Done!\n");
    return MGOS_APP_INIT_SUCCESS;
}

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


static void promisc_cb(uint8_t *buf, uint16_t len) {
  printf("got packet: %d bytes\n", (int) len);
  (void) buf;
}

enum mgos_app_init_result mgos_app_init(void) {
    wifi_set_opmode(STATION_MODE);
    wifi_set_channel(8);
    wifi_promiscuous_enable(0);
    wifi_set_promiscuous_rx_cb(promisc_cb);
    wifi_set_channel(8);
    wifi_promiscuous_enable(1);
    return MGOS_APP_INIT_SUCCESS;
}

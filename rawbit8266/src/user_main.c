#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"
#include "c_types.h"

#ifndef PUF_START
#define PUF_START 0x3FFE9000
#endif

#ifndef PUF_LEN
#define PUF_LEN 512
#endif

void user_rf_pre_init(void) {
}

uint32 ICACHE_FLASH_ATTR user_rf_cal_sector_set(void) {
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;
        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;
        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;
        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;
        case FLASH_SIZE_64M_MAP_1024_1024:
            rf_cal_sec = 2048 - 5;
            break;
        case FLASH_SIZE_128M_MAP_1024_1024:
            rf_cal_sec = 4096 - 5;
            break;
        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

static void dump_raw_window(void) {
    volatile uint8_t *p = (volatile uint8_t *)PUF_START;
    uint32 i;

    os_printf("\nRAW_BEGIN addr=0x%08X len=%u\n", PUF_START, PUF_LEN);

    for (i = 0; i < PUF_LEN; i++) {
        os_printf("%02X", p[i]);
        if ((i & 0x0F) == 0x0F) {
            os_printf("\n");
        }
    }

    if ((PUF_LEN & 0x0F) != 0) {
        os_printf("\n");
    }

    os_printf("RAW_END\n");
}

void user_init(void) {
    system_set_os_print(1);
    wifi_set_opmode_current(NULL_MODE);

    os_printf("\nBOOT_OK\n");
    os_delay_us(20000);
    dump_raw_window();
}
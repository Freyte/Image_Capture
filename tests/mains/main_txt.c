#include <stdio.h>
//
#include "f_util.h"
#include "ff.h"
#include "pico/stdlib.h"
#include "rtc.h"
//
#include "hw_config.h"
#include "arducam/arducam.h"
#include "hardware/timer.h"

#define INTERVAL_US 15000000
#define NUM_PHOTOS 2

uint8_t image_buf[324*324];
uint8_t image_tmp[162*162];
uint8_t image[96*96];
uint8_t header[2] = {0x55, 0xAA};

int main() {
    stdio_init_all();
    time_init();
    //printf("\n\nBooted!\n");
    gpio_init(PIN_LED);
    gpio_set_dir(PIN_LED, GPIO_OUT);

    struct arducam_config config;
    config.sccb = i2c0;
    config.sccb_mode = I2C_MODE_16_8;
    config.sensor_address = 0x24;
    config.pin_sioc = PIN_CAM_SIOC;
    config.pin_siod = PIN_CAM_SIOD;
    config.pin_resetb = PIN_CAM_RESETB;
    config.pin_xclk = PIN_CAM_XCLK;
    config.pin_vsync = PIN_CAM_VSYNC;
    config.pin_y2_pio_base = PIN_CAM_Y2_PIO_BASE;

    config.pio = pio0;
    config.pio_sm = 0;

    config.dma_channel = 0;
    config.image_buf = image_buf;
    config.image_buf_size = sizeof(image_buf);

    puts("Hello, world!");
    FIL fil;
    char filename[20];
    int photo_num = 1;
    arducam_init(&config);
    uint16_t x, y, i, j, index;
    UINT bytes_written;

    sd_card_t *pSD = sd_get_by_num(0);
    FRESULT fr = f_mount(&pSD->fatfs, pSD->pcName, 1);
    if (FR_OK != fr)
        panic("f_mount error: %s (%d)\n", FRESULT_str(fr), fr);

    while (photo_num <= NUM_PHOTOS) {
        gpio_put(PIN_LED, !gpio_get(PIN_LED));
        arducam_capture_frame(&config);

        printf("......Taking photo.......\n ");
        i = 0;
        index = 0;
        uint8_t temp = 0;
        for (y = 0; y < 324; y++) {
            for (x = (1 + x) % 2; x < 324; x += 2) {
                image_tmp[index++] = config.image_buf[y * 324 + x];
            }
        }
        index = 0;
        for (y = 33; y < 129; y++) {
            for (x = 33; x < 129; x++) {
                image[index++] = image_tmp[y * 324 + x];
            }
        }

        sprintf(filename, "Dataimage_%d.txt", photo_num);

        fr = f_open(&fil, filename, FA_OPEN_APPEND | FA_WRITE);
        if (FR_OK != fr && FR_EXIST != fr)
            panic("f_open(%s) error: %s (%d)\n", filename, FRESULT_str(fr), fr);

        // Escribir encabezado
        if (f_puts("Header: ", &fil) < 0)
        if (f_puts("Header: ", &fil) < 0) {
            printf("f_puts failed\n");
        }
        
        // Escribir datos de la imagen
        for (i = 0; i < 96*96; i++) {
            if (f_printf(&fil, "%02X ", image[i]) < 0) {
                printf("f_printf failed\n");
            }
        }
        
        // Agregar un salto de línea al final
        if (f_puts("\n", &fil) < 0) {
            printf("f_puts failed\n");
        }

        fr = f_close(&fil);
        if (FR_OK != fr) {
            printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
        }
        f_unmount(pSD->pcName);

        puts("Transfer Done");
        for (;;);
        sleep_us(INTERVAL_US);

        photo_num++;
    }
    return 0;
}

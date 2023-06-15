#include <stdio.h>
#include "f_util.h"
#include "ff.h"
#include "pico/stdlib.h"
#include "rtc.h"
#include "hw_config.h"
#include "arducam/arducam.h"
#include "hardware/timer.h"
#include "string.h"

#define INTERVAL_US 15000000
#define NUM_PHOTOS 1

// Define BMP header
#define WIDTH  324
#define HEIGHT 324
#define MAGIC_VALUE 0x4D42
#define NUM_PLANE 1
#define COMPRESSION 0
#define NUM_COLORS 256
#define IMPORTANT_COLORS 0
#define BITS_PER_PIXEL 24
#define BITS_PER_BYTE 8
#define BYTES_PER_PIXEL BITS_PER_PIXEL / BITS_PER_BYTE 

uint8_t image_buf[324 * 324];

#pragma pack (push) // Save the original data alignment
#pragma pack(1) // Set data alignment to 1 byte boundary

typedef struct BMPHeader_t_ { // Total: 54 bytes
    uint16_t type; // Magic identifier: 0x4D42
    uint32_t size; // File size in bytes
    uint16_t reserved1; // Not used
    uint16_t reserved2; // Not used
    uint32_t offset; // Offset to image data in bytes from beginning of file (54 bytes)
    uint32_t dib_header_size; // DIB Header size in bytes (40 bytes)
    int32_t width_px; // Width of the image
    int32_t height_px; // Height of the image
    uint16_t num_plane; // Number of color planes
    uint16_t bits_per_pixel; // Bits per pixel
    uint32_t compression; // Compression type
    uint32_t image_size_bytes; // Image size in bytes
    int32_t x_resolution_ppm; // Pixels per meter
    int32_t y_resolution_ppm; // Pixels per meter
    uint32_t num_colors; // Number of colors
    uint32_t important_colors; // Important colors
} BMPHeader_t;

static int getPadding(const BMPHeader_t* header) {
    return (4 - (header->width_px * BYTES_PER_PIXEL) % 4) % 4;
}

// Define custom palette
typedef struct RGBColor_t {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t reserved;
} RGBColor_t;

uint16_t i;
FIL fil;
FRESULT fr;
UINT bytes_written;
char filename[16];
  
// Save data in BMP file
void SaveBMP(const uint8_t* image_buf, const char* filename) {
    BMPHeader_t header;
    header.type = MAGIC_VALUE;
    header.size = sizeof(BMPHeader_t) + NUM_COLORS*sizeof(RGBColor_t) +(WIDTH *BYTES_PER_PIXEL* HEIGHT);
    header.reserved1 = 0;
    header.reserved2 = 0;
    header.offset = sizeof(BMPHeader_t) + NUM_COLORS*sizeof(RGBColor_t);
    header.dib_header_size = 40;
    header.width_px = WIDTH;
    header.height_px = HEIGHT;
    header.num_plane = 1;
    header.bits_per_pixel = BITS_PER_PIXEL;
    header.compression = COMPRESSION;
    header.image_size_bytes = WIDTH * HEIGHT*BYTES_PER_PIXEL;
    header.x_resolution_ppm = 0; 
    header.y_resolution_ppm = 0;
    header.num_colors = NUM_COLORS;
    header.important_colors = IMPORTANT_COLORS;
    
    int padding = getPadding(&header);
    
    //Write header
    fr = f_open(&fil, filename, FA_CREATE_ALWAYS | FA_WRITE);
    if (FR_OK != fr && FR_EXIST != fr)
        panic("f_open error:", filename, FRESULT_str(fr), fr);

    if (f_write(&fil, &header, sizeof(BMPHeader_t), &bytes_written) != FR_OK) {
        printf("f_write failed\n");
    }    
    
     //Write the palette
    for (int i = 0; i < NUM_COLORS; i++) {
        uint8_t rgb[4]={i,i,i,0};
        f_write(&fil,rgb, sizeof(rgb), &bytes_written);
    }          
    
    uint32_t padding_bytes = 0;
    
    //Write the data into the file
    for (int y = HEIGHT - 1; y >= 0; y--) {
        for (int x = WIDTH - 1; x >= 0; x--) {  
                   const uint8_t* pixel = &image_buf[x * BYTES_PER_PIXEL/3 + y * WIDTH * BYTES_PER_PIXEL/3];
            f_write(&fil, pixel, sizeof(uint8_t), &bytes_written);
            f_write(&fil, pixel, sizeof(uint8_t), &bytes_written);
            f_write(&fil, pixel, sizeof(uint8_t), &bytes_written);
        }
    
     if (padding > 0) {
            padding_bytes = padding;
            if (f_write(&fil, &padding_bytes, padding, &bytes_written) != FR_OK) {
                printf("f_write padding failed\n");
            }
        }     
    }
   fr = f_close(&fil); // Close the file
    if (FR_OK != fr) {
        printf("f_close error", FRESULT_str(fr), fr);
    }   
}

int main() {
    stdio_init_all();
    time_init();
    gpio_init(PIN_LED);
    gpio_set_dir(PIN_LED, GPIO_OUT);

    // Camera Configuration
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
    
    // Main Code
    puts("Hello!");
    int photo_num = 1;

    arducam_init(&config);
    // SDcard mount it
    sd_card_t* pSD = sd_get_by_num(0);
    FRESULT fr = f_mount(&pSD->fatfs, pSD->pcName, 1);
    if (FR_OK != fr) panic("f_mount error", FRESULT_str(fr), fr);

    // Cycle
    while (photo_num <= NUM_PHOTOS) {
        gpio_put(PIN_LED, !gpio_get(PIN_LED)); //LED Turns ON
        printf("...Taking photo...\n");
        arducam_capture_frame(&config);// Generate frame
        snprintf(filename, sizeof(filename), "image%d.bmp", photo_num); //Generate the name file
        SaveBMP(image_buf, filename); // Save frame
        f_unmount(pSD->pcName); //Dismount SDcard
        puts("Transfer Done");  // Transfer
        
        for (;;); // Stop cycle
        sleep_us(INTERVAL_US); // Time between photos
        photo_num++; 
    }

    return 0;
}

//LINK SECTION LIB
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
#define NUM_PHOTOS 3

//DEFINITION SECTION
#define WIDTH 96
#define HEIGHT 96 
#define MAGIC_VALUE 0x4D42
#define NUM_PLANE 1
#define COMPRESSION 0
#define IMAGE_SIZE_BYTES WIDTH*HEIGHT*BYTES_PER_PIXEL
#define NUM_COLOR 0
#define IMPORTANT_COLORS 0
#define BITS_PER_PIXEL 24
#define BITS_PER_BYTE 8
#define BYTES_PER_PIXEL BITS_PER_PIXEL/BITS_PER_PIXEL 

//GLOBAL DECLARATION SECTION
uint8_t image_buf[324*324];
uint8_t image_tmp[162*162];
uint8_t image[96*96];

#pragma pack (push)
#pragma pack(1)

//STRUCTURE OF BMPHEADER
typedef struct BMPHeader_t_ {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
    uint32_t dib_header_size; // bitmap setting
    int32_t  width_px;
    int32_t  height_px;
    uint16_t num_plane;
    uint16_t bits_per_pixels;           
    uint32_t compression;
    uint32_t image_size_bytes;
    int32_t  x_resolution_ppm;
    int32_t  y_resolution_ppm;
    uint32_t num_color;
    uint32_t important_colors;
} BMPHeader_t; 

static int getPadding (const BMPHeader_t*header){
    return(4- (header->width_px*BYTES_PER_PIXEL)% 4)% 4;
}  
FIL fil;
FRESULT fr;
UINT bytes_written;
char filename[20];

void SaveBMP(const uint8_t*image,char*filename){ //const uint8_t*header

    BMPHeader_t header;
    memset(&header,0,sizeof(BMPHeader_t));    
    header.type = MAGIC_VALUE;
    header.size = sizeof(BMPHeader_t) + (WIDTH * BYTES_PER_PIXEL + getPadding(&header))*HEIGHT;
    header.reserved1 = 0;
    header.reserved2 = 0;
    header.offset = sizeof(BMPHeader_t);
    header.dib_header_size = 40;
    header.width_px = WIDTH;
    header.height_px = HEIGHT;
    header.num_plane = 1;
    header.bits_per_pixels = BITS_PER_PIXEL;
    header.compression = COMPRESSION;
    header.image_size_bytes = IMAGE_SIZE_BYTES;
    header.x_resolution_ppm = 0;
    header.y_resolution_ppm = 0;
    header.num_color =NUM_COLOR;
    header.important_colors = IMPORTANT_COLORS;
 
    int padding=getPadding(&header);
    memset(image,0,sizeof(image));

    fr=f_open(&fil,filename, FA_OPEN_APPEND | FA_WRITE);
    if (FR_OK != fr && FR_EXIST != fr)
        panic("f_open(%s) error: %s (%d)\n", filename, FRESULT_str(fr), fr);

    if (f_write(&fil,&header,sizeof(BMPHeader_t), &bytes_written) !=FR_OK) {
        printf("f_write failed\n");
         }

    uint32_t row_size_bytes = WIDTH*BYTES_PER_PIXEL;
    uint32_t padding_bytes = 0;

    for(int y=95; y >= 0; y--){
        for(int x=0; x< 96; x++){
            uint8_t*pixel=&image[(y*WIDTH + x)*BYTES_PER_PIXEL];
            f_write(&fil,pixel+2,1,&bytes_written); //red
            f_write(&fil,pixel+1,1,&bytes_written);//green
            f_write(&fil,pixel,1,&bytes_written); // blue
     }
        }
    
        if(padding > 0){
            padding_bytes=padding;   
            if (f_write(&fil,&padding_bytes,padding,&bytes_written) !=FR_OK) {
        printf("f_write padding failed\n");
            }
        }
        
    
    fr = f_close(&fil);
        if (FR_OK != fr) {     
        printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
     }
}

int main() {
    //INIZALITE
    stdio_init_all();
    time_init();
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
    uint16_t x, y,i, j,index;
    int photo_num = 1;
    
    arducam_init(&config);

    sd_card_t *pSD = sd_get_by_num(0);
    FRESULT fr = f_mount(&pSD->fatfs, pSD->pcName, 1);
    if (FR_OK != fr) panic("f_mount error: %s (%d)\n", FRESULT_str(fr), fr);

    while(photo_num <= NUM_PHOTOS){
    gpio_put(PIN_LED, !gpio_get(PIN_LED));
    arducam_capture_frame(&config);

    printf("......Talking photo.......\n ");

    i = 0;
    index = 0;
    uint8_t temp = 0;
    for(y = 0; y < 324; y++) {
            for(x = (1 + x) % 2; x < 324; x += 2) {
                image_tmp[index++] = config.image_buf[y*324+x];
            }
    }
    index = 0;
    for(y = 33; y < 129; y++) {
        for(x = 33; x < 129; x++) {
                image[index++] = image_tmp[y*324+x];
        }
    }

    SaveBMP(image, "imagen.bmp"); 

    f_unmount(pSD->pcName);
    puts("Transfer Done");
    

    for (;;);
    sleep_us(INTERVAL_US);
   

    photo_num++;
}
    return 0;
}
   
    

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
#define BITS_PER_PIXEL 8
#define BITS_PER_BYTE 8
#define BYTES_PER_PIXEL BITS_PER_PIXEL / BITS_PER_BYTE 

uint8_t image_buf[324 * 324];
//Define calcule
#define pix 0.28 // 91.13mm/324
#define n_bandes 18 // Raiz of 324 

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

//Imagen Read Structure
typedef struct BMPImage_t_{
    BMPHeader_t header;
    uint8_t*pixel;
}BMPImage_t;

// Define custom palette
typedef struct RGBColor_t {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t reserved;
} RGBColor_t;

uint16_t i,j;
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

    //Write the data into the file
    for (int y = HEIGHT - 1; y >= 0; y--) {
        for (int x = WIDTH - 1; x >= 0; x--) {  
                   const uint8_t* pixel = &image_buf[x * BYTES_PER_PIXEL + y * WIDTH * BYTES_PER_PIXEL];
            f_write(&fil, pixel, sizeof(uint8_t), &bytes_written);
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

        uint8_t*I_red=[WIDTH*HEIGHT];
        for(int i=0; i<HEIGHT; i++){
        for(int j=0. j<WIDTH; j++){
            NUM_COLORS*RGBColor_t**pixel=(NUM_COLORS*RGBColor_t*)&image_buf[x * BYTES_PER_PIXEL + y * WIDTH * BYTES_PER_PIXEL];
            I_red[i][j]=I_init[i][j][0]; // Index Red Color
        }

        int h[256]={0};// matriz zeros
        for(int i=0; i<size_I_red_1; i++){
            for(int j=0; j<size_I_red_2; j++){
                h[I_red[i][j]+1]++
            }
        }

        float ch[256];
        int sum_h=0;
        for(int i=0; i<256; i++){
            sum_h+=h[i];
        ch[i]=(float)sum_h/(size_I_red_1*size_I_red_2); //Accumulate addition /sum(sumof h)
        }

        int cnt=0;
        while(ch[cnt]<0.98){
            cnt++;
        }

        int I_red_mod[WIDTH*HEIGHT]; 
        for(int i=0; i<WIDTH*HEIGHT; i++){
            I_red_mod[i]=min(I_red[i],cnt);
        }

        int I_red_size=0;
        for(int i=0; i<WIDTH*HEIGHT; i++){
            if(I_red_mod[i]>I_red_size){
                I_red_size=I_red_mod[i];
            }
        }

        int Hist_H[HEIGHT];
        for (int i=0; i<WIDTH*HEIGHT; i++){
            Hist_H[i]=0;
            for(int j=0; j<WIDTH; j++){
                Hist_H[i]+= I_red_mod[i*WIDTH+j];
            }
        } 

        int Hist_H_Bandes_Reshape[round(I_red_size/n_bandes)][n_bandes];
        for(int i=0; i<(round(I_red_size/n_bandes));i++){
            for(int j=0; j<n_bandes; j++){
                Hist_H_Bandes_Reshape[i][j]=0;
            }
        }
         
        int Hist_H_Bandes[n_bandes];
        for(int i=0; i< n_bandes; i++){
            Hist_H_Bandes[i]=0;
            for(int j=0; j<HEIGHT/n_bandes; j++){
                Hist_H_Bandes[i] += Hist_H_Bandes_Reshape[i][j];
            }
        }

        int Hist_H_Bandes_par[n_bandes];
        for(int i=0; i<n_bandes; i++){
            Hist_H_Bandes_par[i]=ceil((Hist_H_Bandes[i]/WIDTH)/(HEIGHT/n_bandes));
        }

        float Taux_Bul_Href =0.1;
        float a_H=100/(1-Taux_Bul_Href);
        float b_H=100-a_H;

        float Taux_Bul_Bref =0.25;
        float a_B=100/(1-Taux_Bul_Bref);
        float b_B=100-a_B;

        //Taux H
        float Taux_Bul_H =0;
        for(int i=0; i<3; i++){
            Taux_Bul_H += Hist_H_Bandes_par[i];
        }
        Taux_Bul_H/=(255*3);

        float eq_H=a_H*Taux_Bul_H + b_H;
        Taux_Bul_H=round(eq_H/10)*10;

        //Taux B
        float Taux_Bul_H =0;
        for(int i=n_bandes-2; i<n_bandes; i++){
            Taux_Bul_B += Hist_H_Bandes_par[i];
        }
        Taux_Bul_B/=(255*(n_bandes-(n_bandes-2)));
        float eq_B=a_B*Taux_Bul_B + b_B;
        Taux_Bul_B=fmax(round(eq_H/10)*10,0);

        int Hist_H_Bandes_tot[n_bandes];
        for(int i=0; i<n_bandes; i++){
            Hist_H_Bandes_tot[i]=ceil(Hist_H_Bandes[i]/324/(324/n_bandes)/cnt*64);
        }

        int diff_Hist_H_Bandes_tot[n_bandes-1];
        for(int i=0; i<n_bandes-1; i++){
            diff_Hist_H_Bandes_tot[i]=Hist_H_Bandes_tot[i+1]-Hist_H_Bandes_tot[i];
        }

        int diff_Hist_H_Bandes_par[n_bandes-1];
        for(int i=0; i<n_bandes-1; i++){
            diff_Hist_H_Bandes_par[i]=Hist_H_Bandes_par[i+1]-Hist_H_Bandes_par[i];
        }

        int A_max_diff_Hist= diff_Hist_H_Bandes_tot[n_bandes-1];
        int B_max_diff_Hist= n_bandes-1;
        for(int i=n_bandes-2; i>=0; i--){
            if(diff_Hist_H_Bandes_tot[i]>A_max_diff_Hist){
                A_max_diff_Hist=diff_Hist_H_Bandes_tot[i];
                B_max_diff_Hist=i;
            }
        }
        B_max_diff_Hist=n_bandes-B_max_diff_Hist;

        int A_min_diff_Hist= diff_Hist_H_Bandes_tot[B_max_diff_Hist];
        int B_min_diff_Hist= B_max_diff_Hist;
        for(int i=B_max_diff_Hist+1; i<n_bandes-1; i++){
            if(diff_Hist_H_Bandes_tot[i]<A_min_diff_Hist){
                A_min_diff_Hist=diff_Hist_H_Bandes_tot[i];
                B_min_diff_Hist=i;
            }
        }  //REVISAR ESTE TRAMO
         if(sum(diff_Hist_temp < 0)==0){
             B_min_diff_Hist =n_bandes-1;
         }

        int y_rect=(B_max_diff_Hist+1)*round(324/n_bandes);
        int h_rect=(B_min_diff_Hist+1)*round(324/n_bandes);
        if(h_rect==0){
            y_rect=y_rect-round(324/n_bandes);
            h_rect=round(324/n_bandes);
        }

        int Min_hist=B_max_diff_Hist+B_min_diff_Hist;

        float Mean_Hist_H = 0.0;
        for(int i=0. i<round(I_red_size/n_bandes); i++){
            Mean_Hist_H += Hist_H_Bandes_tot[i];
        }
        Mean_Hist_H /=round(I_red_size/n_bandes);
        
        int Min_hist_H= Hist_H_Bandes_tot[0];
        int Max_hist_H= Hist_H_Bandes_tot[0];
        float Ecart_Hist_H=0.0;

        int Sum_Hist_H=0;
        for(int i=0; i<round(I_red_size/n_bandes)-1; i++){
            Sum_Hist_H += diff_Hist_H_Bandes_tot[i];
        }
        float indice_1_emul=(Max_hist_H-Min_hist_H);
        float indice_2_emul=(Mean_hist_H-Min_hist_H);

        printf("%s-B_max_diff_Hist:%d B_min_diff_Hist:%d", filename,B_max_diff_Hist,B_min_diff_Hist);
        
        int x=400;
        int Tail_front=12;
        char col[]="black";
        
        int val_max=Hist_H_Bandes_tot[0];
        int ind_max=0;
        int val_min=Hist_H_Bandes_tot[0];
        int ind_min=0;
        float Max_dev=B_max_diff_Hist/64.0+0.5;
        float Min_dev=B_min_diff_Hist/64.0+0.5;

        int Hist_Haut[round(I_red_size/n_bandes)][B_max_diff_Hist];
        int A_min_diff_Hist_tot=0;
        int B_min_diff_Hist_tot=0;

        for(int i=0; i<round(I_red_size/n_bandes); i++){
            for(int j=0; j<B_max_diff_Hist; j++){
                Hist_Haut[i][j]=Hist_H_Bandes_tot[i*B_max_diff_Hist+j];
            }
        }

        for(int i=0; i<round(I_red_size/n_bandes)-1; i++){
            if(diff_Hist_H_Bandes_tot[i]<diff_Hist_H_Bandes_tot[A_min_diff_Hist_tot]){
                A_min_diff_Hist_tot=i;
            }
        }
        

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
}
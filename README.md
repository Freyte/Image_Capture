# Image_Capture
Raspberry Pi Pico &amp; Arducam Image Acquisition
Author: Freyra Sanchez

1. Arducam
Link:https://github.com/ArduCAM/RPI-Pico-Cam/tree/master/rp2040_hm01b0
  Libraries
    -arducam
    -image.pio
2. File systems and MicroSDcard
Author : Carl J Kugler III
Link:https://github.com/carlk3/no-OS-FatFS-SD-SPI-RPi-Pico/tree/dev_sdio
  Libraries and .c
   -include
   -src
   -hw_config.c
   -thing_plus.hw_config.c
   -library.json
3. BMP Format
Author :Ilya Siganov
Link:https://gist.github.com/senior-sigan/67645671ff8c4d0c88599fb3ee41308b
The BMP header is based on:
-https://homepages.inf.ed.ac.uk/rbf/BOOKS/PHILLIPS/cips2ed.pdf
-https://en.wikipedia.org/wiki/BMP_file_format

Main.c
The camera takes and processes directly the buffer to BMP format and generate the image.bmp (8bits) Grey scales.

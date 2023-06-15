# Image_Capture
Raspberry Pi Pico &amp; Arducam Image Acquisition.
Author: Freyra Sanchez.  School: ÉCOLE D’INGÉNIEURS ESTIA  Intership:ESTIA RECHERCHE

1. Arducam.
Link:https://github.com/ArduCAM/RPI-Pico-Cam/tree/master/rp2040_hm01b0

  Libraries.
  
    -arducam
    
    -image.pio
    
2. File systems and MicroSDcard.
Author : Carl J Kugler III.

Link:https://github.com/carlk3/no-OS-FatFS-SD-SPI-RPi-Pico/tree/dev_sdio

  Libraries and .c
  
   -include
   
   -src
   
   -hw_config.c
   
   -thing_plus.hw_config.c
   
   -library.json
   
   -http://elm-chan.org/fsw/ff/doc/open.html
Camera and SDcard tested and conneted per link

3. BMP Format.
Author :Ilya Siganov.

Link:https://gist.github.com/senior-sigan/67645671ff8c4d0c88599fb3ee41308b

The BMP header is based on:

-https://homepages.inf.ed.ac.uk/rbf/BOOKS/PHILLIPS/cips2ed.pdf

-https://en.wikipedia.org/wiki/BMP_file_format

-http://paulbourke.net/dataformats/bitmaps/#:~:text=8%20bit%20indexed%20colour&text=As%20with%208%20bit%20grey,such%20a%20colour%20indexing%20system

Main.c

The camera takes and processes directly the buffer to BMP format and generates the image.bmp (8bits) Grey scales.

Make sure to have at the same levels these folders to obtain a correct .uf2 format.

-pico_sdk

-pico-examples

4. Image-graph

The analysis of the liquids in matlab adapted it for image .bmp 324 x 324 (8 bits).

  -main_test_images
  
  -test_seule_images_3_bandes_2023_03_22
  
   -The codegen is test_seule_images_3_bandes_2023_03_22 convert to C++.
   
   -main_analysis has a partial traslation of matlab to C.
  

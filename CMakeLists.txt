# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(simple_example C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

add_subdirectory("src")

# Add executable. Default name is the project name, version 0.1
add_executable(simple_example
    arducam/arducam.c 
    main.c
    hw_config.c
)
# Can leave these off for silent mode:
# add_compile_definitions(USE_PRINTF USE_DBG_PRINTF)
add_compile_definitions(USE_PRINTF)
pico_generate_pio_header(simple_example ${CMAKE_CURRENT_LIST_DIR}/image.pio)

# Add the standard library and FatFS/SPI to the build
target_link_libraries(simple_example 
    pico_stdlib
    FatFs_SPI
    hardware_dma
	hardware_i2c
	hardware_pio
	hardware_pwm
)

pico_set_program_name(simple_example "simple_example")
pico_set_program_version(simple_example "0.1")

# Choose source and destination for standard input and output:
#   See 4.1. Serial input and output on Raspberry Pi Pico in Getting started with Raspberry Pi Pico (https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf)
#   and 2.7.1. Standard Input/Output (stdio) Support in Raspberry Pi Pico C/C++ SDK (https://datasheets.raspberrypi.org/pico/raspberry-pi-pico-c-sdk.pdf):
pico_enable_stdio_uart(simple_example 1)
pico_enable_stdio_usb(simple_example 1)

pico_add_extra_outputs(simple_example)


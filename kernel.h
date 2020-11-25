#ifndef KERNEL_H
#define KERNEL_H

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

#define VGA_ADDRESS 0xB8000
#define BUFSIZE 2200

uint16* vga_buffer = (uint16*)0xB8000;

#define NULL 0
/*
enum vga_color {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    MAGENTA,
    BROWN,
    GREY,
    DARK_GREY,
    BRIGHT_BLUE,
    BRIGHT_GREEN,
    BRIGHT_CYAN,
    BRIGHT_RED,
    BRIGHT_MAGENTA,
    YELLOW,
    WHITE,
};
*/
#endif

#include <stdint.h>

#ifndef ASM_H
#define ASM_H

unsigned char inb(unsigned short port);
void outb(uint16_t port, uint8_t value);

#endif

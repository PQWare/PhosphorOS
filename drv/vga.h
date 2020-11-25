#define VGA_ADDRESS 0xB8000
#define BUFSIZE 2200
#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_SIZE   VGA_WIDTH * VGA_HEIGHT
uint16* vga_buffer = (uint16*)0xB8000;
uint16_t   curPosX = 0;
uint16_t   curPosY = 0;
uint16_t   curPosAbs = 0;
uint8_t    fgc = 7;
uint8_t    bgc = 0;
uint32_t   lastRandVal = 0;
bool       useWinNL = false;

void scrScroll() {
    for (uint16_t i = 0; i < VGA_SIZE - VGA_WIDTH; i++) {
        vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
    }
    for (uint16_t i = VGA_SIZE - VGA_WIDTH; i < VGA_SIZE; i++) {
        vga_buffer[i] = 0;
    }    
    return;
}

void setCursor(int16_t x, int16_t y) {
    if (x < 0) {x = 0;}
    if (y < 0) {y = 0;}
    if (x > VGA_WIDTH - 1) {x = VGA_WIDTH - 1;}
    if (y > VGA_HEIGHT - 1) {y = VGA_HEIGHT - 1;}
    curPosX = x;
    curPosY = y;
    uint16_t pos = y * VGA_WIDTH + x;
    curPosAbs = pos;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
    return;
}

void movCursor(int16_t x, int16_t y) {
    if (x < 0) {setCursor(curPosX + x, curPosY); x = 0;}
    if (y < 0) {setCursor(curPosX, curPosY + y); y = 0;}
    x += curPosX;
    y += curPosY;
    uint16_t pos = y * VGA_WIDTH + x;
    curPosAbs = pos;
    for (uint16_t i = 0; i < pos; i++) {
        curPosX++;
        if (curPosX > VGA_WIDTH - 1) {
            curPosX = 0;
            curPosY++;
            if (curPosY > VGA_HEIGHT - 1) {
                scrScroll();
                curPosY = VGA_HEIGHT - 1;
            }
        }
    }
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
    return;
}

void setCursorAbs(int16_t pos) {
    int x, y;
    curPosX = 0;
    curPosY = 0;
    curPosAbs = 0;
    if (pos < 0) {pos = 0;}
    if (pos > VGA_SIZE - 1) {pos = VGA_SIZE - 1;}
    for (uint16_t i = 0; i < pos; i++) {
        curPosX++;
        if (curPosX > VGA_WIDTH - 1) {
            curPosX = 0;
            curPosY++;
            if (curPosY > VGA_HEIGHT - 1) {
                scrScroll();
                curPosY = VGA_HEIGHT - 1;
            }
        }
    }
    x = curPosX;
    y = curPosY;
    pos = y * VGA_WIDTH + x;
    curPosAbs = pos;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
    return;
}

void movCursorAbs(int16_t pos) {
    if (pos < 0) {setCursorAbs(curPosAbs + pos); return;}
    int x, y;
    for (uint16_t i = 0; i < pos; i++) {
        curPosX++;
        if (curPosX > VGA_WIDTH - 1) {
            curPosX = 0;
            curPosY++;
            if (curPosY > VGA_HEIGHT - 1) {
                scrScroll();
                curPosY = VGA_HEIGHT - 1;
            }
        }
    }
    x = curPosX;
    y = curPosY;
    pos = y * VGA_WIDTH + x;
    curPosAbs = pos;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
    return;
}

void setCursorStyle(uint8_t top, uint8_t btm) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | top);
    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | btm);
    return;
}

void hideCursor() {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
    return;
}

void cls(uint8_t fg, uint8_t bg) {
    for (uint16_t i = 0; i < VGA_SIZE; i++) {
        vga_buffer[i] = (fg * 256) + (bg * 4096);
    }
}

void kprint(char str[]) {
    int strPos = 0;
    while (str[strPos] != '\0') {
        char tmpChar = str[strPos];
        switch (tmpChar) {
            case '\r':
                if (useWinNL) {setCursor(0, curPosY);}
                break;
            case '\n':
                if (!useWinNL) {setCursor(0, curPosY);}
                movCursorAbs(80);
                break;
            case '\b':
                movCursorAbs(-1);
                break;
            default:
                vga_buffer[curPosAbs] = tmpChar + (fgc * 256) + (bgc * 4096);
                movCursorAbs(1);
                break;
        }
        strPos++;
    }
    return;
}

void nprint(int32_t num) {
    char str[16] = {0};
    numToStr(str, sizeof(str) / sizeof(str[0]), num);
    kprint(str);
    return;
}

void color(uint8_t fg, uint8_t bg) {
    fgc = fg;
    bgc = bg;
}

void setChar(int16_t x, int16_t y, char c) {
    if (x < 0) {x = 0;}
    if (y < 0) {y = 0;}
    if (x > VGA_WIDTH - 1) {x = VGA_WIDTH - 1;}
    if (y > VGA_HEIGHT - 1) {y = VGA_HEIGHT - 1;}
    uint16_t pos = y * VGA_WIDTH + x;
    vga_buffer[pos] = c + (fgc * 256) + (bgc * 4096);
}


#include "kernel.h"
#include "lib/numtostr.h"
#include "drv/io.h"
#include "drv/vga.h"
#include "lib/rand.h"
#include "lib/delay.h"

void halt() {
    while (true) {/*cls(randByte() % 16, randByte() % 16);*/}
}

void foo() {delay(100);}

int16_t main() {
    randByte(); randByte(); randByte();
    cls(randByte() % 16, randByte() % 16);
    for (uint16_t i = 0; i < VGA_SIZE; i++) {
        vga_buffer[i] = (randByte() << 8) + randByte();
    }
    setCursorStyle(0, 15);
    setCursor(5, 3);
    kprint("PhosphorOS v");
    //kprint("0.");
    //nprint(randByte());
    kprint(kVER);
    kprint(" \b");
    //movCursorAbs(-1);
    calibrateDelay();
    uint8_t pmem[262144];
    for (uint32_t i = 0; i < sizeof(pmem); i++) {delay(1000); pmem[i] = randByte();/*kprint("\n"); nprint(pmem[i]);*/}
    /*
    pmem[0] = randByte();
    kprint("\n");
    nprint(sizeof(pmem));
    kprint(" ");
    nprint(pmem[0]);
    halt();
    */
    return 0;
}


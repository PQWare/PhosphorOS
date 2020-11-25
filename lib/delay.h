double dCycles = 0;
double cycles = 0;
uint8_t ov = 0;
uint8_t nv = 0;

void calibrateDelay() {
    dCycles = 0;
    outb(0x70, 0);
    ov = inb(0x71);
    //uint8_t nv = 0;
    while (inb(0x71) == ov) {}
    ov = inb(0x71);
    while (inb(0x71) == ov) {dCycles += 0.001;}
    //kprint("\n");
    //kprint(numStr(dCycles));
    dCycles *= 0.9;
    return;
}

void delay(uint16_t ms) {
    cycles = 0;
    //float nCycles = dCycles;
    //dCycles *= 1000
    while (cycles < dCycles * ms) {nv = inb(0x71); cycles += 1;}
    //dCycles = nCylces;
    return;
}


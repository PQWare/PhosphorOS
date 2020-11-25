uint8_t randByte() {
    uint32_t rtcVal = 0;
    for (uint8_t i = 0; i < 0x0e; i++) {
        outb(0x70, i);
        rtcVal += inb(0x71) * lastRandVal * 1.2 - lastRandVal;
    }
    outb(0x70, 0);
    lastRandVal = (uint32)rtcVal * 0.025 + inb(0x71);
    return (uint8)rtcVal;
}


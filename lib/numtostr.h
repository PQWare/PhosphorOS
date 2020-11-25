char *numStr(int32_t num);

static char *numToStr_sub(char *dest, size_t n, int32_t x) {
    if (n == 0) {
        return NULL;
    }
    if (x <= -10) {
        dest = numToStr_sub(dest, n - 1, x / 10);
        if (dest == NULL) {return NULL;}
    }
    *dest = (char) ('0' - x % 10);
    return dest + 1;
}

char *numToStr(char *dest, size_t n, int32_t x) {
    char *p = dest;
    if (n == 0) {return NULL;}
    n--;
    if (x < 0) {
        if (n == 0) {return NULL;}
        n--;
        *p++ = '-';
    } else {
        x = -x;
    }
    p = numToStr_sub(p, n, x);
    if (p == NULL) {return NULL;}
    *p = 0;
    return dest;
}

char *numStr(int32_t num) {
    char *str[16] = {0};
    numToStr(*str, sizeof(str) / sizeof(str[0]), num);
    return *str;
}

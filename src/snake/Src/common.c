#include "snake_common.h"

u16 seedForRand = 0;

u8 buff[128] = {0};

void My_delay_ms(u16 n)
{
    snake_platform_delay_ms(n);
}

u8 My_strlen(char *x)
{
    u8 count = 0;
    while (*x != 0x00) {
        count++;
        x++;
    }
    return count;
}

u16 My_real_rand(void)
{
    return (u16)my_rand();
}

u16 extRealRand4EmbSnake(void)
{
    return (u16)my_rand();
}

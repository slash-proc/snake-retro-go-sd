#include "def_types.h"
#include "embSnakeDevice.h"
#include "gw_draw.h"
#include "snake_platform.h"

#include <stdint.h>

static uint16_t *fb(void)
{
    return snake_framebuffer();
}

void bmp16_line(int x1, int y1, int x2, int y2, u32 clr,
                void *dstBase, uint dstP)
{
    int ii, dx, dy, xstep, ystep, dd;
    u16 *dstL = (u16 *)((uint8_t *)dstBase + y1 * dstP + x1 * 2);
    dstP /= 2;

    if (x1 > x2) {
        xstep = -1;
        dx = x1 - x2;
    } else {
        xstep = +1;
        dx = x2 - x1;
    }

    if (y1 > y2) {
        ystep = -dstP;
        dy = y1 - y2;
    } else {
        ystep = +dstP;
        dy = y2 - y1;
    }

    if (dy == 0) {
        for (ii = 0; ii <= dx; ii++)
            dstL[ii * xstep] = clr;
    } else if (dx == 0) {
        for (ii = 0; ii <= dy; ii++)
            dstL[ii * ystep] = clr;
    } else if (dx >= dy) {
        dd = 2 * dy - dx;
        for (ii = 0; ii <= dx; ii++) {
            *dstL = clr;
            if (dd >= 0) {
                dd -= 2 * dx;
                dstL += ystep;
            }
            dd += 2 * dy;
            dstL += xstep;
        }
    } else {
        dd = 2 * dx - dy;
        for (ii = 0; ii <= dy; ii++) {
            *dstL = clr;
            if (dd >= 0) {
                dd -= 2 * dy;
                dstL += xstep;
            }
            dd += 2 * dx;
            dstL += ystep;
        }
    }
}

void gw_plot(u16 x, u16 y, COLOR clr)
{
    if (x >= SCREEN_W || y >= SCREEN_H)
        return;
    fb()[y * SCREEN_W + x] = clr;
}

void gw_line(u16 x1, u16 y1, u16 x2, u16 y2, COLOR clr)
{
    bmp16_line(x1, y1, x2, y2, clr, fb(), SCREEN_W * 2);
}

void gw_line_width(u16 x1, u16 y1, u16 x2, u16 y2, u16 width, COLOR color)
{
    u16 w;
    for (w = 0; w < width; w++)
        gw_line(x1 + w, y1, x2 + w, y2, color);
}

void gw_rect(u16 left, u16 top, u16 right, u16 bottom, COLOR clr)
{
    gw_line_width(left, top, left, bottom, right - left, clr);
}

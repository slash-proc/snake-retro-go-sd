/**************************************************
 * File  : embSnakeDevice.c
 * Author: MANDA (slipperstree@gmail.com)
 * Port  : Retro-Go SD homebrew glue
 ***************************************************/

#include "embSnakeDevice.h"
#include "boradSupport.h"
#include "gw_draw.h"
#include "snake_platform.h"

extern u8 buff[128];

u16 drawAreaStartX = 0;
u16 drawAreaStartY = 0;
u16 drawAreaEndX = 0;
u16 drawAreaEndY = 0;
u16 drawAreaCurrX = 0;
u16 drawAreaCurrY = 0;

void devDisplayInit(u16 bgColor)
{
    (void)bgColor;
    devScreenOFF();
    devScreenON();
}

void devPrepareForDrawArea(u16 startX, u16 startY, u16 areaWidth, u16 areaHeight)
{
    drawAreaStartX = startX;
    drawAreaStartY = startY;
    drawAreaEndX = startX + areaWidth - 1;
    drawAreaEndY = startY + areaHeight - 1;
    drawAreaCurrX = drawAreaStartX;
    drawAreaCurrY = drawAreaStartY;
}

void devPointInDrawArea(u16 color)
{
    if (drawAreaCurrX > drawAreaEndX) {
        drawAreaCurrX = drawAreaStartX;
        drawAreaCurrY++;
    }

    if (drawAreaCurrY > drawAreaEndY || drawAreaCurrY >= SCREEN_H ||
        drawAreaCurrX >= SCREEN_W)
        return;

    gw_plot(drawAreaCurrX, drawAreaCurrY, color);
    drawAreaCurrX++;
}

void devDrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 width, u16 color)
{
    gw_line_width(x1, y1, x2, y2, width, color);
}

void devFillRectange(u16 x, u16 y, u16 width, u16 height, u16 color)
{
    u16 wTmp;
    u16 hTmp;

    devPrepareForDrawArea(x, y, width, height);
    wTmp = width;
    while (wTmp--) {
        hTmp = height;
        while (hTmp--)
            devPointInDrawArea(color);
    }
}

void devScreenOFF(void)
{
    snake_lcd_backlight_off();
}

void devScreenON(void)
{
    snake_lcd_backlight_on();
}

void devLoadSetting(SaveData_Struct *setting)
{
    snake_dev_load_setting(setting);
}

void devSaveSetting(SaveData_Struct *setting)
{
    snake_dev_save_setting(setting);
}

void devSndInit(void)
{
}

void devSndBeepShort(void)
{
    snake_sound_queue_beep(880, 40);
}

void devPlaySound(Sound_Type soundType)
{
    switch (soundType) {
    case SOUND_EAT_APPLE:
        snake_sound_queue_beep(740, 50);
        break;
    case SOUND_MOVE:
        break;
    case SOUND_MENU:
        snake_sound_queue_beep(400, 30);
        break;
    case SOUND_DEAD:
        snake_sound_queue_beep(180, 180);
        break;
    case SOUND_GAMEOVER:
        snake_sound_queue_beep(220, 280);
        break;
    case SOUND_HISCORE:
        snake_sound_queue_beep(880, 120);
        break;
    default:
        break;
    }
}

void devEnterHomePage(void)
{
}

void devEnterDemoPage(void)
{
}

void devEnterGamePage(void)
{
}

void devEnterGameOverPage(void)
{
}

void devSpeedChanged(u16 speed)
{
    switch (speed) {
    case SPEED_DEMO_L:
        LED_ALL_OFF();
        break;
    case SPEED_DEMO_M:
        LED_R_ON();
        LED_Y_OFF();
        LED_G_OFF();
        break;
    case SPEED_DEMO_H:
        LED_R_ON();
        LED_Y_ON();
        LED_G_OFF();
        break;
    case SPEED_DEMO_S:
        LED_ALL_ON();
        break;
    default:
        break;
    }
}

/**************************************************
 * File  : embSnakeDevice.h
 * Author: MANDA (slipperstree@gmail.com)
 * Port  : Retro-Go SD homebrew
 ***************************************************/

#ifndef __EMB_SNAKE_DEVICE_H_
#define __EMB_SNAKE_DEVICE_H_

#include "snake_common.h"

/* Key scan mode: down/up events (no click / double-click). */
#define USE_KEY_DOWN_UP
#define NOT_USE_DBCLICK

#define KEY_DOWN_VALUE 1
#define KEY_UP_VALUE   0

#define KEY_CNT  10

#define BTN1   (buttons & B_Up)?KEY_DOWN_VALUE:KEY_UP_VALUE
#define BTN2   (buttons & B_Down)?KEY_DOWN_VALUE:KEY_UP_VALUE
#define BTN3   (buttons & B_Left)?KEY_DOWN_VALUE:KEY_UP_VALUE
#define BTN4   (buttons & B_Right)?KEY_DOWN_VALUE:KEY_UP_VALUE
#define BTN5   (buttons & B_A)?KEY_DOWN_VALUE:KEY_UP_VALUE
#define BTN6   (buttons & B_B)?KEY_DOWN_VALUE:KEY_UP_VALUE
#define BTN7   (buttons & B_GAME)?KEY_DOWN_VALUE:KEY_UP_VALUE
#define BTN8   (buttons & B_TIME)?KEY_DOWN_VALUE:KEY_UP_VALUE
#define BTN9   (buttons & B_PAUSE)?KEY_DOWN_VALUE:KEY_UP_VALUE
#define BTN10  (buttons & B_POWER)?KEY_DOWN_VALUE:KEY_UP_VALUE

/* Frame-based timings (CTL_run is called once per ~60 Hz frame). */
#define TICK_WAIT_CLICK_END         0x4000
#define TICK_WAIT_DBCLICK_START     0x0050
#define TICK_KEEPDOWN               0x0010
#define TICK_KEEPDOWN_NEXT          0x0009

#define XIAO_DOU 1

#define SCREEN_W  320
#define SCREEN_H  240

#define COLOR_DEFAULT_BACK      COLOR_BLACK
#define COLOR_DEFAULT_FRONT     COLOR_SKYLT
#define COLOR_TITLE1            COLOR_SKYLT
#define COLOR_TITLE2            COLOR_GREENLT
#define COLOR_DEFAULT_SNAKE     0xa50c
#define COLOR_DEFAULT_APPLE     COLOR_YELLOW
#define COLOR_DEFAULT_FRAME     COLOR_DEFAULT_FRONT

#define BLOCK_VIEW_FONT FONT_IMG_SNAKE12

#define FRAME_THICKNESS 1

#define GAME_AREA_X_OFFSET 2
#define GAME_AREA_Y_OFFSET 0

#define UI_LANG_EN

void devDisplayInit(u16 bgColor);
void devFillRectange(u16 x, u16 y, u16 width, u16 height, u16 color);
void devPrepareForDrawArea(u16 startX, u16 startY, u16 areaWidth, u16 areaHeight);
void devPointInDrawArea(u16 color);
void devShowChar(u16 x, u16 y,
                        u16 fontWidth, u16 fontHeight, u16 charDataStartIndex,
                        u8* fontData,
                        u16 colorBg, u16 colorFont);
void devDrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 width, u16 color);
void devScreenOFF(void);
void devScreenON(void);

void devEnterHomePage(void);
void devEnterDemoPage(void);
void devEnterGamePage(void);
void devEnterGameOverPage(void);

void devSpeedChanged(u16 speed);

typedef enum
{
    SOUND_EAT_APPLE = 0 ,
    SOUND_MOVE = 1 ,
    SOUND_DEAD = 2 ,
    SOUND_HISCORE = 3 ,
    SOUND_MENU = 4,
    SOUND_GAMEOVER = 5
} Sound_Type;
void devPlaySound(Sound_Type soundType);

void devSndInit(void);
void devSndBeepShort(void);

typedef struct
{
    u16  hiScore;
    u8 soundOnOff;
    u16  colorBackGround;
    u16  colorFront;
    u16  colorSnake;
    u16  colorApple;
    u16  colorFrame;
} SaveData_Struct;
void devLoadSetting(SaveData_Struct *setting);
void devSaveSetting(SaveData_Struct *setting);

/* Implemented in main.c — persist via odroid_settings. */
void snake_dev_load_setting(SaveData_Struct *setting);
void snake_dev_save_setting(SaveData_Struct *setting);

#endif

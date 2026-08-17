/**************************************************
 * File  : control.c
 * Author: MANDA (slipperstree@gmail.com)
 * Date  : 2019.11
 * Updt  : 2021.07
 * Func  : Ì°Ê³ÉßÓÎÏ·¿ØÖÆÆ÷£¬Í³¹ÜsnakeºÍdisplayµÈÄ£¿é
***************************************************/

#include "snake_common.h"
#include "embSnake.h"
#include "control.h"
#include "display.h"
#include "key.h"
#include "saveData.h"
#include "embSnakeDevice.h"


#if ISDEBUG
extern void devShowString(u16 x,u16 y, u8 *str, Font_Type *fontType, u16 colorBg, u16 colorFont);
extern void gameover();
#endif

u16 nowSpeed;
u16 lastDemoSpeed;

// »­ÃæÇ¨ÒÆ×´Ì¬»ú
#define MODE_WELCOME 0
#define MODE_WELCOME_DEMO 1
#define MODE_GAME 2
#define MODE_GAMEOVER 3
#define MODE_INFO 4

u8 nowMode;
u16 maxDemoScore, totalDemoScore, totalDemoCnt, avgDemoScore, lastDemoScore;
u16 oldSaveDataHScore=0, lastGameScore=0;
u8 welcomeWaitTime;
u8 tmpX, tmpY;
u8 flashFlag = 0, flashOnOff = 0;

// ¸÷ÖÖÈ«¾ÖÐÄÌøflg£¬ÔÚ¶¨Ê±Æ÷ÖÐµÝÔö
u16 ttWalk = 0,ttFlag = 0;

// Îª±àÒëÍ¨¹ý£¬ÄÚ²¿º¯ÊýÌáÇ°¶¨Òå²¿·Ö£¨Íâ²¿²»ÐèÒªµ÷ÓÃ£¬²»·ÅÔÚÍ·ÎÄ¼þÖÐ£©
void goPageWelcome(u8 isStartup);
void eventSnake();
void setDemoSpeed(u16 speed);
void switchSound();
void resetDemoWall();

#ifdef SNAKE_AI_DEBUG
u8 isPauseAfterEatAppleMode, isPauseEveryStep, isPause;
void updateBlockAI(u8 x, u8 y);
void updateAllAI();
#endif

// ÁÙÊ±±äÁ¿ÓÃ(sprintfµÈ)
extern u8 buff[128];

// =====================================
// ÊÂÎï´¦Àí´óÑ­»·£¬ÐèÒªÔÚmainº¯ÊýÖÐÑ­»·µ÷ÓÃ
// =====================================
void CTL_run(){
	u16 flashSpeed=0;

    KEY_keyscan();

    
    // TODO:G&W? ±¾À´ÊÇÔÚ¶¨Ê±Æ÷ÖÐ¶ÏÀïÃæ×öµÄ£¬GBA¶¨Ê±Æ÷ÖÐ¶ÏÓÐÂð£¿
    ttWalk++;
    ttFlag++;

    #if ISDEBUG
    devShowString(0,0, "D", &FONT32, COLOR_BLACK, COLOR_YELLOW);
    #endif
    
    if (nowMode == MODE_WELCOME_DEMO)
    {
        flashSpeed = 30;
    } else {
        flashSpeed = 30;
    }

    if (ttFlag > flashSpeed)
    {
        ttFlag = 0;
        flashFlag = 1;
        if (flashOnOff == 1)
        {
            flashOnOff = 0;
        } else {
            flashOnOff = 1;
        }
    }

    switch (nowMode)
    {
        case MODE_WELCOME:
            #if ISDEBUG && DEBUG_SKIP_HOME
                welcomeWaitTime = 100;
            #endif

            // »¶Ó­»­Ãæ³¬¹ýÒ»¶ÎÊ±¼ä£¨ÉÁË¸¼¸´Îºó£©£¬½øÈë×Ô¶¯ÑÝÊ¾DemoÄ£Ê½
            if (welcomeWaitTime >= 10)
            {
                devEnterDemoPage();

                nowMode = MODE_WELCOME_DEMO;

                // »æÖÆDemo»­Ãæ
                DISP_drawDemo(gSetting.soundOnOff);
                welcomeWaitTime = 0;

                // Ò»¶¨ÒªÏÈ»æÖÆDemo»­Ãæ!! ±£Ö¤¿ò¼ÜµÄÎ»ÖÃ±»ÕýÈ·ÉèÖÃ
                // Ò»µ©restartSNAKE¾Í»á³¢ÊÔ»æÖÆ×î³õµÄ¼¸¸öblock£¨³õÊ¼Éß£¬Ê³Îï£©
                // Õâ¸öº¯ÊýÀïÃæ»áÏÈÇå³ýµØÍ¼ÉÏËùÓÐÄÚÈÝ°üÀ¨ÉßÊ³ÎïºÍÕÏ°­Îï,È»ºó»á»Øµ÷Ò»¸öRESTARTµÄÊÂ¼þ£¬Èç¹ûÐèÒªÕÏ°­ÎïÔÚÊÂ¼þÏìÓ¦ÖÐÉèÖÃÕÏ°­Îï
                // »Øµ÷º¯ÊýÖ´ÐÐÍê±Ïºó»áÉèÖÃ¹Ì¶¨µÄ¿ªÊ¼Î»ÖÃ£¨×óÉÏ½ÇºáÏò3¸öblockÉßÍ·ÏòÓÒ£©²¢ÔÚÃ»ÓÐÕÏ°­ÎïµÄ¿ÕÎ»Ëæ»úÉú³ÉÒ»¸öÊ³Îï¡££¨ËùÒÔÉèÖÃÕÏ°­Îï²»¿ÉÕ¼ÓÃ×óÉÏ3¸öblock£©
                SNAKE_restart();
            } else {
                // »­Ãæ¶¯Ì¬Ð§¹û
                if (flashFlag) {
                    DISP_flashWelcome(flashOnOff);
                    welcomeWaitTime++;
                }
            }
            break;

        case MODE_WELCOME_DEMO:

            #if ISDEBUG
                // ³ÔN¸öÆ»¹û¾ÍÇ¿ÖÆ½áÊødemo
                if (SNAKE_getNowScroe() > DEBUG_DEMO_APPLE){
                    // Ä£Äâgameover
                    LOG("Ä£Äâgameover...\r\n");
                    SnakeEventId = SNAKE_EVENT_GAMEOVER;
                    SnakeEventParam1 = SNAKE_getNowScroe();
                    SnakeEventParam2 = 0;
                    eventSnake();
                    LOG("Ä£Äâgameover done.\r\n");
                    return;
                }
            #endif

            // AIÇ°½øÒ»²½
            if (ttWalk >= nowSpeed)
            {
                ttWalk = 0;
                SNAKE_AI_moveNext();

                #ifdef SNAKE_AI_DEBUG
                updateAllAI();
                #endif
            }

            if (flashFlag) {
                DISP_flashDemo(flashOnOff);
            }
            
            break;

        case MODE_GAME:
            // ×Ô¶¯Ç°½ø
            if (ttWalk >= nowSpeed)
            {
                ttWalk = 0;
                SNAKE_moveNext();
            }
            break;
        case MODE_GAMEOVER:
            // Gameover»­Ãæ¶¯Ì¬Ð§¹û
            if (flashFlag == 1) {
                DISP_flashGameOver(flashOnOff, lastGameScore > oldSaveDataHScore ? 1 : 0);
            }
            break;
        case MODE_INFO:
            if (flashFlag == 1) {
                DISP_flashInfo(flashOnOff);
            }
            break;
        default:
            break;
    }

    // »Ö¸´±êÖ¾,±ÜÃâÔÚµÈÓÚ1µÄÆÚ¼ä·´¸´Ë¢ÐÂ
    flashFlag = 0;
}

// =====================================
// °´¼ü¹²Í¨´¦Àí
// =====================================
void doBtnCommon(u8 btnNo, u8 event_id){

    switch (nowMode)
    {
    // ±êÌâÒ³ -----------------------------------
    case MODE_WELCOME:
        switch (event_id)
        {
        // °´¼ü°´ÏÂ
        case KEY_EVENT_DOWN:
            // LOG("KEY_EVENT_DOWN in MODE_WELCOME\r\n");
            // sprintf(buff, "btn=%b2d nowModeA=%b2d\r\n", btnNo, nowMode);LOG(buff);

            devEnterGamePage();

            // ÈÎÒâ¼ü ´Ó±êÌâ»­Ãæ ½øÈëÓÎÏ·»­Ãæ
            nowMode = MODE_GAME;
            // »æÖÆÓÎÏ·»­Ãæ
            DISP_drawGame(gSetting.soundOnOff);

            // SNAKE_restartÖ®Ç°Ò»¶¨ÒªÏÈ»æÖÆÓÎÏ·»­Ãæ!! ±£Ö¤¿ò¼ÜµÄÎ»ÖÃ±»ÕýÈ·ÉèÖÃ
            SNAKE_restart();
            break;
        default:
            break;
        }
        break;
    // DemoÒ³ -----------------------------------
    case MODE_WELCOME_DEMO:
        switch (event_id)
        {
        // °´¼ü°´ÏÂ
        case KEY_EVENT_DOWN:
            // LOG("KEY_EVENT_DOWN in MODE_WELCOME_DEMO\r\n");
            // sprintf(buff, "btn=%b2d nowModeA=%b2d\r\n", btnNo, nowMode);LOG(buff);

            // ¼ÌÐøÅÐ¶ÏÊÇÄÄ¸ö°´¼ü
            switch (btnNo)
            {
            case KEY_B:
                // Demo-°´¼üB=ÇÐ»»ËÙ¶È
                if (nowSpeed == SPEED_DEMO_L) {
                    setDemoSpeed(SPEED_DEMO_M);
                } else if (nowSpeed == SPEED_DEMO_M) {
                    setDemoSpeed(SPEED_DEMO_H);;
                } else if (nowSpeed == SPEED_DEMO_H) {
                    setDemoSpeed(SPEED_DEMO_S);
                } else if (nowSpeed == SPEED_DEMO_S) {
                    setDemoSpeed(SPEED_DEMO_L);
                }
                break;
            case KEY_GAME:
                devEnterGamePage();
                // ½øÈëÓÎÏ·»­Ãæ
                nowMode = MODE_GAME;
                // »æÖÆÓÎÏ·»­Ãæ
                DISP_drawGame(gSetting.soundOnOff);
                // SNAKE_restartÖ®Ç°Ò»¶¨ÒªÏÈ»æÖÆÓÎÏ·»­Ãæ!! ±£Ö¤¿ò¼ÜµÄÎ»ÖÃ±»ÕýÈ·ÉèÖÃ
                SNAKE_restart();
                break;
            // case KEY_PAUSE:
            //     // Demo-°´¼üSELECT=ÇÐ»»ÉùÒô
            //     switchSound();
            //    break;
            default:
                // Demo-ÆäËû°´¼ü=·µ»Ø±êÌâ»­Ãæ
                goPageWelcome(DISP_NO);
                break;
            }
            break;
        default:
            break;
        }
        break;
    // ÓÎÏ·Ò³ -----------------------------------
    case MODE_GAME:
        switch (event_id)
        {
        // °´¼ü°´ÏÂ
        case KEY_EVENT_DOWN:
            // LOG("KEY_EVENT_DOWN in MODE_GAME\r\n");
            // sprintf(buff, "btn=%b2d nowModeA=%b2d\r\n", btnNo, nowMode);LOG(buff);

            // ÊÖ¶¯ÒÆ¶¯³É¹¦µÄ»°£¬×Ô¶¯Ç°½ø¼ÆÊ±ÇåÁã
            if ((btnNo == KEY_UP && SNAKE_moveUp()) || 
                (btnNo == KEY_DOWN && SNAKE_moveDown()) ||
                (btnNo == KEY_LEFT && SNAKE_moveLeft()) ||
                (btnNo == KEY_RIGHT && SNAKE_moveRight())
                ) {
                // ÊÖ¶¯ÒÆ¶¯³É¹¦µÄ»°£¬×Ô¶¯Ç°½ø¼ÆÊ±ÇåÁã
                ttWalk = 0;
            }

            // if (btnNo == KEY_PAUSE) {
            //     // ÇÐ»»ÉùÒô
            //     switchSound();
            // }

            if (btnNo == KEY_TIME) {
                // ½øÈëDemo
                devEnterDemoPage();
                nowMode = MODE_WELCOME_DEMO;
                // »æÖÆDemo»­Ãæ
                DISP_drawDemo(gSetting.soundOnOff);
                SNAKE_restart();
            }
            break;
        // °´¼ü°´±»°´×¡²»·Å£¨Á¬·¢£©
        case KEY_EVENT_KEEPING_PRESS:
            if ((btnNo == KEY_UP && SNAKE_moveUp()) || 
                (btnNo == KEY_DOWN && SNAKE_moveDown()) ||
                (btnNo == KEY_LEFT && SNAKE_moveLeft()) ||
                (btnNo == KEY_RIGHT && SNAKE_moveRight())
                )
            {
                SNAKE_moveNext();
                ttWalk = 0;
            }
            break;
        default:
            break;
        }
        break;
    // GameOverÒ³ -----------------------------------
    case MODE_GAMEOVER:
        switch (event_id)
        {
        // °´¼ü°´ÏÂ
        case KEY_EVENT_DOWN:
            if (btnNo == KEY_TIME) {
                // Info page (TIME â€” PAUSE owned by Retro-Go)
                DISP_drawInfo();
                nowMode = MODE_INFO;
            } else {
                // ÆäËû¼ü»Øµ½±êÌâ»­Ãæ
                goPageWelcome(DISP_NO);
            }
            
            break;
        default:
            break;
        }
        
        break;
    // ½éÉÜÒ³ -----------------------------------
    case MODE_INFO:
        // °´ÈÎÒâ¼ü»Øµ½±êÌâ»­Ãæ
        switch (event_id)
        {
        // °´¼ü°´ÏÂ
        case KEY_EVENT_DOWN:
            goPageWelcome(DISP_NO);
            break;
        default:
            break;
        }
        
        break;
    default:
        break;
    }
}

// =====================================
// °´¼ü¸ö±ð´¦Àí£¬ÏìÓ¦°´¼üÊÂ¼þ
// =====================================
void eventKey1(u8 event_id){
    doBtnCommon(KEY_UP, event_id);
}

void eventKey2(u8 event_id){
    doBtnCommon(KEY_DOWN, event_id);
}

void eventKey3(u8 event_id){
    doBtnCommon(KEY_LEFT, event_id);
}

void eventKey4(u8 event_id){
    doBtnCommon(KEY_RIGHT, event_id);
}

void eventKey5(u8 event_id){
    doBtnCommon(KEY_A, event_id);
}

void eventKey6(u8 event_id){
    doBtnCommon(KEY_B, event_id);
}

void eventKey7(u8 event_id){
    doBtnCommon(KEY_GAME, event_id);
}

void eventKey8(u8 event_id){
    doBtnCommon(KEY_TIME, event_id);
}

void eventKey9(u8 event_id){
    doBtnCommon(KEY_PAUSE, event_id);
}

void powerKey(u8 event_id){
    /* Power / deep-sleep is owned by Retro-Go firmware. */
    (void)event_id;
}


void eventSnake(){
    switch (SnakeEventId)
    {
    case SNAKE_EVENT_UPDATE_BLOCK:
        // Ö»ÔÚÓÎÏ·»òÕßDEMOÒ³ÏìÓ¦Õâ¸öÊÂ¼þ£¨±ÜÃâ·ÇÓÎÏ·×´Ì¬¶¯Ì¬ÉèÖÃÇ½ÌåÊ±Ë¢ÐÂ»­Ãæ£©
        if (nowMode != MODE_GAME && nowMode != MODE_WELCOME_DEMO)
        {
            // ·ÇÓÎÏ·/DemoÒ³Ãæ²»¸üÐÂ»­Ãæ
            //return;
        }
        
        DISP_updateGameBlock(SnakeEventParam1, SnakeEventParam2);
        break;
    #ifdef SNAKE_AI_DEBUG
    case SNAKE_EVENT_UPDATE_AI_PATH:
        if (SNAKE_getNowScroe() >= 5)
        {
            updateBlockAI(SnakeEventParam1, SnakeEventParam2);
        }
        break;
    #endif
    case SNAKE_EVENT_RESTART:
        // Èç¹ûÐèÒªÓÃµ½ÕÏ°­Îï¹¦ÄÜ£¬±ØÐëÔÚÕâ¸öÊÂ¼þµãÖÐÉèÖÃ¡£ÇÒ²»¿ÉÊ¹ÓÃ×óÉÏ½Ç1X3µÄÎ»ÖÃ¡££¨Ô¤Áô¸ø³õÊ¼ÉßÉíÌåÓÃ£©
        // ´¦ÀíË³ÐòÈçÏÂ
        // 1-SnakeÄ£¿éµÄrestartº¯ÊýÖÐÊ×ÏÈ½«mapÈ«²¿Çå³ý,°üÀ¨ÕÏ°­Îï¡£
        // 2-SnakeÄ£¿é»Øµ÷´ËÊÂ¼þ¡£
        // 3-ÔÚ´ËÊÂ¼þÖÐ¸ù¾ÝÐèÒªÌí¼ÓÕÏ°­Îï¡££¨¿ª·¢ÕßÊµÏÖ£©
        // 4-SnakeÄ£¿éÔÚ×óÉÏ½ÇÉú³É1X3´óÐ¡µÄÉßµÄÉíÌå¡£¿ÕµØËæ»úÉú³ÉµÚÒ»¸öÊ³Îï¡£ÓÎÏ·¿ªÊ¼¡£
        // ËùÒÔ£¬ÔçÁË»áÔÚ1´¦±»Çå³ý¡£ÍíÁËÓÐ¿ÉÄÜ¸úÔÚ4´¦Ëæ»úÉú³ÉµÄÊ³ÎïÎ»ÖÃ³åÍ»¡£Äã²¢²»ÖªµÀÊ³ÎïÔÚÄÄÀï£¬ËùÒÔÒªÔÚËæ»úÉú³ÉÊ³ÎïÖ®Ç°£¬MapÇå³ýÖ®ºóÌí¼ÓÕÏ°­Îï¡£
        if (nowMode == MODE_WELCOME_DEMO){
            // ÉèÖÃDemo»­ÃæµÄÕÏ°­Îï
            resetDemoWall();
            DISP_drawWall();
            setDemoSpeed(lastDemoSpeed);
        } else {
            nowSpeed = SPEED_DEFAULT;
        }
        break;
    case SNAKE_EVENT_GAMEOVER:

        if (nowMode == MODE_GAME)
        {
            if (gSetting.soundOnOff)
            {
                devPlaySound(SOUND_GAMEOVER);
            }

            //ÏÈÍ£Ò»»áËÀµôµÄ×´Ì¬(TODO:GBA ÕâÀï»áÓ°ÏìËÀµôÊ±ºòµÄmaxmodÉùÒô²¥·Å£¿£¿)
            //TODO:ÕâÀï²»ÒªÓÃÑÓÊ±£¬¸Ä³É¶©ÔÄµÈ´ýÖ¡ÊýµÄ·½Ê½£¬µÈÖ¡Êýµ½´ïÖ¸¶¨Êý×ÖÖ®ºóÖ´ÐÐ»Øµ÷º¯Êý¡£
            My_delay_ms(2000);

            devEnterGameOverPage();

            lastGameScore = SnakeEventParam1;
            oldSaveDataHScore = gSetting.hiScore;
            DISP_drawGameOver(lastGameScore, oldSaveDataHScore);

            //Èç¹ûµÃ·Ö³¬¹ýÁË×î¸ß·ÖÔò¸üÐÂ×î¸ß·Ö²¢±£´æ
            if (lastGameScore > gSetting.hiScore)
            {
                gSetting.hiScore = lastGameScore;
                SD_saveSetting();
            }

            nowMode = MODE_GAMEOVER;
        } else if (nowMode == MODE_WELCOME_DEMO) {
            lastDemoScore = SnakeEventParam1;

            // Æ½¾ùµÃ·Ö TODO: ¼ÆËãÓÐÎÊÌâ£¬Ã¿´ÎÖ»¸úÉÏÒ»´Î×öÆ½¾ùÊýÊÇ²»¶ÔµÄ
            totalDemoCnt++;
            totalDemoScore += SnakeEventParam1;
            avgDemoScore = totalDemoScore / totalDemoCnt;
            
            // ×î¸ßµÃ·Ö
            if (SnakeEventParam1 > maxDemoScore)
            {
                maxDemoScore = SnakeEventParam1;
            }

            // ÏÔÊ¾×î¸ß·Ö£¬Æ½¾ù·ÖµÈÐÅÏ¢
            DISP_updateDemoGameover(maxDemoScore, avgDemoScore, lastDemoScore);

            #if !(ISDEBUG && DEBUG_DEMO_GAMEOVER_NOWAIT)
                // Í£Ò»»á
                My_delay_ms(3000);
            #endif
            
            // »Øµ½»¶Ó­Ò³
            goPageWelcome(DISP_NO);
        }
        
        break;
    case SNAKE_EVENT_EAT_APPLE:

        if (gSetting.soundOnOff)
        {
            // ²¥·ÅÉùÒô
            devPlaySound(SOUND_EAT_APPLE);
        }
        
        if (nowMode == MODE_WELCOME_DEMO)
        {
            // Ë¢ÐÂ·ÖÊý
            DISP_updateDemoScore(maxDemoScore, SnakeEventParam1);
        } else {
            // Ë¢ÐÂ·ÖÊý
            DISP_updateGameScore(maxDemoScore, SnakeEventParam1);

            // ÊÖ¶¯ÓÎÏ·²Å¸Ä±äËÙ¶È
            nowSpeed -= SPEED_INTERVAL;
            if (nowSpeed <= SPEED_MAX) 
            {
                // ÉèÖÃËÙ¶ÈÉÏÏÞ
                nowSpeed = SPEED_MAX;
            }
        }
        
        break;
    default:
        break;
    }
}

#ifdef SNAKE_AI_DEBUG
void updateBlockAI(u8 x, u8 y){
    switch (snake_getPointDataAI(x, y).subDatas.status)
    {
    case AI_STATUS_ON_THE_WAY:
        //OLED_ShowChar(x*8,y,'O',8);
        break;
    case AI_STATUS_WAY_TO_DEATH:
        //OLED_ShowChar(x*8,y,'X',8);
        break;
    case AI_STATUS_WAY_TO_APPLE:
        OLED_ShowChar(x*8,y,'.',8);
        break;
    default:
        break;
    }
}

void updateAllAI(){
    u8 x,y;
	for (x = 0; x < SNAKE_SIZE_X; ++x)
	{
		for (y = 0; y < SNAKE_SIZE_Y; ++y)
		{
            if (SNAKE_getMapData(x,y) == STS_EMPTY)
            {
                OLED_ShowChar(x*8,y,' ',8);
            }
		}
	}
}
#endif

void goPageWelcome(u8 isStartup) {
    ttWalk = 0;
    ttFlag = 0;
    welcomeWaitTime = 0;

    devEnterHomePage();

    // ÏÔÊ¾±êÌâ»­Ãæ
    LOG("---- ÏÔÊ¾±êÌâ»­Ãæ start\r\n");
    DISP_drawWelcome(isStartup);
    LOG("---- ÏÔÊ¾±êÌâ»­Ãæ end\r\n");

    nowMode = MODE_WELCOME;
}

void setDemoSpeed(u16 speed){
    nowSpeed = speed;
    lastDemoSpeed = speed;
    devSpeedChanged(speed);
}

void switchSound(){
    if (gSetting.soundOnOff)
    {
        gSetting.soundOnOff = 0;
    }else{
        gSetting.soundOnOff = 1;
    }

    DISP_drawSound(gSetting.soundOnOff);
    
    // Ã¿´ÎÇÐ»»ÉùÒô¶¼×ö±£´æ£¬·´Õý¾ÝËµ²ÁÐ´10Íò´Î£¬Ð´×ÅÍæ¶ùßÂ
    // TODO: ÀíÏëµÄ×ö·¨ÊÇÔÚÉèÖÃÒ³ÃæÀïÍ³Ò»±£´æ
    SD_saveSetting();
}

void resetDemoWall(){
    // Çå³ýµ±Ç°ÕÏ°­Îï
    SNAKE_clearWall();

    //¡¸ÑÝÊ¾Ä£Ê½¡¹ËùÔÚÇøÓòÉèÖÃÎªÕÏ°­Îï
    SNAKE_addWall(SNAKE_DEMO_TITLE_1_X,   SNAKE_DEMO_TITLE_1_Y);
    SNAKE_addWall(SNAKE_DEMO_TITLE_1_X+1, SNAKE_DEMO_TITLE_1_Y);
    SNAKE_addWall(SNAKE_DEMO_TITLE_1_X,   SNAKE_DEMO_TITLE_1_Y+1);
    SNAKE_addWall(SNAKE_DEMO_TITLE_1_X+1, SNAKE_DEMO_TITLE_1_Y+1);

    SNAKE_addWall(SNAKE_DEMO_TITLE_2_X,   SNAKE_DEMO_TITLE_2_Y);
    SNAKE_addWall(SNAKE_DEMO_TITLE_2_X+1, SNAKE_DEMO_TITLE_2_Y);
    SNAKE_addWall(SNAKE_DEMO_TITLE_2_X,   SNAKE_DEMO_TITLE_2_Y+1);
    SNAKE_addWall(SNAKE_DEMO_TITLE_2_X+1, SNAKE_DEMO_TITLE_2_Y+1);

    SNAKE_addWall(SNAKE_DEMO_TITLE_3_X,   SNAKE_DEMO_TITLE_3_Y);
    SNAKE_addWall(SNAKE_DEMO_TITLE_3_X+1, SNAKE_DEMO_TITLE_3_Y);
    SNAKE_addWall(SNAKE_DEMO_TITLE_3_X,   SNAKE_DEMO_TITLE_3_Y+1);
    SNAKE_addWall(SNAKE_DEMO_TITLE_3_X+1, SNAKE_DEMO_TITLE_3_Y+1);

    SNAKE_addWall(SNAKE_DEMO_TITLE_4_X,   SNAKE_DEMO_TITLE_4_Y);
    SNAKE_addWall(SNAKE_DEMO_TITLE_4_X+1, SNAKE_DEMO_TITLE_4_Y);
    SNAKE_addWall(SNAKE_DEMO_TITLE_4_X,   SNAKE_DEMO_TITLE_4_Y+1);
    SNAKE_addWall(SNAKE_DEMO_TITLE_4_X+1, SNAKE_DEMO_TITLE_4_Y+1);
}

void CTL_init() {

    u8 x, y, sdCheck;
    
    // ¶ÁÈ¡ÅäÖÃ£¬·ÀÖ¹Ê×´Î¶ÁÈ¡Ê§°ÜÏÈÉèÖÃºÃÄ¬ÈÏÖµ¹©Éè±¸³õ´Î±£´æ
    gSetting.hiScore         = 0;                    //×î¸ß·Ö=0
    gSetting.soundOnOff      = 1;                    //¿ªÆôÉùÒô
    gSetting.colorBackGround = COLOR_DEFAULT_BACK;   //Ä¬ÈÏ±³¾°É«
    gSetting.colorFront      = COLOR_DEFAULT_FRONT;  //Ä¬ÈÏÎÄ×ÖÉ«
    gSetting.colorSnake      = COLOR_DEFAULT_SNAKE;  //Ä¬ÈÏÉßµÄÑÕÉ«
    gSetting.colorApple      = COLOR_DEFAULT_APPLE;  //Ä¬ÈÏÊ³ÎïÑÕÉ«
    gSetting.colorFrame      = COLOR_DEFAULT_FRAME;  //Ä¬ÈÏ¿ò¼ÜÑÕÉ«
    SD_loadSetting();

    // ÉÏµçÄ¬ÈÏdemoËÙ¶È£¨Õâ¸ö²»×ö±£´æ£¬Ã»É¶ÒâÒå£©
    lastDemoSpeed = SPEED_DEMO_H;

    DISP_setBackColor(gSetting.colorBackGround);
    DISP_setForeColor(gSetting.colorFront);
    DISP_setSnakeColor(gSetting.colorSnake);
    DISP_setAppleColor(gSetting.colorApple);
    DISP_setFrameColor(gSetting.colorFrame);

    DISP_init();
    devSndInit();
    KEY_init(eventKey1, eventKey2, eventKey3, eventKey4, eventKey5, eventKey6, eventKey7, eventKey8, eventKey9, powerKey);
    SNAKE_init(eventSnake);
    
    #if ISDEBUG
        LOG("========== MAP DATA (before load Wallmap) START ==========\r\n");
        for (y = 0; y < SNAKE_SIZE_Y; ++y)
        {
            for (x = 0; x < SNAKE_SIZE_X; ++x)
            {   // x= 0,y= 1,idx= 2,offset= 0
                sprintf(buff, 
                    "x=%b2d,y=%b2d,idx=%b2d,offset=%b2d,mapSts=%b2d,isWall=%b2d\r\n", 
                    x, y,
                    (SNAKE_SIZE_X/8)*y+(x/8), 
                    x%8,
                    SNAKE_getMapData(x, y),
                    ((WALLMAP_1[(SNAKE_SIZE_X/8)*y+(x/8)]<<(x%8)) & 0x80));
                LOG(buff);
            }
        }
        LOG("========== MAP DATA (before load Wallmap) END ==========\r\n");
    #endif

    LOG("---- SNAKE_loadWall start\r\n");
    // SNAKE_loadWall(WALLMAP_1);
    LOG("---- SNAKE_loadWall end\r\n");

    #if ISDEBUG
        LOG("========== MAP DATA (After load Wallmap) START ==========\r\n");
        for (y = 0; y < SNAKE_SIZE_Y; ++y)
        {
            for (x = 0; x < SNAKE_SIZE_X; ++x)
            {
                sprintf(buff, 
                    "x=%b2d,y=%b2d,sts=%b2d\r\n", 
                    x, y, SNAKE_getMapData(x, y));
                //sprintf(buff, "x=%b2d,y=%b2d,idx=%b2d,offset=%b2d\r\n", x, y, 5, x%8);
                LOG(buff);
                // if(map[x][y] == STS_EMPTY && ((wallMap[(SNAKE_SIZE_X/8)*y+(x/8)]<<(x%8)) & 0x80) ) {
                //     updateMap(x, y, STS_WALL);
                // }
            }
        }
        LOG("========== MAP DATA (After load Wallmap) END ==========\r\n");
	#endif

    goPageWelcome(DISP_YES);

    LOG("-- CTL_init end\r\n");
}

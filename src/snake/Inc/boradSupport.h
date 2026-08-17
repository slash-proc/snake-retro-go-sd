// 板级支持头文件

#ifndef __EMB_SNAKE_BORAD_SUPPORT_H_
#define __EMB_SNAKE_BORAD_SUPPORT_H_

#include "snake_common.h"

#define XUEDIAN

#ifdef XUEDIAN
    #define LED_R_ON()              ;
    #define LED_Y_ON()              ;
    #define LED_G_ON()              ;
    #define LED_R_OFF()             ;
    #define LED_Y_OFF()             ;
    #define LED_G_OFF()             ;
    #define LED_ALL_ON()            LED_R_ON();LED_Y_ON();LED_G_ON()
    #define LED_ALL_OFF()           LED_R_OFF();LED_Y_OFF();LED_G_OFF()
#endif

#endif

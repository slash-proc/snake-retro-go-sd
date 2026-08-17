#ifndef __EMB_SNAKE_COMMON_H_
#define __EMB_SNAKE_COMMON_H_

#include <stdio.h>

#include "binary.h"
#include "font.h"
#include "color.h"
#include "myMathUtil.h"
#include "def_types.h"
#include "snake_platform.h"

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

void My_delay_ms(u16 n);
u8 My_strlen(char *x);

/* True RNG helper used by embSnake when USE_EXTERN_REAL_RAND_FUNC is set. */
u16 My_real_rand(void);

#define ISDEBUG 0
#define DEBUG_SKIP_HOME 0
#define DEBUG_DEMO_SPEED_L 200
#define DEBUG_DEMO_SPEED_M 10000
#define DEBUG_DEMO_SPEED_H 50
#define DEBUG_DEMO_SPEED_S 1
#define DEBUG_DEMO_APPLE 50
#define DEBUG_DEMO_GAMEOVER_NOWAIT 0

/* Speed tiers ?? score climbs ?? snake moves faster. */
#if ISDEBUG
#define SPEED_DEFAULT 500
#define SPEED_MAX 100
#define SPEED_INTERVAL 50
#define SPEED_DEMO_L DEBUG_DEMO_SPEED_L
#define SPEED_DEMO_M DEBUG_DEMO_SPEED_M
#define SPEED_DEMO_H DEBUG_DEMO_SPEED_H
#define SPEED_DEMO_S DEBUG_DEMO_SPEED_S
#else
#define SPEED_DEFAULT 30
#define SPEED_MAX 6
#define SPEED_INTERVAL 2
#define SPEED_DEMO_L 30
#define SPEED_DEMO_M 15
#define SPEED_DEMO_H 9
#define SPEED_DEMO_S 6
#endif

#define PRINT_LOG 0
#if PRINT_LOG
#define LOG(str) printf("%s", (str))
#else
#define LOG(str)
#endif

#endif

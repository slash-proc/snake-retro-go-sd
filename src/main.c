/*
 * Snake — Retro-Go SD GWHB homebrew
 *
 * Port of https://github.com/slipperstree/game-and-watch-snake
 * (MANDA / slipperstree) onto the freestanding Retro-Go SD ABI.
 *
 * Controls (same as upstream, adapted for Retro-Go):
 *   Home: any button starts; wait for demo; GAME/TIME switch modes
 *   Game: D-pad steer, B toggle sound, GAME restart / mode
 *   Game Over: any button → home; TIME → info page
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "common.h"
#include "gw_lcd.h"
#include "gw_audio.h"
#include "rom_manager.h"
#include "odroid_system.h"
#include "odroid_overlay.h"
#include "odroid_settings.h"
#include "gw_malloc.h"

#include "control.h"
#include "snake_platform.h"
#include "myMathUtil.h"

#ifndef HOST_BUILD
#include "gw_core_bridge.h"
#include "gw_core_i18n.h"
#else
#include "host_compat.h"
#include "gw_core_i18n.h"
#endif

#define APP_ID       14 /* APPID_HOMEBREW */
#define FPS          60
#define SAMPLE_RATE  16000
#define AUDIO_LENGTH (SAMPLE_RATE / FPS)

static odroid_gamepad_state_t pad;

/* --- settings persistence (replaces unused EEPROM in upstream) ------------ */

typedef struct {
    uint16_t hiScore;
    uint8_t soundOnOff;
    uint16_t colorBackGround;
    uint16_t colorFront;
    uint16_t colorSnake;
    uint16_t colorApple;
    uint16_t colorFrame;
} SaveData_Struct;

void snake_dev_load_setting(SaveData_Struct *setting)
{
    int32_t magic = odroid_settings_app_int32_get("sn_magic", 0);
    if (magic != 0x534E4B45) /* 'SNKE' */
        return;

    setting->hiScore = (uint16_t)odroid_settings_app_int32_get("sn_hs", setting->hiScore);
    setting->soundOnOff = (uint8_t)odroid_settings_app_int32_get("sn_snd", setting->soundOnOff);
}

void snake_dev_save_setting(SaveData_Struct *setting)
{
    odroid_settings_app_int32_set("sn_magic", 0x534E4B45);
    odroid_settings_app_int32_set("sn_hs", (int32_t)setting->hiScore);
    odroid_settings_app_int32_set("sn_snd", setting->soundOnOff);
}

/* --- system callbacks ----------------------------------------------------- */

static bool LoadState(const char *savePathName)
{
    (void)savePathName;
    return false;
}

static bool SaveState(const char *savePathName)
{
    (void)savePathName;
    return false;
}

static void present(void)
{
    uint16_t *dst = lcd_get_active_buffer();
    memcpy(dst, snake_framebuffer(), WIDTH * HEIGHT * sizeof(uint16_t));
    common_ingame_overlay();
}

static void present_for_delay(void)
{
    uint16_t *dst = lcd_get_active_buffer();
    memcpy(dst, snake_framebuffer(), WIDTH * HEIGHT * sizeof(uint16_t));
    common_ingame_overlay();
    lcd_swap();
}

static void *Screenshot(void)
{
    lcd_wait_for_vblank();
    present();
    return lcd_get_active_buffer();
}

static void Shutdown(void)
{
}

static void SleepWake(void)
{
    odroid_audio_init(SAMPLE_RATE);
    audio_clear_buffers();
    audio_start_playing(AUDIO_LENGTH);
}

static void SramSave(void)
{
}

static uint32_t map_buttons(const odroid_gamepad_state_t *joy)
{
    uint32_t b = 0;

    if (joy->values[ODROID_INPUT_LEFT])
        b |= B_Left;
    if (joy->values[ODROID_INPUT_UP])
        b |= B_Up;
    if (joy->values[ODROID_INPUT_RIGHT])
        b |= B_Right;
    if (joy->values[ODROID_INPUT_DOWN])
        b |= B_Down;
    if (joy->values[ODROID_INPUT_A])
        b |= B_A;
    if (joy->values[ODROID_INPUT_B])
        b |= B_B;
    if (joy->values[ODROID_INPUT_X] || joy->values[ODROID_INPUT_START])
        b |= B_GAME;
    if (joy->values[ODROID_INPUT_Y] || joy->values[ODROID_INPUT_SELECT])
        b |= B_TIME;
    /* Do not forward PAUSE/POWER — firmware owns the system menu / sleep. */
    return b;
}

static void submit_audio(void)
{
    int16_t *buf;
    uint16_t len;
    uint16_t i;
    int32_t vol;

    if (common_emu_sound_loop_is_muted())
        return;

    buf = audio_get_active_buffer();
    len = audio_get_buffer_length();
    if (!buf || !len)
        return;

    vol = common_emu_sound_get_volume();
    for (i = 0; i < len; i++) {
        int16_t s = 0;
        snake_sound_pop_sample(&s);
        buf[i] = (int16_t)((s * vol) / 255);
    }
}

void app_main(uint8_t load_state, uint8_t start_paused, int8_t save_slot)
{
    odroid_gamepad_state_t joystick;
    odroid_dialog_choice_t options[1];

    (void)load_state;
    (void)save_slot;

    gw_core_bridge_init();
    ram_start = (uint32_t)(uintptr_t)&__CORE_BSS_END__;
    memset(&pad, 0, sizeof(pad));
    snake_sound_set_sample_rate(SAMPLE_RATE);
    snake_platform_present_fn = present_for_delay;
    snake_platform_wdog_fn = wdog_refresh;

    if (start_paused) {
        common_emu_state.pause_after_frames = 2;
        odroid_audio_mute(true);
    } else {
        common_emu_state.pause_after_frames = 0;
    }
    common_emu_state.frame_time_10us = (uint16_t)(100000 / FPS + 0.5f);
    lcd_set_refresh_rate(FPS);

    odroid_system_init(APP_ID, SAMPLE_RATE);
    odroid_system_emu_init(&LoadState, &SaveState, &Screenshot,
                           &Shutdown, &SleepWake, &SramSave, NULL);

    options[0] = (odroid_dialog_choice_t)ODROID_DIALOG_CHOICE_LAST;

    audio_start_playing(AUDIO_LENGTH);
    lcd_clear_buffers();

    my_seed_rand((s32)((uintptr_t)&joystick ^ (uintptr_t)&pad ^ 0x534E4B45));

    CTL_init();

    while (1) {
        wdog_refresh();

        bool draw_frame = common_emu_frame_loop();

        odroid_input_read_gamepad(&joystick);
        common_emu_input_loop(&joystick, options, &present);
        common_emu_input_loop_handle_turbo(&joystick);

        pad = joystick;
        snake_platform_set_buttons(map_buttons(&pad));

        CTL_run();

        if (draw_frame) {
            present();
            lcd_swap();
        }

        submit_audio();
        common_emu_sound_sync(false);
    }
}

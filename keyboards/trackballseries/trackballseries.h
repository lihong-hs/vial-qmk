/*
 * Copyright 2021 Quentin LEBASTARD <qlebastard@gmail.com>
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "quantum.h"

#ifdef POINTING_DEVICE_ENABLE
#    ifndef NO_KEYBALL_KEYCODES
enum keyball_keycodes {
    POINTER_DEFAULT_DPI_FORWARD = QK_KB_0,
    POINTER_DEFAULT_DPI_REVERSE,
    POINTER_ACCEL_SENSITIVITY_FORWARD,
    POINTER_ACCEL_SENSITIVITY_REVERSE,
    POINTER_SNIPING_DPI_FORWARD,
    POINTER_SNIPING_DPI_REVERSE,
    SNIPING_MODE,
    SNIPING_MODE_TOGGLE,
    DRAGSCROLL_MODE,
    DRAGSCROLL_MODE_TOGGLE,
    POINTER_DRAGSCROLL_DPI_FORWARD,
    POINTER_DRAGSCROLL_DPI_REVERSE,
    OLED_TOGGLE,
    OLED_INFO_TOGGLE,
    BACKLIGHT_TOGGLE,
    PRINT_CONFIG,
    DB_TOGG_CUSTOM,
};

#define DPI_MOD POINTER_DEFAULT_DPI_FORWARD
#define DPI_RMOD POINTER_DEFAULT_DPI_REVERSE
#define ACCEL_MOD POINTER_ACCEL_SENSITIVITY_FORWARD
#define ACCEL_RMOD POINTER_ACCEL_SENSITIVITY_REVERSE
#define S_D_MOD POINTER_SNIPING_DPI_FORWARD
#define S_D_RMOD POINTER_SNIPING_DPI_REVERSE
#define SNIPING SNIPING_MODE
#define SNP_TOG SNIPING_MODE_TOGGLE
#define DRGSCRL DRAGSCROLL_MODE
#define DRG_TOG DRAGSCROLL_MODE_TOGGLE
#define DRG_MOD POINTER_DRAGSCROLL_DPI_FORWARD
#define DRG_RMOD POINTER_DRAGSCROLL_DPI_REVERSE
#define OLED_TOG OLED_TOGGLE
#define OLED_INFO_TOG OLED_INFO_TOGGLE
#define BACKLIGHT_TOG BACKLIGHT_TOGGLE
#define PRINT_CFG PRINT_CONFIG 
#define DB_TOGG_CUSTOM DB_TOGG_CUSTOM 
#endif // NO_KEYBALL_KEYCODES


uint16_t keyball_get_pointer_default_dpi(void);
uint16_t keyball_get_pointer_sniping_dpi(void);
uint16_t keyball_get_pointer_dragscroll_dpi(void);
float keyball_get_pointer_accel_factor(void);

bool keyball_get_pointer_sniping_enabled(void);
bool keyball_get_pointer_dragscroll_enabled(void);

void oled_trackball_config_info(void);

#endif // POINTING_DEVICE_ENABLE

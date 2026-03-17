
#include "trackballseries.h"
#include "transactions.h"
#include <string.h>
#include "pointing_device.h"
#include "timer.h"
#include <stdlib.h>

#ifdef CONSOLE_ENABLE
#    include "print.h"
#endif // CONSOLE_ENABLE

#ifdef POINTING_DEVICE_ENABLE
#    ifndef CHARYBDIS_MINIMUM_DEFAULT_DPI
#        define CHARYBDIS_MINIMUM_DEFAULT_DPI 300
#    endif // CHARYBDIS_MINIMUM_DEFAULT_DPI

#    ifndef CHARYBDIS_BASE_ACCEL_SENSITIVITY
#        define CHARYBDIS_BASE_ACCEL_SENSITIVITY 0.00f
#    endif

#    ifndef CHARYBDIS_ACCEL_SENSITIVITY_STEP_SIZE
#        define CHARYBDIS_ACCEL_SENSITIVITY_STEP_SIZE 0.05f
#    endif

#    ifndef CHARYBDIS_DEFAULT_DPI_CONFIG_STEP
#        define CHARYBDIS_DEFAULT_DPI_CONFIG_STEP 50
#    endif // CHARYBDIS_DEFAULT_DPI_CONFIG_STEP

#    ifndef CHARYBDIS_MINIMUM_SNIPING_DPI
#        define CHARYBDIS_MINIMUM_SNIPING_DPI 200
#    endif // CHARYBDIS_MINIMUM_SNIPER_MODE_DPI

#    ifndef CHARYBDIS_SNIPING_DPI_CONFIG_STEP
#        define CHARYBDIS_SNIPING_DPI_CONFIG_STEP 50
#    endif // CHARYBDIS_SNIPING_DPI_CONFIG_STEP


typedef union {
    uint32_t raw;
    struct {
        uint16_t pointer_dragscroll_dpi : 9; // 0-511
        uint8_t pointer_default_dpi : 4; // 16 steps available.
        uint8_t pointer_accel_sensitivty : 5;  // 32 steps available.
        uint8_t pointer_sniping_dpi : 2; // 4 steps available.
        bool    is_dragscroll_enabled : 1;
        bool    is_sniping_enabled : 1;
    } __attribute__((packed));
} charybdis_config_t;

static charybdis_config_t g_charybdis_config = {0};
//static charybdis_config_t g_charybdis_config;


// Fixed DPI for drag-scroll.
#    ifndef CHARYBDIS_DRAGSCROLL_DPI
#        define CHARYBDIS_DRAGSCROLL_DPI g_charybdis_config.pointer_dragscroll_dpi
#    endif // CHARYBDIS_DRAGSCROLL_DPI
//100 6
#    ifndef CHARYBDIS_DRAGSCROLL_BUFFER_SIZE
#        define CHARYBDIS_DRAGSCROLL_BUFFER_SIZE 6
#    endif // !CHARYBDIS_DRAGSCROLL_BUFFER_SIZE



static void read_charybdis_config_from_eeprom(charybdis_config_t* config) {
    config->raw                   = eeconfig_read_kb();
//    config->raw                   = eeconfig_read_kb() & 0xff;
//    config->pointer_dragscroll_dpi = 100;
    config->is_dragscroll_enabled = false;
    config->is_sniping_enabled    = false;
}


static void write_charybdis_config_to_eeprom(charybdis_config_t* config) {
    eeconfig_update_kb(config->raw);
}

/** \brief Return the current value of the pointer's default DPI. */
static uint16_t get_pointer_default_dpi(charybdis_config_t* config) {
    return (uint16_t)config->pointer_default_dpi * CHARYBDIS_DEFAULT_DPI_CONFIG_STEP + CHARYBDIS_MINIMUM_DEFAULT_DPI;
}

/** \brief Return the current value of the pointer's sniper-mode DPI. */
static uint16_t get_pointer_sniping_dpi(charybdis_config_t* config) {
    return (uint16_t)config->pointer_sniping_dpi * CHARYBDIS_SNIPING_DPI_CONFIG_STEP + CHARYBDIS_MINIMUM_SNIPING_DPI;
}

/** \brief Return the current acceleration factor as a float. */
static float get_pointer_accel_sensitivity(charybdis_config_t config) {
    return (float)config.pointer_accel_sensitivty * CHARYBDIS_ACCEL_SENSITIVITY_STEP_SIZE + CHARYBDIS_BASE_ACCEL_SENSITIVITY;
}

/** \brief Step the acceleration factor index up or down. */
static void step_pointer_accel_factor(charybdis_config_t* config, bool forward) {
    // Max value for a 5-bit field is 31
    #define ACCEL_FACTOR_INDEX_MAX 31

    if (forward) {
        if (config->pointer_accel_sensitivty < ACCEL_FACTOR_INDEX_MAX) {
            config->pointer_accel_sensitivty++;
        }
    } else {
        if (config->pointer_accel_sensitivty > 0) {
            config->pointer_accel_sensitivty--;
        }
    }
}

/** \brief Set the appropriate DPI for the input config. */
static void maybe_update_pointing_device_cpi(charybdis_config_t* config) {
    if (config->is_dragscroll_enabled) {
//        pointing_device_set_cpi(CHARYBDIS_DRAGSCROLL_DPI);
        pointing_device_set_cpi(g_charybdis_config.pointer_dragscroll_dpi);
    } else if (config->is_sniping_enabled) {
        pointing_device_set_cpi(get_pointer_sniping_dpi(config));
    } else {
        pointing_device_set_cpi(get_pointer_default_dpi(config));
    }
}

/**
 * The increment/decrement steps are equal to CHARYBDIS_DEFAULT_DPI_CONFIG_STEP.
 */
static void step_pointer_default_dpi(charybdis_config_t* config, bool forward) {
    config->pointer_default_dpi += forward ? 1 : -1;
    maybe_update_pointing_device_cpi(config);
}
/**
 * The increment/decrement steps are equal to CHARYBDIS_SNIPING_DPI_CONFIG_STEP.
 */
static void step_pointer_sniping_dpi(charybdis_config_t* config, bool forward) {
    config->pointer_sniping_dpi += forward ? 1 : -1;
    maybe_update_pointing_device_cpi(config);
}



uint16_t charybdis_get_pointer_default_dpi(void) {
    return get_pointer_default_dpi(&g_charybdis_config);
}

uint16_t charybdis_get_pointer_sniping_dpi(void) {
    return get_pointer_sniping_dpi(&g_charybdis_config);
}

void charybdis_cycle_pointer_default_dpi_noeeprom(bool forward) {
    step_pointer_default_dpi(&g_charybdis_config, forward);
}


void charybdis_cycle_pointer_default_dpi(bool forward) {
    step_pointer_default_dpi(&g_charybdis_config, forward);
    write_charybdis_config_to_eeprom(&g_charybdis_config);
}

void charybdis_cycle_pointer_sniping_dpi(bool forward) {
    step_pointer_sniping_dpi(&g_charybdis_config, forward);
    write_charybdis_config_to_eeprom(&g_charybdis_config);
}

bool charybdis_get_pointer_sniping_enabled(void) {
    return g_charybdis_config.is_sniping_enabled;
}


void charybdis_set_pointer_sniping_enabled(bool enable) {
    g_charybdis_config.is_sniping_enabled = enable;
    maybe_update_pointing_device_cpi(&g_charybdis_config);
}

bool charybdis_get_pointer_dragscroll_enabled(void) {
    return g_charybdis_config.is_dragscroll_enabled;
}

void charybdis_set_pointer_dragscroll_enabled(bool enable) {
    g_charybdis_config.is_dragscroll_enabled = enable;
    maybe_update_pointing_device_cpi(&g_charybdis_config);
}

void charybdis_cycle_pointer_sniping_dpi_noeeprom(bool forward) {
    step_pointer_sniping_dpi(&g_charybdis_config, forward);
}


// --- Acceleration Configuration ---
// The minimum speed (counts/ms) required before acceleration kicks in.
// Prevents jitter when making tiny, precise movements.
#define VELOCITY_THRESHOLD 1.0f
// The maximum HID report value allowed. Prevents cursor from flying off-screen.
#define MAX_REPORT_VALUE 127

// --- State Tracking ---
static uint32_t last_movement_timer = 0;

// --- Acceleration Logic ---
/**
 * \brief Calculates the speed multiplier based on how fast the trackball is moved.
 *
 * \param velocity      The speed of movement (counts per millisecond).
 * \param sensitivity   The configuration slope (how aggressively speed increases).
 * \return              A multiplier (1.0 = normal speed, 2.0 = double speed).
 */
static float calculate_velocity_multiplier(float velocity, float sensitivity) {
    // If moving slower than threshold, no acceleration (1.0x speed)
    if (velocity <= VELOCITY_THRESHOLD) {
        return 1.0f;
    }

    // Calculate how much faster than the threshold we are moving
    float excess_velocity = velocity - VELOCITY_THRESHOLD;

    // Apply the sensitivity curve
    // Formula: Base Speed + (Excess Speed * Sensitivity)
    float multiplier = 1.0f + (excess_velocity * sensitivity);

    return multiplier;
}

/**
 * \brief Applies acceleration to a single axis based on time elapsed.
 */
static int16_t apply_velocity_accel(int16_t movement_delta, uint16_t time_elapsed_ms, float sensitivity) {
    // Safety: Avoid division by zero if timer hasn't updated
    if (time_elapsed_ms == 0 || movement_delta == 0) {
        return movement_delta;
    }

    // 1. Calculate Velocity (Distance / Time)
    // We use abs() because direction doesn't matter for speed calculation
    float velocity = (float)abs(movement_delta) / (float)time_elapsed_ms;

    // 2. Get the Speed Multiplier based on Velocity
    float multiplier = calculate_velocity_multiplier(velocity, sensitivity);

    // 3. Apply Multiplier to the original movement
    float accelerated_delta = (float)movement_delta * multiplier;

    // 4. Clamp value to prevent HID report overflow (max 127 per report)
    if (accelerated_delta > MAX_REPORT_VALUE) {
        accelerated_delta = MAX_REPORT_VALUE;
    } else if (accelerated_delta < -MAX_REPORT_VALUE) {
        accelerated_delta = -MAX_REPORT_VALUE;
    }

    return (int16_t)accelerated_delta;
}

/**
 * \brief Augment the pointing device behavior.
 *
 * Implement drag-scroll.
 */
static void pointing_device_task_charybdis(report_mouse_t* mouse_report) {
    static int16_t scroll_buffer_x = 0;
    static int16_t scroll_buffer_y = 0;
    if (g_charybdis_config.is_dragscroll_enabled) {
    #ifdef CHARYBDIS_DRAGSCROLL_REVERSE_X
        scroll_buffer_x -= mouse_report->x;
    #else
        scroll_buffer_x += mouse_report->x;
    #endif // CHARYBDIS_DRAGSCROLL_REVERSE_X

    #ifdef CHARYBDIS_DRAGSCROLL_REVERSE_Y
        scroll_buffer_y -= mouse_report->y;
    #else
        scroll_buffer_y += mouse_report->y;
    #endif // CHARYBDIS_DRAGSCROLL_REVERSE_Y
        mouse_report->x = 0;
        mouse_report->y = 0;
        if (abs(scroll_buffer_x) > CHARYBDIS_DRAGSCROLL_BUFFER_SIZE) {
            mouse_report->h = scroll_buffer_x > 0 ? 1 : -1;
            scroll_buffer_x = 0;
        }
        if (abs(scroll_buffer_y) > CHARYBDIS_DRAGSCROLL_BUFFER_SIZE) {
            mouse_report->v = scroll_buffer_y > 0 ? 1 : -1;
            scroll_buffer_y = 0;
        }
    }
}

//
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    // Calculate time since last movement
    uint32_t current_timer = timer_elapsed32(last_movement_timer);

    // Safety: If keyboard was idle for >1 second, reset timer to avoid huge velocity spikes
    // We assume a standard USB poll rate ~16ms (60Hz) as a baseline minimum
    if (current_timer > 1000) {
        current_timer = 16;
    }

    // Only process acceleration if there is actual movement
    if (mouse_report.x != 0 || mouse_report.y != 0) {
        // Update timer for next cycle
        last_movement_timer = timer_read32();

        // Get sensitivity from config (Index 0-31 * Step Size 0.05)
        float sensitivity = get_pointer_accel_sensitivity(g_charybdis_config);

        // Apply acceleration to both X and Y axes
        mouse_report.x = apply_velocity_accel(mouse_report.x, current_timer, sensitivity);
        mouse_report.y = apply_velocity_accel(mouse_report.y, current_timer, sensitivity);
    }

    return mouse_report;
}

report_mouse_t pointing_device_task_kb(report_mouse_t mouse_report) {
    if (is_keyboard_master()) {
        pointing_device_task_charybdis(&mouse_report);
        mouse_report = pointing_device_task_user(mouse_report);
    }
    return mouse_report;
}

#    if defined(POINTING_DEVICE_ENABLE) && !defined(NO_CHARYBDIS_KEYCODES)
/** \brief Whether SHIFT mod is enabled. */
static bool has_shift_mod(void) {
#        ifdef NO_ACTION_ONESHOT
    return mod_config(get_mods()) & MOD_MASK_SHIFT;
#        else
    return mod_config(get_mods() | get_oneshot_mods()) & MOD_MASK_SHIFT;
#        endif // NO_ACTION_ONESHOT
}
#    endif // POINTING_DEVICE_ENABLE && !NO_CHARYBDIS_KEYCODES

/**
 * \brief Outputs the Charybdis configuration to console.
 *
 */
char* get_trackball_config_string(void) {
    static char buffer[256];  // Static buffer to hold the combined string
    
    float accel_sensitivity = get_pointer_accel_sensitivity(g_charybdis_config);
    uint8_t whole = (uint8_t)accel_sensitivity;
    uint8_t fractional = (uint8_t)((accel_sensitivity - whole) * 100);
    char accel_str[8];
    snprintf(accel_str, sizeof(accel_str), "%d.%02d", whole, fractional);
    
    snprintf(buffer, sizeof(buffer),
        "=== Trackball Config ===\n"
        "Default DPI : %d\n"
        "Accel Sensitivity: %s\n"
        "Sniping DPI : %d [%s]\n"
        "Drag-scroll DPI : %d [%s]\n"
        "========================\n",
        charybdis_get_pointer_default_dpi(),
        accel_str,
        charybdis_get_pointer_sniping_dpi(),
        g_charybdis_config.is_sniping_enabled ? "Active" : "Inactive",
        g_charybdis_config.pointer_dragscroll_dpi,
        g_charybdis_config.is_dragscroll_enabled ? "Active" : "Inactive"
    );
    
    return buffer;
}

static void print_charybdis_config_to_console(charybdis_config_t *config) {
#    ifdef CONSOLE_ENABLE
    char *config_string = get_trackball_config_string();
    uprintf("%s", config_string);
#    endif // CONSOLE_ENABLE
}

bool process_record_kb(uint16_t keycode, keyrecord_t* record) {
#    ifdef POINTING_DEVICE_ENABLE
#ifndef NO_CHARYBDIS_KEYCODES
    switch (keycode) {
        case POINTER_DEFAULT_DPI_FORWARD:
            if (record->event.pressed) {
                // Step backward if shifted, forward otherwise.
                charybdis_cycle_pointer_default_dpi(/* forward= */ !has_shift_mod());
                print_charybdis_config_to_console(&g_charybdis_config);
            }
            break;
        case POINTER_DEFAULT_DPI_REVERSE:
            if (record->event.pressed) {
                // Step forward if shifted, backward otherwise.
                charybdis_cycle_pointer_default_dpi(/* forward= */ has_shift_mod());
                print_charybdis_config_to_console(&g_charybdis_config);
            }
            break;
        case POINTER_ACCEL_SENSITIVITY_FORWARD:
            if (record->event.pressed) {
                // Shift reverses direction (consistent with DPI keys)
                step_pointer_accel_factor(&g_charybdis_config, !has_shift_mod());
                write_charybdis_config_to_eeprom(&g_charybdis_config);
                print_charybdis_config_to_console(&g_charybdis_config);
            }
            break;
        case POINTER_ACCEL_SENSITIVITY_REVERSE:
            if (record->event.pressed) {
                step_pointer_accel_factor(&g_charybdis_config, has_shift_mod());
                write_charybdis_config_to_eeprom(&g_charybdis_config);
                print_charybdis_config_to_console(&g_charybdis_config);
            }
            break;
        case POINTER_SNIPING_DPI_FORWARD:
            if (record->event.pressed) {
                // Step backward if shifted, forward otherwise.
                charybdis_cycle_pointer_sniping_dpi(/* forward= */ !has_shift_mod());
                print_charybdis_config_to_console(&g_charybdis_config);
            }
            break;
        case POINTER_SNIPING_DPI_REVERSE:
            if (record->event.pressed) {
                // Step forward if shifted, backward otherwise.
                charybdis_cycle_pointer_sniping_dpi(/* forward= */ has_shift_mod());
                print_charybdis_config_to_console(&g_charybdis_config);
            }
            break;
        case SNIPING_MODE:
            charybdis_set_pointer_sniping_enabled(record->event.pressed);
            break;
        case SNIPING_MODE_TOGGLE:
            if (record->event.pressed) {
                charybdis_set_pointer_sniping_enabled(!charybdis_get_pointer_sniping_enabled());
                print_charybdis_config_to_console(&g_charybdis_config);
            }
            break;


        case POINTER_DRAGSCROLL_DPI_FORWARD:
            if (record->event.pressed) {
                uint16_t new_value = g_charybdis_config.pointer_dragscroll_dpi + 100;
                if (new_value > 511) { // 确保不超过位域的最大值511
                    g_charybdis_config.pointer_dragscroll_dpi = 100; // 超过511时重置为100
                } else if (new_value > 500) {
                    g_charybdis_config.pointer_dragscroll_dpi = 100; // 达到或超过500时也重置为100
                } else {
                    g_charybdis_config.pointer_dragscroll_dpi = new_value; // 正常增加
                }
                maybe_update_pointing_device_cpi(&g_charybdis_config);
                write_charybdis_config_to_eeprom(&g_charybdis_config);
                print_charybdis_config_to_console(&g_charybdis_config);
            }
            break;
        case POINTER_DRAGSCROLL_DPI_REVERSE:
            if (record->event.pressed) {
               uint16_t new_value = g_charybdis_config.pointer_dragscroll_dpi - 100;
                if (new_value > 511) { // 确保不超过位域的最大值511
                    g_charybdis_config.pointer_dragscroll_dpi = 500; // 超过511时重置为100
                } else if (new_value > 500) {
                    g_charybdis_config.pointer_dragscroll_dpi = 500; // 达到或超过500时也重置为100
                } else {
                    g_charybdis_config.pointer_dragscroll_dpi = new_value; // 正常增加
                }
                maybe_update_pointing_device_cpi(&g_charybdis_config);
                write_charybdis_config_to_eeprom(&g_charybdis_config);
                print_charybdis_config_to_console(&g_charybdis_config);
            }
            break;
        case DRAGSCROLL_MODE:
            charybdis_set_pointer_dragscroll_enabled(record->event.pressed);
            break;
        case DRAGSCROLL_MODE_TOGGLE:
            if (record->event.pressed) {
                charybdis_set_pointer_dragscroll_enabled(!charybdis_get_pointer_dragscroll_enabled());
                print_charybdis_config_to_console(&g_charybdis_config);
            }
            break;
        case PRINT_CONFIG:
            if (record->event.pressed) {
                char *config_string = get_trackball_config_string();
                SEND_STRING(config_string);
            }
            break;
    }
#        endif // !NO_CHARYBDIS_KEYCODES
#    endif     // POINTING_DEVICE_ENABLE
    return true;
}

void eeconfig_init_kb(void) {
    g_charybdis_config.raw = 0;

    // Default accel factor: index 0 = 0.0f (flat)
    // g_charybdis_config.pointer_accel_sensitivty = 0;

    write_charybdis_config_to_eeprom(&g_charybdis_config);
    maybe_update_pointing_device_cpi(&g_charybdis_config);
    eeconfig_init_user();
}

void matrix_init_kb(void) {
    read_charybdis_config_from_eeprom(&g_charybdis_config);
    matrix_init_user();
}

#ifdef CHARYBDIS_CONFIG_SYNC
void charybdis_config_sync_handler(uint8_t initiator2target_buffer_size, const void* initiator2target_buffer, uint8_t target2initiator_buffer_size, void* target2initiator_buffer) {
    if (initiator2target_buffer_size == sizeof(g_charybdis_config)) {
        memcpy(&g_charybdis_config, initiator2target_buffer, sizeof(g_charybdis_config));
    }
}
#    endif

void keyboard_post_init_kb(void) {
    debug_enable=true;
    maybe_update_pointing_device_cpi(&g_charybdis_config);
    #ifdef CHARYBDIS_CONFIG_SYNC
        transaction_register_rpc(RPC_ID_KB_CONFIG_SYNC, charybdis_config_sync_handler);
    #endif
    keyboard_post_init_user();
}

#    ifdef CHARYBDIS_CONFIG_SYNC
void housekeeping_task_kb(void) {
    if (is_keyboard_master()) {
        // Keep track of the last state, so that we can tell if we need to propagate to slave.
        static charybdis_config_t last_charybdis_config = {0};
        static uint32_t           last_sync             = 0;
        bool                      needs_sync            = false;

        // Check if the state values are different.
        if (memcmp(&g_charybdis_config, &last_charybdis_config, sizeof(g_charybdis_config))) {
            needs_sync = true;
            memcpy(&last_charybdis_config, &g_charybdis_config, sizeof(g_charybdis_config));
        }
        // Send to slave every 500ms regardless of state change.
        if (timer_elapsed32(last_sync) > 500) {
            needs_sync = true;
        }

        // Perform the sync if requested.
        if (needs_sync) {
            if (transaction_rpc_send(RPC_ID_KB_CONFIG_SYNC, sizeof(g_charybdis_config), &g_charybdis_config)) {
                last_sync = timer_read32();
            }
        }
    }
    // No need to invoke the user-specific callback, as it's been called
    // already.
}
#    endif // CHARYBDIS_CONFIG_SYNC
#endif     // POINTING_DEVICE_ENABLE

#if defined(KEYBOARD_bastardkb_charybdis_3x5_blackpill) || defined(KEYBOARD_bastardkb_charybdis_4x6_blackpill)
void keyboard_pre_init_kb(void) {
    gpio_set_pin_input_high(A0);
    keyboard_pre_init_user();
}
void matrix_scan_kb(void) {
    if (!gpio_read_pin(A0)) {
        reset_keyboard();
    }
    matrix_scan_user();
}
#endif // KEYBOARD_bastardkb_charybdis_3x5_blackpill || KEYBOARD_bastardkb_charybdis_4x6_blackpill

bool shutdown_kb(bool jump_to_bootloader) {
    if (!shutdown_user(jump_to_bootloader)) {
        return false;
    }

#ifdef RGB_MATRIX_ENABLE
    void rgb_matrix_update_pwm_buffers(void);
    rgb_matrix_set_color_all(RGB_RED);
    rgb_matrix_update_pwm_buffers();
#endif // RGB_MATRIX_ENABLE
    return true;
}

void trackball_oled_default(void) {
#ifdef OLED_ENABLE
    char count_default_str[6];
    snprintf(count_default_str, sizeof(count_default_str), "%d", charybdis_get_pointer_default_dpi());
    oled_write_P(PSTR(" M-DPI  :"), false);
    oled_write_ln(count_default_str, false);
#endif // OLED_ENABLE
}

void trackball_oled_info(void) {
#ifdef OLED_ENABLE
    float accel_sensitivity = get_pointer_accel_sensitivity(g_charybdis_config);
    uint8_t whole = (uint8_t)accel_sensitivity;
    uint8_t fractional = (uint8_t)((accel_sensitivity - whole) * 100);
    char accel_str[8];
    snprintf(accel_str, sizeof(accel_str), "%d.%02d", whole, fractional);
    oled_write_P(PSTR(" ACC-S :"), false);
    oled_write_ln(accel_str, false);

    char is_sniping_str[2];
    snprintf(is_sniping_str, sizeof(is_sniping_str), "%d", charybdis_get_pointer_sniping_enabled());
    oled_write_P(PSTR("SNP-T:"), false);
    oled_write(is_sniping_str, false);

    char count_sniping_str[6];
    snprintf(count_sniping_str, sizeof(count_sniping_str), "%d", charybdis_get_pointer_sniping_dpi());
    oled_write_P(PSTR(" SNP-DPI:"), false);
    oled_write_ln(count_sniping_str, false);

    char is_dragscroll_str[2];
    snprintf(is_dragscroll_str, sizeof(is_dragscroll_str), "%d", charybdis_get_pointer_dragscroll_enabled());
    oled_write_P(PSTR("DRG-T:"), false);
    oled_write(is_dragscroll_str, false);

    char count_dragscroll_str[6];
    snprintf(count_dragscroll_str, sizeof(count_dragscroll_str), "%d", g_charybdis_config.pointer_dragscroll_dpi);
    oled_write_P(PSTR(" DRG-DPI:"), false);
    oled_write_ln(count_dragscroll_str, false);
#endif // OLED_ENABLE
}

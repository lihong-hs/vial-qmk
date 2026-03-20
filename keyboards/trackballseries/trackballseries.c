
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
#    ifndef KEYBALL_MINIMUM_DEFAULT_DPI
#        define KEYBALL_MINIMUM_DEFAULT_DPI 300
#    endif // KEYBALL_MINIMUM_DEFAULT_DPI

#    ifndef KEYBALL_DEFAULT_DPI_CONFIG_STEP
#        define KEYBALL_DEFAULT_DPI_CONFIG_STEP 50
#    endif // KEYBALL_DEFAULT_DPI_CONFIG_STEP

#    ifndef KEYBALL_MINIMUM_SNIPING_DPI
#        define KEYBALL_MINIMUM_SNIPING_DPI 100
#    endif // KEYBALL_MINIMUM_SNIPER_MODE_DPI

#    ifndef KEYBALL_SNIPING_DPI_CONFIG_STEP
#        define KEYBALL_SNIPING_DPI_CONFIG_STEP 50
#    endif // KEYBALL_SNIPING_DPI_CONFIG_STEP

#    ifndef KEYBALL_MINIMUM_DRAGSCROLL_DPI
#        define KEYBALL_MINIMUM_DRAGSCROLL_DPI 200
#    endif // KEYBALL_MINIMUM_DRAGSCROLL_DPI

#    ifndef KEYBALL_DRAGSCROLL_DPI_CONFIG_STEP
#        define KEYBALL_DRAGSCROLL_DPI_CONFIG_STEP 50
#    endif // KEYBALL_DRAGSCROLL_DPI_CONFIG_STEP

#    ifndef KEYBALL_DRAGSCROLL_BUFFER_SIZE
#        define KEYBALL_DRAGSCROLL_BUFFER_SIZE 6
#    endif // !KEYBALL_DRAGSCROLL_BUFFER_SIZE

#    ifndef KEYBALL_BASE_ACCEL_FACTOR
#        define KEYBALL_BASE_ACCEL_FACTOR 0.00f
#    endif // KEYBALL_BASE_ACCEL_FACTOR

#    ifndef KEYBALL_ACCEL_FACTOR_STEP_SIZE
#        define KEYBALL_ACCEL_FACTOR_STEP_SIZE 0.05f
#    endif // KEYBALL_ACCEL_FACTOR_STEP_SIZE

#    define DEFAULT_DPI_DATA_BITS 4    // 16 steps available.
#    define SNIPING_DPI_DATA_BITS 2    // 4 steps available.
#    define DRAGSCROLL_DPI_DATA_BITS 4 // 16 steps available.
#    define ACCEL_FACTOR_DATA_BITS 5   // 32 steps available.

// Auto-calculate max index values
#    define DEFAULT_DPI_INDEX_MAX ((1 << DEFAULT_DPI_DATA_BITS) - 1)
#    define SNIPING_DPI_INDEX_MAX ((1 << SNIPING_DPI_DATA_BITS) - 1)
#    define DRAGSCROLL_DPI_INDEX_MAX ((1 << DRAGSCROLL_DPI_DATA_BITS) - 1)
#    define ACCEL_FACTOR_INDEX_MAX ((1 << ACCEL_FACTOR_DATA_BITS) - 1)

typedef union {
    uint32_t raw;
    struct {
        uint8_t pointer_default_dpi_index : DEFAULT_DPI_DATA_BITS;
        uint8_t pointer_accel_factor_index : ACCEL_FACTOR_DATA_BITS;
        uint8_t pointer_sniping_dpi_index : SNIPING_DPI_DATA_BITS;
        uint8_t pointer_dragscroll_dpi_index : DRAGSCROLL_DPI_DATA_BITS;
        bool    is_dragscroll_enabled : 1;
        bool    is_sniping_enabled : 1;
    } __attribute__((packed));
} keyball_config_t;

static keyball_config_t g_keyball_config = {0};

static void read_keyball_config_from_eeprom(keyball_config_t *config) {
    config->raw                   = eeconfig_read_kb();
    config->is_dragscroll_enabled = false;
    config->is_sniping_enabled    = false;
}

static void write_keyball_config_to_eeprom(keyball_config_t *config) {
    eeconfig_update_kb(config->raw);
}

// ===============================================
// Config Getters
// ===============================================

// Default DPI
static uint16_t get_pointer_default_dpi(keyball_config_t *config) {
    return (uint16_t)config->pointer_default_dpi_index * KEYBALL_DEFAULT_DPI_CONFIG_STEP + KEYBALL_MINIMUM_DEFAULT_DPI;
}

uint16_t keyball_get_pointer_default_dpi(void) {
    return get_pointer_default_dpi(&g_keyball_config);
}

// Sniping DPI
bool keyball_get_pointer_sniping_enabled(void) {
    return g_keyball_config.is_sniping_enabled;
}

static uint16_t get_pointer_sniping_dpi(keyball_config_t *config) {
    return (uint16_t)config->pointer_sniping_dpi_index * KEYBALL_SNIPING_DPI_CONFIG_STEP + KEYBALL_MINIMUM_SNIPING_DPI;
}

uint16_t keyball_get_pointer_sniping_dpi(void) {
    return get_pointer_sniping_dpi(&g_keyball_config);
}

// Drag Scroll DPI
bool keyball_get_pointer_dragscroll_enabled(void) {
    return g_keyball_config.is_dragscroll_enabled;
}

static uint16_t get_pointer_dragscroll_dpi(keyball_config_t *config) {
    return (uint16_t)config->pointer_dragscroll_dpi_index * KEYBALL_DRAGSCROLL_DPI_CONFIG_STEP + KEYBALL_MINIMUM_DRAGSCROLL_DPI;
}

uint16_t keyball_get_pointer_dragscroll_dpi(void) {
    return get_pointer_dragscroll_dpi(&g_keyball_config);
}

// Acceleration
static float get_pointer_accel_factor(keyball_config_t *config) {
    return (float)config->pointer_accel_factor_index * KEYBALL_ACCEL_FACTOR_STEP_SIZE + KEYBALL_BASE_ACCEL_FACTOR;
}

float keyball_get_pointer_accel_factor(void) {
    return get_pointer_accel_factor(&g_keyball_config);
}

// ===============================================
// Check mode and apply DPI if needed
// ===============================================

static void apply_dpi(keyball_config_t *config) {
    if (config->is_dragscroll_enabled) {
        pointing_device_set_cpi(get_pointer_dragscroll_dpi(config));
    } else if (config->is_sniping_enabled) {
        pointing_device_set_cpi(get_pointer_sniping_dpi(config));
    } else {
        pointing_device_set_cpi(get_pointer_default_dpi(config));
    }
}

// ===============================================
// Config Setters
// ===============================================

// Default DPI
static void step_pointer_default_dpi(keyball_config_t *config, bool forward) {
    if (forward) {
        if (config->pointer_default_dpi_index < DEFAULT_DPI_INDEX_MAX) {
            config->pointer_default_dpi_index++;
        }
    } else {
        if (config->pointer_default_dpi_index > 0) {
            config->pointer_default_dpi_index--;
        }
    }

    write_keyball_config_to_eeprom(&g_keyball_config);
    apply_dpi(config);
}

// Sniping DPI
void set_pointer_sniping_enabled(bool enable) {
    g_keyball_config.is_sniping_enabled = enable;
    apply_dpi(&g_keyball_config);
}

static void step_pointer_sniping_dpi(keyball_config_t *config, bool forward) {
    if (forward) {
        if (config->pointer_sniping_dpi_index < SNIPING_DPI_INDEX_MAX) {
            config->pointer_sniping_dpi_index++;
        }
    } else {
        if (config->pointer_sniping_dpi_index > 0) {
            config->pointer_sniping_dpi_index--;
        }
    }

    write_keyball_config_to_eeprom(&g_keyball_config);
    apply_dpi(&g_keyball_config);
}

// Drag Scoll DPI
void set_pointer_dragscroll_enabled(bool enable) {
    g_keyball_config.is_dragscroll_enabled = enable;
    apply_dpi(&g_keyball_config);
}

static void step_pointer_dragscroll_dpi(keyball_config_t *config, bool forward) {
    if (forward) {
        if (config->pointer_dragscroll_dpi_index < DRAGSCROLL_DPI_INDEX_MAX) {
            config->pointer_dragscroll_dpi_index++;
        }
    } else {
        if (config->pointer_dragscroll_dpi_index > 0) {
            config->pointer_dragscroll_dpi_index--;
        }
    }

    write_keyball_config_to_eeprom(&g_keyball_config);
    apply_dpi(&g_keyball_config);
}

// Acceleration
static void step_pointer_accel_factor(keyball_config_t *config, bool forward) {
    if (forward) {
        if (config->pointer_accel_factor_index < ACCEL_FACTOR_INDEX_MAX) {
            config->pointer_accel_factor_index++;
        }
    } else {
        if (config->pointer_accel_factor_index > 0) {
            config->pointer_accel_factor_index--;
        }
    }

    write_keyball_config_to_eeprom(&g_keyball_config);
}

// ===============================================
// Acceleration Calculation
// ===============================================

// The minimum speed (counts/ms) required before acceleration kicks in.
// Prevents jitter when making tiny, precise movements.
#    define VELOCITY_THRESHOLD 1.0f
// The maximum HID report value allowed. Prevents cursor from flying off-screen.
#    define MAX_REPORT_VALUE 127

static uint32_t last_movement_timer = 0;

// --- Acceleration Logic ---
/**
 * \brief Calculates the speed multiplier based on how fast the trackball is moved.
 *
 * \param velocity      The speed of movement (counts per millisecond).
 * \param accel_factor   The configuration slope (how aggressively speed increases).
 * \return              A multiplier (1.0 = normal speed, 2.0 = double speed).
 */
static float calculate_velocity_multiplier(float velocity, float accel_factor) {
    // If moving slower than threshold, no acceleration (1.0x speed)
    if (velocity <= VELOCITY_THRESHOLD) {
        return 1.0f;
    }

    // Calculate how much faster than the threshold we are moving
    float excess_velocity = velocity - VELOCITY_THRESHOLD;

    // Apply the acceleration curve
    // Formula: Base Speed + (Excess Speed * Accelleration Factor)
    float multiplier = 1.0f + (excess_velocity * accel_factor);

    return multiplier;
}

/**
 * \brief Applies acceleration to a single axis based on time elapsed.
 */
static int16_t apply_velocity_accel(int16_t movement_delta, uint16_t time_elapsed_ms, float accel_factor) {
    // Safety: Avoid division by zero if timer hasn't updated
    if (time_elapsed_ms == 0 || movement_delta == 0) {
        return movement_delta;
    }

    // 1. Calculate Velocity (Distance / Time)
    // We use abs() because direction doesn't matter for speed calculation
    float velocity = (float)abs(movement_delta) / (float)time_elapsed_ms;

    // 2. Get the Speed Multiplier based on Velocity
    float multiplier = calculate_velocity_multiplier(velocity, accel_factor);

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

        // Get accel factor from config
        float accel_factor = keyball_get_pointer_accel_factor();

        // Apply acceleration to both X and Y axes
        mouse_report.x = apply_velocity_accel(mouse_report.x, current_timer, accel_factor);
        mouse_report.y = apply_velocity_accel(mouse_report.y, current_timer, accel_factor);
    }

    return mouse_report;
}

// ===============================================
// Pointing Device Dehavior
//// Drag Scoll
// ===============================================

static void pointing_device_task_keyball(report_mouse_t *mouse_report) {
    static int16_t scroll_buffer_x = 0;
    static int16_t scroll_buffer_y = 0;
    if (g_keyball_config.is_dragscroll_enabled) {
#    ifdef KEYBALL_DRAGSCROLL_REVERSE_X
        scroll_buffer_x -= mouse_report->x;
#    else
        scroll_buffer_x += mouse_report->x;
#    endif // KEYBALL_DRAGSCROLL_REVERSE_X

#    ifdef KEYBALL_DRAGSCROLL_REVERSE_Y
        scroll_buffer_y -= mouse_report->y;
#    else
        scroll_buffer_y += mouse_report->y;
#    endif // KEYBALL_DRAGSCROLL_REVERSE_Y
        mouse_report->x = 0;
        mouse_report->y = 0;
        if (abs(scroll_buffer_x) > KEYBALL_DRAGSCROLL_BUFFER_SIZE) {
            mouse_report->h = scroll_buffer_x > 0 ? 1 : -1;
            scroll_buffer_x = 0;
        }
        if (abs(scroll_buffer_y) > KEYBALL_DRAGSCROLL_BUFFER_SIZE) {
            mouse_report->v = scroll_buffer_y > 0 ? 1 : -1;
            scroll_buffer_y = 0;
        }
    }
}

report_mouse_t pointing_device_task_kb(report_mouse_t mouse_report) {
    if (is_keyboard_master()) {
        pointing_device_task_keyball(&mouse_report);
        mouse_report = pointing_device_task_user(mouse_report);
    }
    return mouse_report;
}

// ===============================================
// Printing Config
// ===============================================

static char *get_keyball_accell_factor_string(void) {
    static char accel_str[8];

    float   acceleration_factor = keyball_get_pointer_accel_factor();
    uint8_t whole               = (uint8_t)acceleration_factor;
    uint8_t fractional          = (uint8_t)((acceleration_factor - whole) * 100);
    snprintf(accel_str, sizeof(accel_str), "%d.%02d", whole, fractional);

    return accel_str;
}

// clang-format off
char *get_g_keyball_config_string(void) {
    static char buffer[256]; // Static buffer to hold the combined string

    snprintf(buffer, sizeof(buffer),
             "=== Trackball Config ===\n"
             "Default DPI : %d\n"
             "Sniping DPI : %d [%s]\n"
             "Drag-scroll DPI : %d [%s]\n"
             "Accel Factor: %s\n"
             "========================\n",
             keyball_get_pointer_default_dpi(),
             keyball_get_pointer_sniping_dpi(), keyball_get_pointer_sniping_enabled() ? "Active" : "Inactive",
             keyball_get_pointer_dragscroll_dpi(), keyball_get_pointer_dragscroll_enabled() ? "Active" : "Inactive",
             get_keyball_accell_factor_string());

    return buffer;
}
// clang-format on

static void print_keyball_config_to_console(keyball_config_t *config) {
#    ifdef CONSOLE_ENABLE
    if (debug_enable) {
        char *config_string = get_g_keyball_config_string();
        uprintf("%s", config_string);
    }
#    endif // CONSOLE_ENABLE
}

// ===============================================
// Key Process
// ===============================================

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_user(keycode, record)) {
        // debug_charybdis_config_to_console(&g_charybdis_config);
        return false;
    }
#    ifdef POINTING_DEVICE_ENABLE
#        ifndef NO_KEYBALL_KEYCODES
    switch (keycode) {
        case POINTER_DEFAULT_DPI_FORWARD:
            if (record->event.pressed) {
                // Step backward if shifted, forward otherwise.
                step_pointer_default_dpi(&g_keyball_config, true);
                print_keyball_config_to_console(&g_keyball_config);
            }
            break;
        case POINTER_DEFAULT_DPI_REVERSE:
            if (record->event.pressed) {
                // Step forward if shifted, backward otherwise.
                step_pointer_default_dpi(&g_keyball_config, false);
                print_keyball_config_to_console(&g_keyball_config);
            }
            break;
        case POINTER_ACCEL_SENSITIVITY_FORWARD:
            if (record->event.pressed) {
                // Shift reverses direction (consistent with DPI keys)
                step_pointer_accel_factor(&g_keyball_config, true);
                print_keyball_config_to_console(&g_keyball_config);
            }
            break;
        case POINTER_ACCEL_SENSITIVITY_REVERSE:
            if (record->event.pressed) {
                step_pointer_accel_factor(&g_keyball_config, false);
                print_keyball_config_to_console(&g_keyball_config);
            }
            break;
        case POINTER_SNIPING_DPI_FORWARD:
            if (record->event.pressed) {
                step_pointer_sniping_dpi(&g_keyball_config, true);
                print_keyball_config_to_console(&g_keyball_config);
            }
            break;
        case POINTER_SNIPING_DPI_REVERSE:
            if (record->event.pressed) {
                step_pointer_sniping_dpi(&g_keyball_config, false);
                print_keyball_config_to_console(&g_keyball_config);
            }
            break;
        case SNIPING_MODE:
            set_pointer_sniping_enabled(record->event.pressed);
            break;
        case SNIPING_MODE_TOGGLE:
            if (record->event.pressed) {
                set_pointer_sniping_enabled(!keyball_get_pointer_sniping_enabled());
                print_keyball_config_to_console(&g_keyball_config);
            }
            break;

        case POINTER_DRAGSCROLL_DPI_FORWARD:
            if (record->event.pressed) {
                step_pointer_dragscroll_dpi(&g_keyball_config, true);
                print_keyball_config_to_console(&g_keyball_config);
            }
            break;
        case POINTER_DRAGSCROLL_DPI_REVERSE:
            if (record->event.pressed) {
                step_pointer_dragscroll_dpi(&g_keyball_config, false);
                print_keyball_config_to_console(&g_keyball_config);
            }
            break;
        case DRAGSCROLL_MODE:
            set_pointer_dragscroll_enabled(record->event.pressed);
            break;
        case DRAGSCROLL_MODE_TOGGLE:
            if (record->event.pressed) {
                set_pointer_dragscroll_enabled(!keyball_get_pointer_dragscroll_enabled());
                print_keyball_config_to_console(&g_keyball_config);
            }
            break;
        case PRINT_CONFIG:
            if (record->event.pressed) {
                char *config_string = get_g_keyball_config_string();
                SEND_STRING(config_string);
            }
            break;
    }

#        endif // !NO_KEYBALL_KEYCODES
#    endif     // POINTING_DEVICE_ENABLE

    switch (keycode) {
        case DB_TOGG_CUSTOM:
            if (record->event.pressed) {
#    ifdef CONSOLE_ENABLE
                debug_enable = !debug_enable;
                if (debug_enable) {
                    uprint("Toggle debug on\n========================\n");
                    print_keyball_config_to_console(&g_keyball_config);
                } else {
                    uprint("Toggle debug off\n========================\n");
                }
#    endif // CONSOLE_ENABLE
            }
            break;
    }
    return true;
}

// ===============================================
// Config & EEPROM
// ===============================================

void eeconfig_init_kb(void) {
    g_keyball_config.raw = 0;

    // Default accel factor: index 0 = 0.0f (flat)
    // g_keyball_config.pointer_accel_factor = 0;

    write_keyball_config_to_eeprom(&g_keyball_config);
    apply_dpi(&g_keyball_config);
    eeconfig_init_user();
}

void matrix_init_kb(void) {
    read_keyball_config_from_eeprom(&g_keyball_config);
    matrix_init_user();
}

// ===============================================
//  Config Sync
// ===============================================

#    ifdef KEYBALL_CONFIG_SYNC
// void keyball_config_sync_handler(uint8_t initiator2target_buffer_size, const void *initiator2target_buffer, uint8_t target2initiator_buffer_size, void *target2initiator_buffer) {
//     if (initiator2target_buffer_size == sizeof) {
//         memcpy(&g_keyball_config, initiator2target_buffer, sizeof);
//     }
// }
void keyball_config_sync_handler(uint8_t initiator2target_buffer_size, const void *initiator2target_buffer, uint8_t target2initiator_buffer_size, void *target2initiator_buffer) {
    // Fixed: Added operand to sizeof
    if (initiator2target_buffer_size == sizeof(keyball_config_t)) {
        // Fixed: Added operand to sizeof
        memcpy(&g_keyball_config, initiator2target_buffer, sizeof(keyball_config_t));
        apply_dpi(&g_keyball_config); // Apply changes immediately after sync
    }
}
#    endif

void keyboard_post_init_kb(void) {
    debug_enable = false;
    apply_dpi(&g_keyball_config);
#    ifdef KEYBALL_CONFIG_SYNC
    transaction_register_rpc(RPC_KB_CONFIG_SYNC, keyball_config_sync_handler);
#    endif
    keyboard_post_init_user();
}

#    ifdef KEYBALL_CONFIG_SYNC
void housekeeping_task_kb(void) {
    if (is_keyboard_master()) {
        static keyball_config_t last_keyball_config = {0};
        static uint32_t         last_sync           = 0;
        bool                    needs_sync          = false;

        // Fixed: Pass addresses (&) and use sizeof(variable), not sizeof(&variable)
        if (memcmp(&g_keyball_config, &last_keyball_config, sizeof(g_keyball_config)) != 0) {
            needs_sync = true;
            memcpy(&last_keyball_config, &g_keyball_config, sizeof(g_keyball_config));
        }

        // sync every 500ms
        if (timer_elapsed32(last_sync) > 500) {
            needs_sync = true;
        }

        if (needs_sync) {
            // Fixed: Use sizeof(g_keyball_config) instead of sizeof(&...)
            if (transaction_rpc_send(RPC_KB_CONFIG_SYNC, sizeof(g_keyball_config), &g_keyball_config)) {
                last_sync = timer_read32();
            }
        }
    }
}
#    endif // KEYBALL_CONFIG_SYNC
#endif     // POINTING_DEVICE_ENABLE

// ===============================================
//  General
// ===============================================

bool shutdown_kb(bool jump_to_bootloader) {
    if (!shutdown_user(jump_to_bootloader)) {
        return false;
    }

#ifdef RGB_MATRIX_ENABLE
    if (jump_to_bootloader) {
        // red for bootloader
        rgb_matrix_set_color_all(RGB_OFF);
    } else {
        // off for soft reset
        rgb_matrix_set_color_all(RGB_GREEN);
    }
    // force flushing -- otherwise will never happen
    rgb_matrix_update_pwm_buffers();
#endif // RGB_MATRIX_ENABLE

    return true;
}

// ===============================================
// OLED
// ===============================================

void oled_trackball_config_info(void) {
#ifdef OLED_ENABLE
    char count_default_str[6];
    snprintf(count_default_str, sizeof(count_default_str), "%d", keyball_get_pointer_default_dpi());
    oled_write_P(PSTR(" M-DPI  :"), false);
    oled_write_ln(count_default_str, false);

    char is_sniping_str[2];
    snprintf(is_sniping_str, sizeof(is_sniping_str), "%d", keyball_get_pointer_sniping_enabled());
    oled_write_P(PSTR("SNP-T:"), false);
    oled_write(is_sniping_str, false);

    char count_sniping_str[6];
    snprintf(count_sniping_str, sizeof(count_sniping_str), "%d", keyball_get_pointer_sniping_dpi());
    oled_write_P(PSTR(" SNP-DPI:"), false);
    oled_write_ln(count_sniping_str, false);

    char is_dragscroll_str[2];
    snprintf(is_dragscroll_str, sizeof(is_dragscroll_str), "%d", keyball_get_pointer_dragscroll_enabled());
    oled_write_P(PSTR("DRG-T:"), false);
    oled_write(is_dragscroll_str, false);

    char count_dragscroll_str[6];
    snprintf(count_dragscroll_str, sizeof(count_dragscroll_str), "%d", keyball_get_pointer_dragscroll_dpi());
    oled_write_P(PSTR(" DRG-DPI:"), false);
    oled_write_ln(count_dragscroll_str, false);

    char *accel_str = get_keyball_accell_factor_string();
    oled_write_P(PSTR(" ACC-F :"), false);
    oled_write_ln(accel_str, false);
#endif // OLED_ENABLE
}

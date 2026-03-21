
#include QMK_KEYBOARD_H
#include "transactions.h"

enum keyball_keymap_layers {
    LAYER_BASE = 0,
    LAYER_QWERTY,
    LAYER_LOWER_MOUSE_NAV,
    LAYER_QWERTY_MOUSE,
    LAYER_WORKSPACE_SWITCH,
    LAYER_F_KEYS,
    LAYER_SYMBOLS,
    LAYER_NUMPAD,
    LAYER_UPPER_MOUSE_NAV,
    LAYER_SETTINGS,
};

typedef union {
    uint8_t raw;
    struct {
        bool is_oled_enabled : 1;
        bool is_oled_display_info : 1;
        bool is_backlight_enabled : 1;
    };
} auto_config_t;

static auto_config_t g_user_config = {0};

typedef union {
    uint8_t raw;
    struct {
        bool is_oled_timeout : 1;
    };
} kb_state_t;

static kb_state_t g_kb_state = {0};

void g_user_config_sync_handler(uint8_t initiator2target_buffer_size, const void *initiator2target_buffer, uint8_t target2initiator_buffer_size, void *target2initiator_buffer) {
    if (initiator2target_buffer_size == sizeof(auto_config_t)) {
        memcpy(&g_user_config, initiator2target_buffer, sizeof(auto_config_t));
    }
}

void kb_state_sync_handler(uint8_t initiator2target_buffer_size, const void *initiator2target_buffer, uint8_t target2initiator_buffer_size, void *target2initiator_buffer) {
    if (initiator2target_buffer_size == sizeof(kb_state_t)) {
        memcpy(&g_kb_state, initiator2target_buffer, sizeof(kb_state_t));
    }
}

void keyboard_post_init_user(void) {
    g_user_config.raw = eeconfig_read_user();
    transaction_register_rpc(RPC_USER_CONFIG_SYNC, g_user_config_sync_handler);
    transaction_register_rpc(RPC_USER_KB_STATE_SYNC, kb_state_sync_handler);
}

void eeconfig_init_user(void) {
    g_user_config.raw                  = 0;
    g_user_config.is_oled_enabled      = 0;
    g_user_config.is_oled_display_info = 1;
    g_user_config.is_backlight_enabled = 0;
    eeconfig_update_user(g_user_config.raw);
}

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮                                            ╭──────────────────────────────────────────────────────╮
       LT(9,KC_ESCAPE),  KC_1,  KC_2,  KC_3,   KC_4,  KC_5,                                                KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    TG(2),
  // ├──────────────────────────────────────────────────────┤                                            ├──────────────────────────────────────────────────────┤
       LSG_T(KC_GRAVE),  KC_Q, LT(7,KC_W),  LT(6,KC_F), LT(8,KC_P),  KC_B,                                 KC_J,  LT(5,KC_L),  LT(6,KC_U), LT(7,KC_Y),  KC_SEMICOLON, KC_MINUS,
  // ├──────────────────────────────────────────────────────┤                                            ├──────────────────────────────────────────────────────┤
       LSG_T(KC_TAB), LALT_T(KC_A), LGUI_T(KC_R), LSFT_T(KC_S), LCTL_T(KC_T), LT(2,KC_G),                  KC_M,  LCTL_T(KC_N),  LSFT_T(KC_E),  LGUI_T(KC_I), LALT_T(KC_O), KC_QUOT,
  // ├──────────────────────────────────────────────────────┤                                            ├──────────────────────────────────────────────────────┤
       KC_NO,    KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,    OLED_TOG,                   BACKLIGHT_TOG,   KC_K,  KC_H, KC_COMM,  KC_DOT, KC_SLASH, KC_BACKSLASH,
  // ╰──────────────────────────────────────────────────────┤                                            ├──────────────────────────────────────────────────────╯
       KC_NO,   KC_NO,   LT(7,KC_DELETE),   LT(5,KC_BSPC),
                                LCTL_T(KC_SPACE), LGUI_T(KC_ENTER), KC_DELETE,                   LT(6,KC_ENTER),  LT(7,KC_BSPC),        KC_LEFT_BRACKET, KC_RIGHT_BRACKET
  //                                    ╰───────────────────────────╯                               ╰──────────────────╯                     ╰──────────────────╯
  ),
  [LAYER_QWERTY] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮                   ╭──────────────────────────────────────────────────────╮
       KC_TILD, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_UNDS,
  // ├──────────────────────────────────────────────────────┤                   ├──────────────────────────────────────────────────────┤
       RGB_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_LBRC,   KC_P7,   KC_P8,   KC_P9, KC_RBRC, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤                   ├──────────────────────────────────────────────────────┤
       RGB_TOG, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                      KC_PPLS,   KC_P4,   KC_P5,   KC_P6, KC_PMNS, KC_PEQL,
  // ├──────────────────────────────────────────────────────┤                   ├──────────────────────────────────────────────────────┤
      RGB_RMOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   KC_B,      KC_N,   KC_PAST,   KC_P1,   KC_P2,   KC_P3, KC_PSLS, KC_PDOT,
  // ╰──────────────────────────────────────────────────────┤                   ├──────────────────────────────────────────────────────╯
       KC_LGUI,   KC_SPC,   XXXXXXX,   KC_LALT,                                                                      KC_SPC, KC_LALT,
                                             KC_LALT, KC_BSPC, KC_LALT,          KC_SPC, KC_DEL
  //                            ╰───────────────────────────╯               ╰──────────────────╯
  ),

  [LAYER_LOWER_MOUSE_NAV] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮               ╭──────────────────────────────────────────────────────╮
       _______, _______, _______, _______, _______, _______,                  TG(3),   KC_HOME,   KC_PGUP,   KC_PGDN,  KC_END,  TG(2),
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       _______, _______, _______, _______, _______, _______,                  KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       _______, _______, _______, _______, _______, _______,                  KC_WBAK, KC_BTN1, KC_BTN2, DRG_TOG, KC_BTN3, SNP_TOG,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       _______, _______, _______, _______, _______, _______,   KC_B,   KC_N,  KC_WFWD, LCTL(KC_C), LCTL(KC_V), SNIPING, LCTL(KC_X), DRG_TOG,
  // ╰──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────╯
      _______, _______, _______, _______,                                                                      _______, _______,
                                            _______, _______, _______,        _______, _______
 //                            ╰───────────────────────────╯               ╰──────────────────╯
  ),

  [LAYER_QWERTY_MOUSE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮               ╭──────────────────────────────────────────────────────╮
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  TG(3), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DPI_MOD, S_D_MOD,                  S_D_MOD, DPI_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                  XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, _______, DRGSCRL, SNIPING, KC_B, KC_B,   KC_B,   KC_N,  KC_B, KC_B, SNIPING, DRGSCRL, _______, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────╯
      KC_LGUI,   KC_SPC,   XXXXXXX,   KC_LALT,                                                                      KC_SPC, KC_LALT,
                                            KC_LALT, KC_BSPC, KC_LALT,        KC_BTN1, KC_BTN2
 //                            ╰───────────────────────────╯               ╰──────────────────╯
  ),
  [LAYER_WORKSPACE_SWITCH] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮               ╭──────────────────────────────────────────────────────╮
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DPI_MOD, S_D_MOD,                  S_D_MOD, DPI_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                  XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, _______, DRGSCRL, SNIPING, KC_B, KC_B,   KC_B,   KC_N,  KC_B, KC_B, SNIPING, DRGSCRL, _______, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────╯
      KC_LGUI,   KC_SPC,   XXXXXXX,   KC_LALT,                                                                      KC_SPC, KC_LALT,
                                            KC_LALT, KC_BSPC, KC_LALT,        KC_BTN1, KC_BTN2
 //                            ╰───────────────────────────╯               ╰──────────────────╯
  ),
   [LAYER_F_KEYS] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮               ╭──────────────────────────────────────────────────────╮
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DPI_MOD, S_D_MOD,                  S_D_MOD, DPI_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                  XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, _______, DRGSCRL, SNIPING, KC_B, KC_B,   KC_B,   KC_N,  KC_B, KC_B, SNIPING, DRGSCRL, _______, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────╯
      KC_LGUI,   KC_SPC,   XXXXXXX,   KC_LALT,                                                                      KC_SPC, KC_LALT,
                                            KC_LALT, KC_BSPC, KC_LALT,        KC_BTN1, KC_BTN2
 //                            ╰───────────────────────────╯               ╰──────────────────╯
  ),
  [LAYER_SYMBOLS] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮               ╭──────────────────────────────────────────────────────╮
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DPI_MOD, S_D_MOD,                  S_D_MOD, DPI_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                  XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, _______, DRGSCRL, SNIPING, KC_B, KC_B,   KC_B,   KC_N,  KC_B, KC_B, SNIPING, DRGSCRL, _______, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────╯
      KC_LGUI,   KC_SPC,   XXXXXXX,   KC_LALT,                                                                      KC_SPC, KC_LALT,
                                            KC_LALT, KC_BSPC, KC_LALT,        KC_BTN1, KC_BTN2
 //                            ╰───────────────────────────╯               ╰──────────────────╯
  ),
   [LAYER_NUMPAD] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮               ╭──────────────────────────────────────────────────────╮
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DPI_MOD, S_D_MOD,                  S_D_MOD, DPI_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                  XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, _______, DRGSCRL, SNIPING, KC_B, KC_B,   KC_B,   KC_N,  KC_B, KC_B, SNIPING, DRGSCRL, _______, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────╯
      KC_LGUI,   KC_SPC,   XXXXXXX,   KC_LALT,                                                                      KC_SPC, KC_LALT,
                                            KC_LALT, KC_BSPC, KC_LALT,        KC_BTN1, KC_BTN2
 //                            ╰───────────────────────────╯               ╰──────────────────╯
  ),
   [LAYER_UPPER_MOUSE_NAV] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮               ╭──────────────────────────────────────────────────────╮
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DPI_MOD, S_D_MOD,                  S_D_MOD, DPI_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                  XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, _______, DRGSCRL, SNIPING, KC_B, KC_B,   KC_B,   KC_N,  KC_B, KC_B, SNIPING, DRGSCRL, _______, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────╯
      KC_LGUI,   KC_SPC,   XXXXXXX,   KC_LALT,                                                                      KC_SPC, KC_LALT,
                                            KC_LALT, KC_BSPC, KC_LALT,        KC_BTN1, KC_BTN2
 //                            ╰───────────────────────────╯               ╰──────────────────╯
  ),
  [LAYER_SETTINGS] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮               ╭──────────────────────────────────────────────────────╮
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, DPI_MOD, S_D_MOD,                  S_D_MOD, DPI_MOD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,                  XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, _______, DRGSCRL, SNIPING, KC_B, KC_B,   KC_B,   KC_N,  KC_B, KC_B, SNIPING, DRGSCRL, _______, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────╯
      KC_LGUI,   KC_SPC,   XXXXXXX,   KC_LALT,
                                            KC_LALT, KC_BSPC, KC_LALT,        KC_SPC, KC_LALT,                     KC_BTN1, KC_BTN2
 //                            ╰───────────────────────────╯               ╰──────────────────╯
  ),
  [10] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮               ╭──────────────────────────────────────────────────────╮
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────╯
      XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,
                                            XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX
 //                            ╰───────────────────────────╯               ╰──────────────────╯
  ),
  [11] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮               ╭──────────────────────────────────────────────────────╮
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────╯
      XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,
                                            XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX
 //                            ╰───────────────────────────╯               ╰──────────────────╯
  ),
  [12] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮               ╭──────────────────────────────────────────────────────╮
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────╯
      XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,
                                            XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX
 //                            ╰───────────────────────────╯               ╰──────────────────╯
  ),
  [13] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮               ╭──────────────────────────────────────────────────────╮
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────╯
      XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,
                                            XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX
 //                            ╰───────────────────────────╯               ╰──────────────────╯
  ),
  [14] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮               ╭──────────────────────────────────────────────────────╮
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────╯
      XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,
                                            XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX
 //                            ╰───────────────────────────╯               ╰──────────────────╯
  ),
  [15] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮               ╭──────────────────────────────────────────────────────╮
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  // ╰──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────╯
      XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX,
                                            XXXXXXX, XXXXXXX, XXXXXXX,        XXXXXXX, XXXXXXX,                    XXXXXXX, XXXXXXX
 //                            ╰───────────────────────────╯               ╰──────────────────╯
  ),
};

// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
    led_t led_state      = host_keyboard_led_state();
    bool  num_lock_state = led_state.num_lock;

    switch (get_highest_layer(state)) {
        case LAYER_BASE:
            break;
        case LAYER_QWERTY:
            break;
        case LAYER_LOWER_MOUSE_NAV:
            break;
        case LAYER_QWERTY_MOUSE:
            break;
        case LAYER_WORKSPACE_SWITCH:
            break;
        case LAYER_F_KEYS:
            break;
        case LAYER_SYMBOLS:
            break;
        case LAYER_NUMPAD: // turn on numlock, if it isn't already on.
            if (!num_lock_state) {
                tap_code(KC_NUM_LOCK);
            }
            break;
        case LAYER_UPPER_MOUSE_NAV:
            break;
        case LAYER_SETTINGS:
            break;
        default:
    }
    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case OLED_TOG:
            if (record->event.pressed) { // key down
                g_user_config.is_oled_enabled ^= 1;
                eeconfig_update_user(g_user_config.raw);
            }
            return false;
        case OLED_INFO_TOG:
            if (record->event.pressed) { // key down
                g_user_config.is_oled_display_info ^= 1;
            }
            return false;
        case BACKLIGHT_TOG:
            if (record->event.pressed) { // key down
                g_user_config.is_backlight_enabled ^= 1;
                eeconfig_update_user(g_user_config.raw);
            }
            return false;
    }
    return true;
}

// ===============================================
// RGB
// ===============================================

#ifdef RGB_MATRIX_ENABLE

enum rgb_highlight_mode {
    HIGHLIGHT_DEFAULT              = 0,
    HIGHLIGHT_PER_INDEX            = 1,
    HIGHLIGHT_WHERE_THERE_ARE_KEYS = 2,
};

typedef struct {
    int   index;
    hsv_t color;
} indexed_color_t;

bool contains_int(const int arr[], size_t size, int target) {
    for (size_t i = 0; i < size; i++) {
        if (arr[i] == target) {
            return true;
        }
    }
    return false;
}

// Find index in indexed color array
int find_indexed_color(const indexed_color_t arr[], size_t size, int target) {
    for (size_t i = 0; i < size; i++) {
        if (arr[i].index == target) {
            return i;
        }
    }
    return -1;
}

// clang-format off
static const indexed_color_t base_indexes[] = {};

static const indexed_color_t qwerty_indexes[] = {
    {15, {HSV_YELLOW}},
};

static const indexed_color_t lower_mouse_upper_mouse_nav_indexes[] = {
    {55, {HSV_SPRINGGREEN}},
    {44, {HSV_YELLOW}}, {47, {HSV_YELLOW}}, {48, {HSV_YELLOW}}, {52, {HSV_YELLOW}}
};

static const indexed_color_t qwerty_mouse_indexes[] = {
    {55, {HSV_SPRINGGREEN}},
    {32, {HSV_RED}},
    {24, {HSV_RED}}, {25, {HSV_RED}}, {31, {HSV_RED}},
    {44, {HSV_YELLOW}}, {47, {HSV_YELLOW}}, {48, {HSV_YELLOW}}, {52, {HSV_YELLOW}}
};

static const indexed_color_t workspace_switch_indexes[] = {
    {2, {HSV_RED}},
    {8, {HSV_CORAL}}, {11, {HSV_CORAL}}, {18, {HSV_CORAL}}, {20, {HSV_CORAL}}, {28, {HSV_CORAL}},
    {7, {HSV_CORAL}}, {12, {HSV_CORAL}}, {17, {HSV_CORAL}}, {22, {HSV_CORAL}}, {27, {HSV_CORAL}}
};

static const indexed_color_t fkeys_indexes[] = {
    {2, {HSV_RED}},
    {42, {HSV_ORANGE}}, {49, {HSV_ORANGE}}, {50, {HSV_ORANGE}},
    {43, {HSV_ORANGE}}, {48, {HSV_ORANGE}}, {51, {HSV_ORANGE}},
    {44, {HSV_ORANGE}}, {47, {HSV_ORANGE}}, {52, {HSV_ORANGE}},
    {45, {HSV_ORANGE}}, {46, {HSV_ORANGE}}, {53, {HSV_ORANGE}}
};

static const indexed_color_t symbols_indexes[] = {
    {2, {HSV_RED}},
    {7, {HSV_WHITE}}, {12, {HSV_WHITE}}, {17, {HSV_WHITE}}, {22, {HSV_WHITE}}, {27, {HSV_WHITE}},
    {52, {HSV_WHITE}}, {57, {HSV_WHITE}}, {53, {HSV_YELLOW}},
    {18, {HSV_YELLOW}}, {21, {HSV_YELLOW}}, {28, {HSV_YELLOW}},
    {51, {HSV_YELLOW}}
};

static const indexed_color_t numpad_indexes[] = {
    {43, {HSV_MAGENTA}}, {48, {HSV_MAGENTA}}, {51, {HSV_MAGENTA}},
    {44, {HSV_MAGENTA}}, {47, {HSV_MAGENTA}}, {52, {HSV_MAGENTA}},
    {45, {HSV_MAGENTA}}, {46, {HSV_MAGENTA}}, {53, {HSV_MAGENTA}}
};

static const indexed_color_t upper_mouse_nav_indexes[] = {
    {55, {HSV_SPRINGGREEN}},
    {44, {HSV_ORANGE}}, {47, {HSV_ORANGE}}, {48, {HSV_ORANGE}}, {52, {HSV_ORANGE}}
};
// clang-format on

// Dynamic array for runtime modifications
#    define MAX_DYNAMIC_INDEXES 50
static indexed_color_t dynamic_indexes[MAX_DYNAMIC_INDEXES];
static size_t          dynamic_indexes_size = 0;

// Helper function to build dynamic index list from base array
void build_dynamic_index_list(const indexed_color_t base_array[], size_t base_size) {
    // Reset dynamic array
    dynamic_indexes_size = 0;

    // Copy base indexes with their colors
    for (size_t i = 0; i < base_size && dynamic_indexes_size < MAX_DYNAMIC_INDEXES; i++) {
        dynamic_indexes[dynamic_indexes_size++] = base_array[i];
    }
}

// Function to add or update an index with a specific color
void add_dynamic_index(int index, hsv_t color) {
    int pos = find_indexed_color(dynamic_indexes, dynamic_indexes_size, index);

    if (pos >= 0) {
        // Update existing index's color
        dynamic_indexes[pos].color = color;
    } else if (dynamic_indexes_size < MAX_DYNAMIC_INDEXES) {
        // Add new index with color
        dynamic_indexes[dynamic_indexes_size].index = index;
        dynamic_indexes[dynamic_indexes_size].color = color;
        dynamic_indexes_size++;
    }
}

// Function to add an index with default highlight color
void add_dynamic_index_default(int index, hsv_t default_color) {
    add_dynamic_index(index, default_color);
}

// Function to get color for an index (returns highlight color if found, otherwise base color)
hsv_t get_index_color(int index, hsv_t default_color) {
    int pos = find_indexed_color(dynamic_indexes, dynamic_indexes_size, index);
    if (pos >= 0) {
        return dynamic_indexes[pos].color;
    }
    return default_color;
}

static void add_modifier_status_color(void) {
    led_t led_state = host_keyboard_led_state();
    if (led_state.caps_lock) {
        add_dynamic_index(28, (hsv_t){HSV_RED});
        add_dynamic_index(58, (hsv_t){HSV_RED});
    }

    if (keyball_get_pointer_sniping_enabled()) {
        add_dynamic_index(54, (hsv_t){HSV_PINK});
    }
    if (keyball_get_pointer_dragscroll_enabled()) {
        add_dynamic_index(55, (hsv_t){HSV_BLUE});
    }
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    hsv_t hsv_base      = {HSV_WHITE};
    hsv_t hsv_highlight = {HSV_WHITE};

    if (!g_user_config.is_backlight_enabled) {
        hsv_base = (hsv_t){HSV_OFF};
    }

    int                    lightMode            = HIGHLIGHT_DEFAULT;
    const indexed_color_t *highlightIndexes     = NULL;
    size_t                 highlightIndexesSize = 0;

    uint8_t layer = get_highest_layer(layer_state);

    switch (layer) {
        case LAYER_BASE:
            lightMode = HIGHLIGHT_PER_INDEX;
            build_dynamic_index_list(base_indexes, sizeof(base_indexes) / sizeof(base_indexes[0]));
            add_modifier_status_color();
            highlightIndexes     = dynamic_indexes;
            highlightIndexesSize = dynamic_indexes_size;
            break;
        case LAYER_QWERTY:
            lightMode = HIGHLIGHT_PER_INDEX;
            build_dynamic_index_list(qwerty_indexes, sizeof(qwerty_indexes) / sizeof(qwerty_indexes[0]));
            add_modifier_status_color();
            highlightIndexes     = dynamic_indexes;
            highlightIndexesSize = dynamic_indexes_size;
            break;
        case LAYER_LOWER_MOUSE_NAV:
            lightMode = HIGHLIGHT_PER_INDEX;
            build_dynamic_index_list(lower_mouse_upper_mouse_nav_indexes, sizeof(lower_mouse_upper_mouse_nav_indexes) / sizeof(lower_mouse_upper_mouse_nav_indexes[0]));
            add_modifier_status_color();
            highlightIndexes     = dynamic_indexes;
            highlightIndexesSize = dynamic_indexes_size;
            break;
        case LAYER_QWERTY_MOUSE:
            lightMode = HIGHLIGHT_PER_INDEX;
            build_dynamic_index_list(qwerty_mouse_indexes, sizeof(qwerty_mouse_indexes) / sizeof(qwerty_mouse_indexes[0]));
            add_modifier_status_color();
            highlightIndexes     = dynamic_indexes;
            highlightIndexesSize = dynamic_indexes_size;
            break;
        case LAYER_WORKSPACE_SWITCH:
            lightMode = HIGHLIGHT_PER_INDEX;
            hsv_base  = (hsv_t){HSV_OFF};
            build_dynamic_index_list(workspace_switch_indexes, sizeof(workspace_switch_indexes) / sizeof(workspace_switch_indexes[0]));
            highlightIndexes     = dynamic_indexes;
            highlightIndexesSize = dynamic_indexes_size;
            break;
        case LAYER_F_KEYS:
            lightMode = HIGHLIGHT_PER_INDEX;
            hsv_base  = (hsv_t){HSV_OFF};
            build_dynamic_index_list(fkeys_indexes, sizeof(fkeys_indexes) / sizeof(fkeys_indexes[0]));
            highlightIndexes     = dynamic_indexes;
            highlightIndexesSize = dynamic_indexes_size;
            break;
        case LAYER_SYMBOLS:
            lightMode = HIGHLIGHT_PER_INDEX;
            hsv_base  = (hsv_t){HSV_OFF};
            build_dynamic_index_list(symbols_indexes, sizeof(symbols_indexes) / sizeof(symbols_indexes[0]));
            highlightIndexes     = dynamic_indexes;
            highlightIndexesSize = dynamic_indexes_size;
            break;
        case LAYER_NUMPAD:
            lightMode = HIGHLIGHT_PER_INDEX;
            hsv_base  = (hsv_t){HSV_OFF};
            build_dynamic_index_list(numpad_indexes, sizeof(numpad_indexes) / sizeof(numpad_indexes[0]));
            add_modifier_status_color();

            led_t led_state = host_keyboard_led_state();
            add_dynamic_index(50, led_state.num_lock ? (hsv_t){HSV_MAGENTA} : (hsv_t){HSV_YELLOW});

            highlightIndexes     = dynamic_indexes;
            highlightIndexesSize = dynamic_indexes_size;
            break;
        case LAYER_UPPER_MOUSE_NAV:
            lightMode = HIGHLIGHT_PER_INDEX;
            hsv_base  = (hsv_t){HSV_OFF};
            build_dynamic_index_list(upper_mouse_nav_indexes, sizeof(upper_mouse_nav_indexes) / sizeof(upper_mouse_nav_indexes[0]));
            add_modifier_status_color();
            highlightIndexes     = dynamic_indexes;
            highlightIndexesSize = dynamic_indexes_size;
            break;
        case LAYER_SETTINGS:
            lightMode     = HIGHLIGHT_WHERE_THERE_ARE_KEYS;
            hsv_base      = (hsv_t){HSV_OFF};
            hsv_highlight = (hsv_t){HSV_RED};
            break;
        default:
            break;
    }

    if (hsv_base.v > rgb_matrix_get_val()) {
        hsv_base.v = rgb_matrix_get_val();
    }
    if (hsv_highlight.v > rgb_matrix_get_val()) {
        hsv_highlight.v = rgb_matrix_get_val();
    }

    rgb_t rgb_base      = hsv_to_rgb(hsv_base);
    rgb_t rgb_highlight = hsv_to_rgb(hsv_highlight);

    switch (lightMode) {
        case HIGHLIGHT_DEFAULT:
            rgb_t rgb = rgb_base;
            for (uint8_t i = led_min; i < led_max; i++) {
                rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
            }
            break;
        case HIGHLIGHT_PER_INDEX:
            for (uint8_t i = led_min; i < led_max; i++) {
                rgb_t rgb = rgb_base;
                int   pos = find_indexed_color(highlightIndexes, highlightIndexesSize, i);
                if (pos >= 0) {
                    hsv_t temp = highlightIndexes[pos].color;
                    temp.v     = rgb_matrix_get_val();
                    rgb        = hsv_to_rgb(temp);
                }
                rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
            }
            break;
        case HIGHLIGHT_WHERE_THERE_ARE_KEYS:
            for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
                for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                    uint8_t index = g_led_config.matrix_co[row][col];

                    if (index >= led_min && index < led_max && index != NO_LED) {
                        rgb_t rgb = rgb_base;
                        if (keymap_key_to_keycode(layer, (keypos_t){col, row}) > KC_TRNS) {
                            rgb = rgb_highlight;
                        }
                        rgb_matrix_set_color(index, rgb.r, rgb.g, rgb.b);
                    }
                }
            }
            break;
        default:
            break;
    }

    return false;
}

#endif // RGB_MATRIX_ENABLE

// ===============================================
// OLED
// ===============================================

#ifdef OLED_ENABLE

static void render_space(void) {
    oled_write_P(PSTR("     "), false);
}

static void render_logo(void) {
    oled_write_P(PSTR("KBL59"), false);
}

static void slave_data(void) {
    render_space();
    render_logo();
    render_space();

    uint8_t layer = get_highest_layer(layer_state);

    switch (layer) {
        case LAYER_BASE:
            oled_write("  0  ", false);
            render_space();
            oled_write_ln("Base", false);
            break;
        case LAYER_QWERTY:
            oled_write("  1  ", false);
            render_space();
            oled_write_ln("Qwrty", false);
            break;
        case LAYER_LOWER_MOUSE_NAV:
            oled_write("  2  ", false);
            render_space();
            oled_write_ln("LmNav", false);
            break;
        case LAYER_QWERTY_MOUSE:
            oled_write("  3  ", false);
            render_space();
            oled_write_ln("MiceQ", false);
            break;
        case LAYER_WORKSPACE_SWITCH:
            oled_write("  4  ", false);
            render_space();
            oled_write_ln(" WSS ", false);
            break;
        case LAYER_F_KEYS:
            oled_write("  5  ", false);
            render_space();
            oled_write_ln(" Fn ", false);
            break;
        case LAYER_SYMBOLS:
            oled_write("  6  ", false);
            render_space();
            oled_write_ln(" Sym ", false);
            break;
        case LAYER_NUMPAD:
            oled_write("  7  ", false);
            render_space();
            oled_write_ln("NumPD", false);
            break;
        case LAYER_UPPER_MOUSE_NAV:
            oled_write("  8  ", false);
            render_space();
            oled_write_ln("UmNav", false);
            break;
        case LAYER_SETTINGS:
            oled_write("  9  ", false);
            render_space();
            oled_write_ln("Prefs", false);
            break;
        case 10:
            oled_write(" 10  ", false);
            render_space();
            render_space();
            break;
        case 11:
            oled_write(" 11  ", false);
            render_space();
            render_space();
            break;
        case 12:
            oled_write(" 12  ", false);
            render_space();
            render_space();
            break;
        case 13:
            oled_write(" 13  ", false);
            render_space();
            render_space();
            break;
        case 14:
            oled_write(" 14  ", false);
            render_space();
            render_space();
            break;
        case 15:
            oled_write(" 15  ", false);
            render_space();
            render_space();
            break;
        default:
            oled_write("Undef", false);
            render_space();
            render_space();
    }
}

static void master_data(void) {
    oled_clear();
    oled_trackball_config_info();
}

static bool can_render_oled(void) { // Fixed typo
#    if defined(OLED_TIMEOUT) && OLED_TIMEOUT > 0
    if (is_keyboard_master()) {
        if (last_input_activity_elapsed() < OLED_TIMEOUT) {
            g_kb_state.is_oled_timeout = false;
            return true;
        }
        g_kb_state.is_oled_timeout = true;
        return false;
    }
    return !g_kb_state.is_oled_timeout;
#    else
    return true;
#    endif
}

bool oled_task_user(void) {
    bool can_render = g_user_config.is_oled_enabled && can_render_oled();

    if (can_render) {
        oled_on();
        if (is_keyboard_master()) {
            if (is_oled_on()) {
                master_data();
            }
        } else {
            if (is_oled_on()) {
                slave_data();
            }
        }
    } else {
        oled_off();
    }
    return false;
}

void oled_render_boot(bool bootloader) {
    oled_clear();
    for (int i = 0; i < 16; i++) {
        oled_set_cursor(0, i);
        if (bootloader) {
            oled_write_P(PSTR("Awaiting New Firmware "), false);
        } else {
            oled_write_P(PSTR("Rebooting "), false);
        }
    }

    oled_render_dirty(true);
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return rotation;
    } else {
        return OLED_ROTATION_270;
    }
}

bool shutdown_user(bool jump_to_bootloader) {
    oled_render_boot(jump_to_bootloader);

    return true;
}

#endif // OLED_ENABLE

// ===============================================
// State Sync
// ===============================================

// Generic sync function template
static inline bool sync_state(const void *current_state, void *last_state, uint32_t *last_sync_time, uint16_t rpc_id, size_t state_size) {
    bool needs_sync = false;

    // Check if state has changed
    if (memcmp(current_state, last_state, state_size) != 0) {
        needs_sync = true;
        memcpy(last_state, current_state, state_size);
    }

    // Periodic sync every 500ms
    if (timer_elapsed32(*last_sync_time) > 500) {
        needs_sync = true;
    }

    if (needs_sync) {
        if (transaction_rpc_send(rpc_id, state_size, current_state)) {
            *last_sync_time = timer_read32();
            return true;
        }
        return false;
    }
    return true;
}

#define SYNC_STATE(state_var, rpc_id, last_sync_var, last_state_var) sync_state(&state_var, &last_state_var, &last_sync_var, rpc_id, sizeof(state_var))

void housekeeping_task_user(void) {
    if (is_keyboard_master()) {
        static auto_config_t last_g_user_config    = {0};
        static uint32_t      user_config_last_sync = 0;
        SYNC_STATE(g_user_config, RPC_USER_CONFIG_SYNC, user_config_last_sync, last_g_user_config);

        static kb_state_t last_g_kb_state    = {0};
        static uint32_t   kb_state_last_sync = 0;
        SYNC_STATE(g_kb_state, RPC_USER_KB_STATE_SYNC, kb_state_last_sync, last_g_kb_state);
    }
}

// ===============================================
// ENCODER
// ===============================================

// clang-format off
//旋钮映射需要在vial的rules.mk加入
//ENCODER_ENABLE = yes
//ENCODER_MAP_ENABLE = yes
//旋钮要(0,0 0,1)  (1,0 1,1) 这里前一个代表旋钮下标，后一个都一样，有两个旋钮，前面就是0 1
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [1] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [2] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [3] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [4] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [5] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [6] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [7] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [8] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [9] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [10] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [11] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [12] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [13] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [14] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
    [15] = { ENCODER_CCW_CW(KC_TRNS, KC_TRNS), ENCODER_CCW_CW(KC_TRNS, KC_TRNS) },
};

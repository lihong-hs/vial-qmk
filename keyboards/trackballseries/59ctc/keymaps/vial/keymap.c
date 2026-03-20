
#include QMK_KEYBOARD_H
#include "transactions.h"

enum keyball_keymap_layers {
    LAYER_BASE = 0,
    LAYER_QWERTY,
    LAYER_MOUSE,
    LAYER_MOUSE_QWERTY,
    LAYER_NUMROW,
    LAYER_F_KEYS,
    LAYER_SYMBOLS,
    LAYER_NUMPAD,
    LAYER_NAV,
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

  [LAYER_MOUSE] = LAYOUT(
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

  [LAYER_MOUSE_QWERTY] = LAYOUT(
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
  [LAYER_NUMROW] = LAYOUT(
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
   [LAYER_NAV] = LAYOUT(
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
    led_t led_state        = host_keyboard_led_state();
    bool  g_num_lock_state = led_state.num_lock;

    switch (get_highest_layer(state)) {
        case LAYER_BASE:
            break;
        case LAYER_QWERTY:
            break;
        case LAYER_MOUSE:
            break;
        case LAYER_MOUSE_QWERTY:
            break;
        case LAYER_NUMROW:
            break;
        case LAYER_F_KEYS:
            break;
        case LAYER_SYMBOLS:
            break;
        case LAYER_NUMPAD: // turn on numlock, if it isn't already on.
            if (!g_num_lock_state) {
                tap_code(KC_NUM_LOCK);
            }
            break;
        case LAYER_NAV:
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

bool contains_int(int arr[], int size, int target) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            return true;
        }
    }
    return false;
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    hsv_t hsv_base      = {HSV_WHITE};
    hsv_t hsv_highlight = {HSV_WHITE};

    if (!g_user_config.is_backlight_enabled) {
        hsv_base = (hsv_t){HSV_OFF};
    }

    int lightMode = HIGHLIGHT_DEFAULT;

    int  highlightIndexesSize = 10;
    int *highlightIndexes     = malloc(highlightIndexesSize * sizeof(int));

    uint8_t layer = get_highest_layer(layer_state);
    switch (layer) {
        case LAYER_BASE:
            lightMode = HIGHLIGHT_DEFAULT;
            break;
        case LAYER_QWERTY:
            lightMode = HIGHLIGHT_DEFAULT;
            break;
        case LAYER_MOUSE:
            lightMode     = HIGHLIGHT_PER_INDEX;
            hsv_highlight = (hsv_t){HSV_SPRINGGREEN};
            if (g_user_config.is_backlight_enabled) {
                hsv_base = (hsv_t){HSV_SPRINGGREEN};
            }
            if (keyball_get_pointer_sniping_enabled()) {
                hsv_highlight = (hsv_t){HSV_PINK};
            }
            if (keyball_get_pointer_dragscroll_enabled()) {
                hsv_highlight = (hsv_t){HSV_GOLD};
            }
            int highlightIndexes2[] = {55};
            highlightIndexesSize    = 1;
            memcpy(highlightIndexes, highlightIndexes2, highlightIndexesSize * sizeof(int));
            break;
        case LAYER_MOUSE_QWERTY:
            lightMode     = HIGHLIGHT_PER_INDEX;
            hsv_highlight = (hsv_t){HSV_CHARTREUSE};
            if (g_user_config.is_backlight_enabled) {
                hsv_base = (hsv_t){HSV_CHARTREUSE};
            }
            if (keyball_get_pointer_sniping_enabled()) {
                hsv_highlight = (hsv_t){HSV_PINK};
            }
            if (keyball_get_pointer_dragscroll_enabled()) {
                hsv_highlight = (hsv_t){HSV_GOLD};
            }
            int highlightIndexes3[] = {31, 55};
            highlightIndexesSize    = 2;
            memcpy(highlightIndexes, highlightIndexes3, highlightIndexesSize * sizeof(int));
            break;
        case LAYER_NUMROW:
            lightMode               = HIGHLIGHT_PER_INDEX;
            hsv_base                = (hsv_t){HSV_OFF};
            hsv_highlight           = (hsv_t){HSV_CORAL};
            int highlightIndexes4[] = {7, 12, 17, 22, 27, 39, 44, 47, 52, 57};
            highlightIndexesSize    = 10;
            memcpy(highlightIndexes, highlightIndexes4, highlightIndexesSize * sizeof(int));
            break;
        case LAYER_F_KEYS:
            lightMode               = HIGHLIGHT_PER_INDEX;
            hsv_base                = (hsv_t){HSV_OFF};
            hsv_highlight           = (hsv_t){HSV_ORANGE};
            int highlightIndexes5[] = {7, 27, 39, 57, 43, 48};
            highlightIndexesSize    = 6;
            memcpy(highlightIndexes, highlightIndexes5, highlightIndexesSize * sizeof(int));
            break;
        case LAYER_SYMBOLS:
            lightMode               = HIGHLIGHT_PER_INDEX;
            hsv_base                = (hsv_t){HSV_OFF};
            hsv_highlight           = (hsv_t){HSV_BLUE};
            int highlightIndexes6[] = {21, 42, 49};
            highlightIndexesSize    = 3;
            memcpy(highlightIndexes, highlightIndexes6, highlightIndexesSize * sizeof(int));
            break;
        case LAYER_NUMPAD:
            lightMode               = HIGHLIGHT_PER_INDEX;
            hsv_base                = (hsv_t){HSV_OFF};
            hsv_highlight           = (hsv_t){HSV_MAGENTA};
            int highlightIndexes7[] = {43, 48, 51, 44, 47, 52, 45, 46, 53};
            highlightIndexesSize    = 9;
            memcpy(highlightIndexes, highlightIndexes7, highlightIndexesSize * sizeof(int));
            break;
        case LAYER_NAV:
            lightMode               = HIGHLIGHT_PER_INDEX;
            hsv_base                = (hsv_t){HSV_OFF};
            hsv_highlight           = (hsv_t){HSV_YELLOW};
            int highlightIndexes8[] = {44, 47, 52, 57};
            highlightIndexesSize    = 4;
            memcpy(highlightIndexes, highlightIndexes8, highlightIndexesSize * sizeof(int));
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
                if (contains_int(highlightIndexes, highlightIndexesSize, i)) {
                    rgb = rgb_highlight;
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

    free(highlightIndexes);

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
        case LAYER_MOUSE:
            oled_write("  2  ", false);
            render_space();
            oled_write_ln("Mouse", false);
            break;
        case LAYER_MOUSE_QWERTY:
            oled_write("  3  ", false);
            render_space();
            oled_write_ln("MiceQ", false);
            break;
        case LAYER_NUMROW:
            oled_write("  4  ", false);
            render_space();
            oled_write_ln("NumRw", false);
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
        case LAYER_NAV:
            oled_write("  8  ", false);
            render_space();
            oled_write_ln(" Nav ", false);
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

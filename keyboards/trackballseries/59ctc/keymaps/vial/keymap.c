
#include QMK_KEYBOARD_H
#include "transactions.h"

enum charybdis_keymap_layers {
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

// #define LOWER MO(LAYER_LOWER)
// #define RAISE MO(LAYER_RAISE)
// #define PT_Z LT(LAYER_POINTER, KC_Z)
// #define PT_SLSH LT(LAYER_POINTER, KC_SLSH)

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE

typedef union {
    uint32_t raw;
    struct {
        bool is_oled_enabled : 1;
        bool is_oled_display_info : 1;
        bool is_backlight_enabled : 1;
    };
} auto_config_t;
// 2的数字范围是从0到3， 4是16位，5是0-31位，6是即0到63。
static auto_config_t user_config;

bool need_sync = true;

void data_sync_handler(uint8_t in_buflen, const void *in_data, uint8_t out_buflen, void *out_data) {
    memcpy(&user_config, in_data, in_buflen);
}

void keyboard_post_init_user(void) {
    user_config.raw = eeconfig_read_user();

    transaction_register_rpc(RPC_DATA_SYNC, data_sync_handler);
}

void eeconfig_init_user(void) {
    user_config.raw                  = 0;
    user_config.is_oled_enabled      = 0;
    user_config.is_oled_display_info = 1;
    user_config.is_backlight_enabled = 0;
    eeconfig_update_user(user_config.raw);
}

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT(
  // ╭──────────────────────────────────────────────────────╮               ╭──────────────────────────────────────────────────────╮
        KC_ESC,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_MINS,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
        KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_BSLS,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       KC_LCTL,    KC_SPC,    KC_X,    KC_C,    KC_V,    KC_B,   KC_B,  KC_N,      KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SPC, KC_RSFT,
  // ╰──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────╯
       KC_LGUI,   KC_SPC,   XXXXXXX,   KC_LALT,                                                                      KC_RALT, KC_PSCR,
                                             KC_LALT, KC_BSPC, KC_LALT,          KC_SPC,  KC_DEL
  //                            ╰───────────────────────────╯               ╰──────────────────╯
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
        KC_F12,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                    KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       KC_MNXT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLU,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       KC_MPLY, KC_LEFT,   KC_UP, KC_DOWN, KC_RGHT, XXXXXXX,                  XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, KC_MUTE,
  // ├──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────┤
       KC_MPRV, KC_HOME, KC_PGUP, KC_PGDN,  KC_END, XXXXXXX,   KC_B,   KC_N,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLD,
  // ╰──────────────────────────────────────────────────────┤               ├──────────────────────────────────────────────────────╯
      KC_LGUI,   KC_SPC,   XXXXXXX,   KC_LALT,                                                                      KC_SPC, KC_LALT,
                                            KC_LALT, KC_BSPC, KC_LALT,        KC_SPC, KC_DEL
 //                            ╰───────────────────────────╯               ╰──────────────────╯
  ),

  [LAYER_MOUSE_QWERTY] = LAYOUT(
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
      KC_LGUI,   KC_SPC,   XXXXXXX,   KC_LALT,                                                                      KC_SPC, KC_LALT,
                                            KC_LALT, KC_BSPC, KC_LALT,        KC_BTN1, KC_BTN2
 //                            ╰───────────────────────────╯               ╰──────────────────╯
  ),
};

// clang-format on
// 用来检测自动切换鼠标层的地方，如果鼠标的水平位移x或垂直位移y的绝对值大于设定的阈值
// 自动切换到LAYER_POINTER鼠标层，并将rgb改为绿色
// 如果没有开启这个不起作用
#ifdef POINTING_DEVICE_ENABLE

#endif // POINTING_DEVICE_ENABLE

// 和rgb有关可以不用管
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

    if (!user_config.is_backlight_enabled) {
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
            lightMode               = HIGHLIGHT_PER_INDEX;
            hsv_highlight           = (hsv_t){HSV_SPRINGGREEN};
            int highlightIndexes2[] = {55};
            highlightIndexesSize    = 1;
            memcpy(highlightIndexes, highlightIndexes2, highlightIndexesSize * sizeof(int));
            break;
        case LAYER_MOUSE_QWERTY:
            lightMode               = HIGHLIGHT_PER_INDEX;
            hsv_highlight           = (hsv_t){HSV_SPRINGGREEN};
            int highlightIndexes3[] = {54};
            highlightIndexesSize    = 1;
            memcpy(highlightIndexes, highlightIndexes3, highlightIndexesSize * sizeof(int));
            break;
        case LAYER_NUMROW:
            lightMode               = HIGHLIGHT_PER_INDEX;
            hsv_base                = (hsv_t){HSV_OFF};
            hsv_highlight           = (hsv_t){HSV_GOLD};
            int highlightIndexes4[] = {7, 12, 17, 22, 27, 39, 44, 47, 52, 57};
            highlightIndexesSize    = 10;
            memcpy(highlightIndexes, highlightIndexes4, highlightIndexesSize * sizeof(int));
            break;
        case LAYER_F_KEYS:
            lightMode               = HIGHLIGHT_PER_INDEX;
            hsv_base                = (hsv_t){HSV_OFF};
            hsv_highlight           = (hsv_t){HSV_RED};
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
            int highlightIndexes7[] = {43, 48, 51, 44, 47, 52, 45, 46, 53, 56};
            highlightIndexesSize    = 10;
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
            hsv_highlight = (hsv_t){HSV_SPRINGGREEN};
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

#endif

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
        case LAYER_NUMPAD:
            // turn on numlock, if it isn't already on.
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

// 自定义键值
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case OLED_TOG:
            if (record->event.pressed) { // key down
                user_config.is_oled_enabled ^= 1;
                need_sync = true;
                eeconfig_update_user(user_config.raw);
            }
            return false;
        case OLED_INFO_TOG:
            if (record->event.pressed) { // key down
                user_config.is_oled_display_info ^= 1;
                need_sync = true;
            }
            return false;
        case BACKLIGHT_TOG:
            if (record->event.pressed) { // key down
                user_config.is_backlight_enabled ^= 1;
                need_sync = true;
                eeconfig_update_user(user_config.raw);
            }
            return false;
    }
    return true;
}

/* oled stuff :) */
#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return rotation;
    } else {
        return OLED_ROTATION_270;
    }
}

void render_space(void) {
    oled_write_P(PSTR("     "), false);
}

void render_logo(void) {
    oled_write_P(PSTR("Key  "), false);
    oled_write_P(PSTR("ball "), false);
    oled_write_P(PSTR("59   "), false);
}

void render_mod_status_gui_alt(uint8_t modifiers) {
    static const char PROGMEM gui_off_1[] = {0x85, 0x86, 0};
    static const char PROGMEM gui_off_2[] = {0xa5, 0xa6, 0};
    static const char PROGMEM gui_on_1[]  = {0x8d, 0x8e, 0};
    static const char PROGMEM gui_on_2[]  = {0xad, 0xae, 0};

    static const char PROGMEM alt_off_1[] = {0x87, 0x88, 0};
    static const char PROGMEM alt_off_2[] = {0xa7, 0xa8, 0};
    static const char PROGMEM alt_on_1[]  = {0x8f, 0x90, 0};
    static const char PROGMEM alt_on_2[]  = {0xaf, 0xb0, 0};

    // fillers between the modifier icons bleed into the icon frames
    static const char PROGMEM off_off_1[] = {0xc5, 0};
    static const char PROGMEM off_off_2[] = {0xc6, 0};
    static const char PROGMEM on_off_1[]  = {0xc7, 0};
    static const char PROGMEM on_off_2[]  = {0xc8, 0};
    static const char PROGMEM off_on_1[]  = {0xc9, 0};
    static const char PROGMEM off_on_2[]  = {0xca, 0};
    static const char PROGMEM on_on_1[]   = {0xcb, 0};
    static const char PROGMEM on_on_2[]   = {0xcc, 0};

    if (modifiers & MOD_MASK_GUI) {
        oled_write_P(gui_on_1, false);
    } else {
        oled_write_P(gui_off_1, false);
    }

    if ((modifiers & MOD_MASK_GUI) && (modifiers & MOD_MASK_ALT)) {
        oled_write_P(on_on_1, false);
    } else if (modifiers & MOD_MASK_GUI) {
        oled_write_P(on_off_1, false);
    } else if (modifiers & MOD_MASK_ALT) {
        oled_write_P(off_on_1, false);
    } else {
        oled_write_P(off_off_1, false);
    }

    if (modifiers & MOD_MASK_ALT) {
        oled_write_P(alt_on_1, false);
    } else {
        oled_write_P(alt_off_1, false);
    }

    if (modifiers & MOD_MASK_GUI) {
        oled_write_P(gui_on_2, false);
    } else {
        oled_write_P(gui_off_2, false);
    }

    if (modifiers & MOD_MASK_GUI & MOD_MASK_ALT) {
        oled_write_P(on_on_2, false);
    } else if (modifiers & MOD_MASK_GUI) {
        oled_write_P(on_off_2, false);
    } else if (modifiers & MOD_MASK_ALT) {
        oled_write_P(off_on_2, false);
    } else {
        oled_write_P(off_off_2, false);
    }

    if (modifiers & MOD_MASK_ALT) {
        oled_write_P(alt_on_2, false);
    } else {
        oled_write_P(alt_off_2, false);
    }
}

void render_mod_status_ctrl_shift(uint8_t modifiers) {
    static const char PROGMEM ctrl_off_1[] = {0x89, 0x8a, 0};
    static const char PROGMEM ctrl_off_2[] = {0xa9, 0xaa, 0};
    static const char PROGMEM ctrl_on_1[]  = {0x91, 0x92, 0};
    static const char PROGMEM ctrl_on_2[]  = {0xb1, 0xb2, 0};

    static const char PROGMEM shift_off_1[] = {0x8b, 0x8c, 0};
    static const char PROGMEM shift_off_2[] = {0xab, 0xac, 0};
    static const char PROGMEM shift_on_1[]  = {0xcd, 0xce, 0};
    static const char PROGMEM shift_on_2[]  = {0xcf, 0xd0, 0};

    // fillers between the modifier icons bleed into the icon frames
    static const char PROGMEM off_off_1[] = {0xc5, 0};
    static const char PROGMEM off_off_2[] = {0xc6, 0};
    static const char PROGMEM on_off_1[]  = {0xc7, 0};
    static const char PROGMEM on_off_2[]  = {0xc8, 0};
    static const char PROGMEM off_on_1[]  = {0xc9, 0};
    static const char PROGMEM off_on_2[]  = {0xca, 0};
    static const char PROGMEM on_on_1[]   = {0xcb, 0};
    static const char PROGMEM on_on_2[]   = {0xcc, 0};

    if (modifiers & MOD_MASK_CTRL) {
        oled_write_P(ctrl_on_1, false);
    } else {
        oled_write_P(ctrl_off_1, false);
    }

    if ((modifiers & MOD_MASK_CTRL) && (modifiers & MOD_MASK_SHIFT)) {
        oled_write_P(on_on_1, false);
    } else if (modifiers & MOD_MASK_CTRL) {
        oled_write_P(on_off_1, false);
    } else if (modifiers & MOD_MASK_SHIFT) {
        oled_write_P(off_on_1, false);
    } else {
        oled_write_P(off_off_1, false);
    }

    if (modifiers & MOD_MASK_SHIFT) {
        oled_write_P(shift_on_1, false);
    } else {
        oled_write_P(shift_off_1, false);
    }

    if (modifiers & MOD_MASK_CTRL) {
        oled_write_P(ctrl_on_2, false);
    } else {
        oled_write_P(ctrl_off_2, false);
    }

    if (modifiers & MOD_MASK_CTRL & MOD_MASK_SHIFT) {
        oled_write_P(on_on_2, false);
    } else if (modifiers & MOD_MASK_CTRL) {
        oled_write_P(on_off_2, false);
    } else if (modifiers & MOD_MASK_SHIFT) {
        oled_write_P(off_on_2, false);
    } else {
        oled_write_P(off_off_2, false);
    }

    if (modifiers & MOD_MASK_SHIFT) {
        oled_write_P(shift_on_2, false);
    } else {
        oled_write_P(shift_off_2, false);
    }
}

// 从设备
static void slave_data(void) {
#    if OLED_TIMEOUT > 0
    if (last_input_activity_elapsed() > OLED_TIMEOUT) {
        //        oled_clear();
        oled_off();
        return;
    } else {
        oled_on();
    }
#    endif

    render_logo();
    oled_set_cursor(0, 4);

    render_mod_status_gui_alt(get_mods());
    render_mod_status_ctrl_shift(get_mods());

    render_space();

    /* Print current layer */
    switch (get_highest_layer(layer_state)) {
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
        default:
            oled_write("Undef", false);
    }
}

static void trackball_dpis(void) {
#    if OLED_TIMEOUT > 0
    if (last_input_activity_elapsed() > OLED_TIMEOUT) {
        //        oled_clear();
        oled_off();
        return;
    } else {
        oled_on();
    }
#    endif

    oled_clear();
    trackball_oled_default();
    trackball_oled_info();
}

// 主设备OLED
static void master_data(void) {
    trackball_dpis();
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        if (!user_config.is_oled_enabled) {
            // oled_clear();
            oled_off();
        } else {
            oled_on();

            if (is_oled_on()) {
                master_data();
            }
        }
    } else {
        if (is_oled_on()) {
            slave_data();
        }
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

bool shutdown_user(bool jump_to_bootloader) {
    oled_render_boot(jump_to_bootloader);
}

#endif

void housekeeping_task_user(void) {
    if (is_keyboard_master()) {
        if (need_sync) {
            // Interact with slave every 500ms
            static uint32_t last_sync = 0;
            if (timer_elapsed32(last_sync) > 500) {
                if (transaction_rpc_send(RPC_DATA_SYNC, sizeof(user_config), &user_config)) {
                    last_sync = timer_read32();
                    need_sync = false;
                }
            }
        }
    }
}

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
};

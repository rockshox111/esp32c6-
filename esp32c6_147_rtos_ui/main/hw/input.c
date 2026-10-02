#include "input.h"
#include "board_pin.h"
#include "driver/gpio.h"
#include "freertos/queue.h"

static QueueHandle_t key_q;
typedef struct { uint32_t key; bool press; } key_evt_t;

static void IRAM_ATTR gpio_isr(void *arg)
{
    uint32_t io = (uint32_t)(uintptr_t)arg;
    key_evt_t e = { .key = io, .press = true };
    xQueueSendFromISR(key_q, &e, NULL);
}

static lv_key_t map_gpio(uint32_t io)
{
    if (io == PIN_KEY_UP)   return LV_KEY_PREV;
    if (io == PIN_KEY_DOWN) return LV_KEY_NEXT;
    if (io == PIN_KEY_OK)   return LV_KEY_ENTER;
    if (io == PIN_KEY_BACK) return LV_KEY_ESC;
    return 0;
}

static lv_key_t g_key = 0;
static bool g_press = false;

static void indev_read(lv_indev_t *indev, lv_indev_data_t *data)
{
    key_evt_t e;
    while (xQueueReceive(key_q, &e, 0) == pdTRUE) {
        lv_key_t k = map_gpio(e.key);
        if (k) { g_key = k; g_press = e.press; }
    }
    data->key = g_key;
    data->state = g_press ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
    if (g_press) g_press = false;
}

void input_init(lv_group_t **out_group)
{
    key_q = xQueueCreate(16, sizeof(key_evt_t));
    gpio_config_t cfg = { .mode = GPIO_MODE_INPUT, .pull_up_en = 1, .intr_type = GPIO_INTR_NEGEDGE };
    uint32_t pins[] = { PIN_KEY_OK, PIN_KEY_UP, PIN_KEY_DOWN, PIN_KEY_BACK };
    for (int i = 0; i < 4; i++) {
        if (pins[i] < 0) continue;
        cfg.pin_bit_mask = 1ULL << pins[i];
        gpio_config(&cfg);
        gpio_install_isr_service(0);
        gpio_isr_handler_add(pins[i], gpio_isr, (void *)(uintptr_t)pins[i]);
    }
    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_KEYPAD);
    lv_indev_set_read_cb(indev, indev_read);
    lv_group_t *g = lv_group_create();
    lv_group_set_default(g);
    lv_indev_set_group(indev, g);
    if (out_group) *out_group = g;
}

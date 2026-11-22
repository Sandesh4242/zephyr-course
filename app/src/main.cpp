#include <zephyr/drivers/led_strip.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <string.h>

/* The devicetree node identifier for the "app-led" alias. */
#define LED_NODE DT_ALIAS(app_led)

static const struct device *strip = DEVICE_DT_GET(LED_NODE);
static struct led_rgb pixel;

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    LOG_INF("Board Initialized");
    bool led_state = true;

    if (!device_is_ready(strip)) return 0;

    while (1) {
        led_state = !led_state;
        memset(&pixel, 0, sizeof(pixel));
        pixel.g = led_state ? 0x20 : 0;
        led_strip_update_rgb(strip, &pixel, 1);
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }
    return 0;
}

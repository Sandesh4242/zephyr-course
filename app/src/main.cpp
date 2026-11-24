#include <zephyr/drivers/led_strip.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <string.h>
#include <zephyr/drivers/sensor.h>

extern "C" {
#include "../drivers/our_driver/our_driver_api.h"
}

#define LED_NODE DT_ALIAS(app_led)

static const struct device *strip = DEVICE_DT_GET(LED_NODE);
static struct led_rgb pixel;

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

namespace {
  void test(){
    const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(our_driver_inst));
    struct sensor_value val;
    int ret;

    /* Call custom extension API to change brightness */
    our_driver_set_brightness(driver, 0x05);

    ret = sensor_sample_fetch(driver);
    LOG_INF("Fetch ret %d", ret);

    ret = sensor_channel_get(driver, SENSOR_CHAN_ALL, &val);
    LOG_INF("Channel ret %d", ret);
  }
}

int main(void)
{
    LOG_INF("Board Initialized");
    test();
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

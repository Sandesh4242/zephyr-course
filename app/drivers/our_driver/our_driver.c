#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/led_strip.h>
#include <string.h>
#include <zephyr/logging/log.h>
#include "our_driver_api.h"

#define DT_DRV_COMPAT our_driver

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

static const struct device *led_strip = DEVICE_DT_GET(DT_ALIAS(app_led));

/* Dynamic data struct */
struct our_driver_data {
    uint8_t brightness;
};

static struct our_driver_data driver_data = {
    .brightness = 0x20,  /* default brightness */
};

/* Custom extension API function */
int our_driver_set_brightness(const struct device *dev, uint8_t brightness)
{
    struct our_driver_data *data = dev->data;
    data->brightness = brightness;
    LOG_INF("Brightness set to 0x%02x", brightness);
    return 0;
}

static int sample_fetch_impl(const struct device *dev,
                             enum sensor_channel chan)
{
    struct our_driver_data *data = dev->data;
    struct led_rgb pixel = {0};
    pixel.g = data->brightness;
    led_strip_update_rgb(led_strip, &pixel, 1);
    LOG_INF("sample_fetch: LED ON (brightness 0x%02x)", data->brightness);
    return 0;
}

static int channel_get_impl(const struct device *dev,
                            enum sensor_channel chan,
                            struct sensor_value *val)
{
    struct led_rgb pixel = {0};
    led_strip_update_rgb(led_strip, &pixel, 1);
    val->val1 = 0;
    val->val2 = 0;
    LOG_INF("channel_get: LED OFF");
    return 0;
}

static DEVICE_API(sensor, api_iomico_lecture) = {
    .sample_fetch = sample_fetch_impl,
    .channel_get = channel_get_impl,
};

static int init(const struct device *dev)
{
    LOG_INF("Device Initialized!");
    return 0;
}

DEVICE_DT_INST_DEFINE(0, init, NULL, &driver_data, NULL, POST_KERNEL, 90, &api_iomico_lecture);

#ifndef OUR_DRIVER_API_H
#define OUR_DRIVER_API_H

#include <zephyr/device.h>

int our_driver_set_brightness(const struct device *dev, uint8_t brightness);

#endif

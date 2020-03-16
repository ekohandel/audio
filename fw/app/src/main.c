#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>

#define LED0  DT_ALIAS_LED0_GPIOS_CONTROLLER
#define PIN   DT_ALIAS_LED0_GPIOS_PIN
#define FLAGS DT_ALIAS_LED0_GPIOS_FLAGS

void main(void)
{
	struct device *dev;
	int ret;

	dev = device_get_binding(LED0);
	if (dev == NULL) {
		return;
	}

	ret = gpio_pin_configure(dev, PIN, GPIO_OUTPUT_ACTIVE | FLAGS);
	if (ret < 0) {
		return;
	}

	while (1) {
		gpio_pin_set(dev, PIN, !gpio_pin_get(dev, PIN));
		k_sleep(1000);
	}
}

#include "esp_system.h"
#include "driver/gpio.h"

void ConfigureLEDs();

#define GPIO_LED5    21
#define GPIO_LED6    22
#define GPIO_LED7    23

#define LED5_ON() { gpio_set_level(GPIO_LED5, 0); }
#define LED6_ON() { gpio_set_level(GPIO_LED6, 0); }
#define LED7_ON() { gpio_set_level(GPIO_LED7, 0); }

#define LED5_OFF() { gpio_set_level(GPIO_LED5, 1); }
#define LED6_OFF() { gpio_set_level(GPIO_LED6, 1); }
#define LED7_OFF() { gpio_set_level(GPIO_LED7, 1); }
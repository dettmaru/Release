#include "LEDs.h"

void ConfigureLEDs()
{
    gpio_config_t ioConfig = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1<<GPIO_LED5) | (1<<GPIO_LED6) | (1<<GPIO_LED7)
    };
    gpio_config(&ioConfig);

    LED5_OFF();
    LED6_OFF();
    LED7_OFF();
}


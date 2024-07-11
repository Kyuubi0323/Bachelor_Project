#include "sdk.h"
#include "gpio.h"

#define DEBOUNCE_DEF    5

static Gpio_p gpioProc = {
		Gpio_getState,
		Gpio_setState
};

Gpio_t* Gpio_create(Gpio_t* init, int pin) {
	if (init != NULL)  {
		init->pin = pin;
		init->proc = &gpioProc;
	}
	return init;
}

bool Gpio_getState(Gpio_t* gpio) {
	return (sAPI_GpioGetValue(gpio->pin) > 0) ? true : false;
}

void Gpio_setState(Gpio_t* gpio, bool state) {
	sAPI_GpioSetValue(gpio->pin, state);
}

void Gpio_config(Gpio_t* gpio, int mode, int pull, int level) {
	SC_GPIOConfiguration config;
    config.initLv = level;
    config.pinDir = (SC_GPIOPinDirection)mode;
	config.pinPull = (SC_GPIOPullUpDown)pull;
    config.pinEd = SC_GPIO_NO_EDGE;
    config.wu = NULL;
	config.isr =  NULL;
	sAPI_GpioConfig(gpio->pin, config);
}

void registerIOIntrEvents(Gpio_t* gpio, int trigger, void(*callback)(void)) {
	sAPI_GpioConfigInterrupt(gpio->pin, (SC_GPIOTransitionType)trigger, callback);
	sAPI_GpioWakeupEnable(gpio->pin, (SC_GPIOTransitionType)trigger);
}

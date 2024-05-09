#ifndef __IO_H__
#define __IO_H__

typedef struct Gpio_t Gpio_t;

typedef struct Gpio_p {
	bool(*get_state)(Gpio_t*);
	void(*set_state)(Gpio_t*, bool);
} Gpio_p;

struct Gpio_t {
	const Gpio_p* proc;
	int pin;
};

/*
* pin: SC_Module_GPIONumbers type
* mode: SC_GPIOPinDirection type
* level: SC_GPIOReturnCode type
* pull: SC_GPIOPullUpDown type
* trigger: SC_GPIOTransitionType type
* callback: GPIOCallback
*/

Gpio_t* Gpio_create(Gpio_t* init, int pin);
bool Gpio_getState(Gpio_t* gpio);
void Gpio_setState(Gpio_t* gpio, bool state);
void Gpio_config(Gpio_t* gpio, int mode, int pull, int level);
void registerIOIntrEvents(Gpio_t* gpio,  int trigger, void(*callback)(void));

#endif

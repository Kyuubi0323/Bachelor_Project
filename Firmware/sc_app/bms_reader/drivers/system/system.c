#include "sdk.h"
#include "system.h"

void initSystem(void) {
}

void reboot(void) {
    sAPI_SysReset();
}

uint32_t getSysTickCount(void) {
    return sAPI_GetTicks() * 5;
}

void initHardWdt(void) {
    SC_GPIOConfiguration pinConfig;
    pinConfig.initLv = 0;
    pinConfig.pinDir = SC_GPIO_OUT_PIN;
    pinConfig.pinPull = SC_GPIO_PULLUP_ENABLE;
    sAPI_GpioConfig(SC_MODULE_GPIO_14, pinConfig);
}

void refreshHardWdt(void) {
    static bool state = false;
    state = !state;
    sAPI_GpioSetValue(SC_MODULE_GPIO_14, state);

    // sAPI_GpioSetValue(SC_MODULE_GPIO_18, 1);
    // sAPI_TaskSleep(20);
    // sAPI_GpioSetValue(SC_MODULE_GPIO_18, 0);

    // sAPI_PWMConfig(SC_PWM_DEVICE_2, PWM_ON, 1, 7, 14);
    // sAPI_TaskSleep(2);
    // sAPI_PWMConfig(SC_PWM_DEVICE_2, PWM_OFF, 1, 7, 14);
}

void activateWatchdog(void) {
    //eat_watchdog_start(27000, 0); //reboot after 27s
}

void refreshWatchdog(void) {
    //eat_watchdog_feed();
}
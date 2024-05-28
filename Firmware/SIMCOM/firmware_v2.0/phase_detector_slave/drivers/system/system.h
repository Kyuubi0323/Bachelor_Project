#ifndef __SYSTEM_H__
#define __SYSTEM_H__

void initSystem(void);
void reboot(void);
void initHardWdt(void);
void refreshHardWdt(void);
void activateWatchdog(void);
void refreshWatchdog(void);
uint32_t getSysTickCount(void);

#endif /* __SYSTEM_H__ */
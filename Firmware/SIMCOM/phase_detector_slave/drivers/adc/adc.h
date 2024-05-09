#ifndef __ADC_H__
#define __ADC_H__

#define ADC_CHANNEL     0

#define readVoltage()  (sAPI_ReadAdc(ADC_CHANNEL) * 11)
#define readVbat()     (sAPI_ReadVbat())

void initAdc(void);
bool startAdcPeriodic(int period);
void registerAdcDataCallback(void(*callback)(int value));

#endif //__ADC_H__
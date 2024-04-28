#ifndef _SIMCOM_DSPGAIN_H
#define _SIMCOM_DSPGAIN_H

typedef struct 
{
    unsigned char resmode;
    unsigned int dtTresh;
    short gamma;
    short nonlinear;
    int noiseFloorDbov;
    unsigned char txBoostMode;
    unsigned char txBoostGainDb;
    char txPeakGainDb;
    char txBoostNoiseGainDb;
    unsigned char txAgcMaxGainDb;
    unsigned char rxBoostMode;
    unsigned char rxBoostGainDb;
    char rxPeakGainDb;
    char rxBoostNoiseGainDb;
    unsigned char rxAgcMaxGainDb;
    unsigned char DAC_DigGain;
    unsigned char DAC_Gain;
    unsigned char RCV_Gain;
    unsigned char PGA_Stage1;
    unsigned char PGA_Stage2;
    unsigned char ADC_DIgGain;
    unsigned char sampleDelay;
    unsigned char numOfTaps;
    unsigned char convergenceSpeed;
    char ref2EchoPowerRatio;
}SC_Dspconfiguration;

#endif
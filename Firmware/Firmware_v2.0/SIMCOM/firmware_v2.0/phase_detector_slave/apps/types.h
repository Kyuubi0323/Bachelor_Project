#ifndef _TYPE_H_
#define _TYPE_H_

#include "sdk.h"

#define MAX_CELL_NUMBER 20
typedef struct {
    BOOL state;
    char id[64];
    UINT16 vol;
    UINT16 soc;
    UINT16 cells;
    UINT16 cap;
    UINT16 i_out;
    UINT16 envTemp;
    UINT16 boardTemp;
    UINT16 cellsTemp;  
    UINT16 cellsVol[MAX_CELL_NUMBER];  
} bmsInfo_t; 

typedef struct {
    UINT32 ts;
    UINT16 l1;
    UINT16 l2;
    UINT16 l3;
    BOOL seq;       //seq = 0 -> ABC seq = 1 -> ACB
    char ret;
} phaseData_t;

extern phaseData_t phaseData;
extern bmsInfo_t bmsInfo;
#endif
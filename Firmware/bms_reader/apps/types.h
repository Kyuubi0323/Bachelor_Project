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

extern bmsInfo_t bmsInfo;
#endif
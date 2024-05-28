#ifndef _ATC_H
#define _ATC_H

#include "stdint.h"

typedef struct embedded_at_t embedded_at_t;

typedef struct embedded_at_p {
	int(*write_data)(uint8_t*, int);
	int(*read_data)(uint8_t*, int);
	int(*read_data_block)(uint8_t*, int);
    void(*flush)(void);
    int (*get_data_rdy)(void);
    void(*restart)(void);
} embedded_at_p;

struct embedded_at_t {
	const embedded_at_p * proc;
};

embedded_at_t* embedded_at_create(void); //for simcom
int embedded_at_write(uint8_t* data, int size);
int embedded_at_read(uint8_t* data, int size);
int embedded_at_read_block(uint8_t* data, int size);
void embedded_at_flush(void);
int embedded_at_get_data_rdy(void);
void embedded_at_restart(void);
void register_eat_write_event(int(*callback)(uint8_t* data, int size));
void register_eat_read_event(int(*callback)(uint8_t* data, int size));
void register_eat_read_block_event(int(*callback)(uint8_t* data, int size));
void register_eat_flush_event(void(*callback)(void));
void register_eat_get_data_rdy_event(int(*callback)(void));
void register_eat_restart_event(void(*callback)(void));
#endif

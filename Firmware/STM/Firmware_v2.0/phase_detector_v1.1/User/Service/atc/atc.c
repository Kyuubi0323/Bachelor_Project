/**
 * @file embedded_at.c
 * @author Phan Ngoc Vinh (phanngocvinh96@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-05-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "stdio.h"
#include "atc.h"

#define EAT_MDM_RX_BUF_LEN_MAX 2000

static embedded_at_t mbed_at = { 0 };
static int(*eat_write_event)(uint8_t* data, int size);
static int(*eat_read_event)(uint8_t* data, int size);
static int(*eat_read_block_event)(uint8_t* data, int size);
static void (*eat_flush_event)(void);
static int (*eat_get_data_rdy_event)(void);
static void (*eat_restart_event)(void);
static embedded_at_p mbed_atProc = {
    embedded_at_write,
    embedded_at_read,
    embedded_at_read_block,
    embedded_at_flush,
	embedded_at_get_data_rdy,
	embedded_at_restart,
};

embedded_at_t* embedded_at_create(void) {
    if ((eat_write_event != NULL) && (eat_read_event != NULL)) {
        mbed_atProc.write_data = eat_write_event;
        mbed_atProc.read_data = eat_read_event;
        mbed_atProc.flush = eat_flush_event;
        mbed_atProc.read_data_block = eat_read_block_event;
        mbed_at.proc = &mbed_atProc;
	    return &mbed_at;
    }
    return NULL;
}
void embedded_at_flush(void)
{
    if (eat_write_event != NULL)
        eat_flush_event();
}
int embedded_at_get_data_rdy(void)
{
    if (eat_get_data_rdy_event != NULL)
        return eat_get_data_rdy_event();
    return -1;
}

void embedded_at_restart(void)
{
    if (eat_restart_event != NULL)
        eat_restart_event();
}
int embedded_at_write(uint8_t* data, int size) {
    if (eat_write_event != NULL)
        return eat_write_event(data, size);
    return -1;
}

int embedded_at_read(uint8_t* data, int size) {
    if (eat_read_event != NULL)
        return eat_read_event(data, size);
    return -1;
}
int embedded_at_read_block(uint8_t* data, int size) {
    if (eat_read_block_event != NULL)
        return eat_read_block_event(data, size);
    return -1;
}
void register_eat_write_event(int(*callback)(uint8_t* data, int size)) {
    eat_write_event = callback;
}

void register_eat_read_event(int(*callback)(uint8_t* data, int size)) {
    eat_read_event = callback;
}

void register_eat_read_block_event(int(*callback)(uint8_t* data, int size)) {
    eat_read_block_event = callback;
}

void register_eat_flush_event(void(*callback)(void)) {
    eat_flush_event = callback;
}
void register_eat_get_data_rdy_event(int(*callback)(void))
{
	eat_get_data_rdy_event = callback;
}
void register_eat_restart_event(void(*callback)(void)) {
	eat_restart_event = callback;
}

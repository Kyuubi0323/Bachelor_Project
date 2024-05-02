/*
 * utils.h
 *
 *  Created on: 1 thg 9, 2020
 *      Author: E7470
 */

#ifndef SERVICES_UTILS_H_
#define SERVICES_UTILS_H_

#define IP2STR(_ip)		(uint8_t)((_ip) >> 24),(uint8_t)((_ip) >> 16),(uint8_t)((_ip) >> 8),(uint8_t)(_ip)

int u_atoi(char* str);
double u_atof(char* str);
uint32_t u_ip2addr(char* ip);
uint32_t u_mktime(struct tm* time);
struct tm* u_gmtime(const time_t* ts);
int split_string(char* source, char* dest, int start, int src_size, int dest_size, char delim);

#endif /* SERVICES_UTILS_H_ */

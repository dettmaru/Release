/*
 * sntp_time.h
 *
 *  Created on: Okt 5, 2018
 *      Author: Daniel Lohmann
 */
#ifndef SNTP_TIME_H
#define SNTP_TIME_H

#include <time.h>
#include <stdbool.h>

time_t GetTime(char* timeServer, bool* synced);

#endif // SNTP_TIME_H
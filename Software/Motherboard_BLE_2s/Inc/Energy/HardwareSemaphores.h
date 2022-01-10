/**
 * @brief Hardware energy management (turn on and off)
 *        Contais a list of HardwareSemaphores
 * 
 * @file HardwareSemaphores.h
 * @author Daniel Lohmann
 * @date 2019-05-08
 */
#ifndef __HARDWARESEMAPHORES_H_
#define __HARDWARESEMAPHORES_H_

#include "HardwareSemaphore.h"
#include "main.h"

extern HardwareSemaphore soilSensors;
extern HardwareSemaphore airSensors;
extern HardwareSemaphore sdi12hardware;
extern HardwareSemaphore uart1;

#endif

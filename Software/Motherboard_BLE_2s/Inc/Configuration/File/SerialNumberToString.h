/**
 * @brief SerialNumber to string handler
 * 
 * @file SerialNumberToString.h
 * @author Daniel Lohmann
 * @date 2019-03-21
 */
#ifndef __SERIALNUMBER_TO_STRING_H_
#define __SERIALNUMBER_TO_STRING_H_

#include "Motherboard.h"

char* GetSerialNumberString(const SerialNumber_t serialNumber);

#endif /* __SERIALNUMBER_TO_STRING_H_ */
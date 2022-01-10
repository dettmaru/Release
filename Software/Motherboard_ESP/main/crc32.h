
#ifndef __CRC32_H_
#define __CRC32_H_

#include "string.h"

void CCRC32_Initialize(void);
unsigned long CCRC32_Reflect(unsigned long iReflect, const char cChar);
void CCRC32_PartialCRC(unsigned long *iCRC, const unsigned char *sData, size_t iDataLength);
unsigned long CCRC32_FullCRC(const unsigned char *sData, size_t iDataLength);

#endif /* __CRC32_H_ */
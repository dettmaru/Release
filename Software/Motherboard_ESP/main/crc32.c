#include "crc32.h"

unsigned long iTable[256]; // CRC lookup table array.
/*
	This function initializes "CRC Lookup Table". You only need to call it once to
		initialize the table before using any of the other CRC32 calculation functions.
*/
void CCRC32_Initialize(void)
{
	//0x04C11DB7 is the official polynomial used by PKZip, WinZip and Ethernet.
	unsigned long iPolynomial = 0x04C11DB7;

	memset(iTable, 0, sizeof(iTable));

	// 256 values representing ASCII character codes.
	for(int iCodes = 0; iCodes <= 0xFF; iCodes++)
	{
		iTable[iCodes] = CCRC32_Reflect(iCodes, 8) << 24;

		for(int iPos = 0; iPos < 8; iPos++)
		{
			iTable[iCodes] = (iTable[iCodes] << 1)
				^ ((iTable[iCodes] & (1 << 31)) ? iPolynomial : 0);
		}

		iTable[iCodes] = CCRC32_Reflect(iTable[iCodes], 32);
	}
}

/*
	Reflection is a requirement for the official CRC-32 standard.
	You can create CRCs without it, but they won't conform to the standard.
*/
unsigned long CCRC32_Reflect(unsigned long iReflect, const char cChar)
{
	unsigned long iValue = 0;

	// Swap bit 0 for bit 7, bit 1 For bit 6, etc....
	for(long iPos = 1; iPos < (cChar + 1); iPos++)
	{
		if(iReflect & 1)
		{
			iValue |= (1 << (cChar - iPos));
		}
		iReflect >>= 1;
	}

	return iValue;
}

/*
	Calculates the CRC32 by looping through each of the bytes in sData.
	
	Note: For Example usage example, see FileCRC().
*/
void CCRC32_PartialCRC(unsigned long *iCRC, const unsigned char *sData, size_t iDataLength)
{
	while(iDataLength--)
	{
		//If your compiler complains about the following line, try changing
		//	each occurrence of *iCRC with ((unsigned long)*iCRC).

		*iCRC = (*iCRC >> 8) ^ iTable[(*iCRC & 0xFF) ^ *sData++];
	}
}

/*
	Returns the calculated CRC23 for the given string.
*/
unsigned long CCRC32_FullCRC(const unsigned char *sData, size_t iDataLength)
{
    unsigned long iCRC = 0xffffffff; //Initialize the CRC.

	CCRC32_PartialCRC(&iCRC, sData, iDataLength);

	return(iCRC ^ 0xffffffff); //Finalize the CRC and return.
}




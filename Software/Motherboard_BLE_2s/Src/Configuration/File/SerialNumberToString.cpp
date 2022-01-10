/**
 * @brief SerialNumber to string handler
 * 
 * @file SerialNumberToString.h
 * @author Daniel Lohmann
 * @date 2019-03-21
 */
#include "SerialNumberToString.h"
#include "string.h"
#include "StringHelper.h"
#include "math.h"
#include <bitset>

#include <stdio.h>

void add(char* a, char* b)
{
    int carry = 0;
    int length = strlen(b);
    int i = 0;
    for (i = 0; i < length; i++)
    {
        int add = (a[i]-'0') + (b[i]-'0') + carry;
        carry = add / 10;
        a[i] = (add % 10) + '0';
    }

    while (carry)
    {
        i += 1;
        int add = (a[i]-'0') + carry;
        carry = add / 10;
        a[i] = (add % 10) + '0';
    }
}

char* GetSerialNumberString(const SerialNumber_t serialNumber)
{
    std::bitset<96> bits;
    for (int i = 0; i < 32; i++)
    {
        //bits.set(i,    (serialNumber.low  & (1<<i)) != 0);
        //bits.set(i+32, (serialNumber.mid  & (1<<i)) != 0);
        //bits.set(i+64, (serialNumber.high & (1<<i)) != 0);
        bits.set(95-i, (serialNumber.low & (1<<i)) != 0);
        bits.set(63-i, (serialNumber.mid & (1<<i)) != 0);
        bits.set(31-i, (serialNumber.high & (1<<i)) != 0);
    }
    int length = (96.0 * log(2) / log(10)) + 1;
    char* buffer = (char*)calloc(length + 1, sizeof(char));
    for (int i = 0; i < length; i++)
    {
        buffer[i] = '0';
    }

    for (int i = 0; i < 96; i++)
    {
        add(buffer, buffer);
        if (bits[i] == true)
        {
            add(buffer, "1");
        }
    }

    strreverse(buffer);
    return buffer;
}

//https://stackoverflow.com/questions/5246976/binary-string-to-decimal-string


/*



You can use an algorithm like:

// X is the input
while ( X != "0" )
    compute X' and R such that X = 10 * X' + R  (Euclidean division, see below)
    output R    // least significant decimal digit first
    X = X'

The Euclidean division of X by 10 is computed like this:

R = 0  // remainder in 0..9
X' = ""
for (b in bits of X)  // msb to lsb
    R = 2*R + b
    if R >= 10
        X' += "1"
        R -= 10
    else
        X' += "0"

Remove leading "0" from X'
The remainder is R in 0..9

*/

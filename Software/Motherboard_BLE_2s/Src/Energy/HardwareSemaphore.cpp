/**
 * @brief Hardware energy management (turn on and off)
 * 
 * @file energy.c
 * @author Daniel Lohmann
 * @date 2019-05-08
 */
#include "HardwareSemaphore.h"
#include "stdlib.h"

HardwareSemaphore::HardwareSemaphore(
    void (*turnONfunc)(),
    void (*turnOFFfunc)()
)
{
    this->turnONfunc = turnONfunc;
    this->turnOFFfunc = turnOFFfunc;
    counter = 0;
}

void HardwareSemaphore::Request()
{
    counter++;
    if (counter == 1)
    {
        if (turnONfunc != nullptr && turnONfunc != NULL)
        {
            turnONfunc();
        }
    }
}

void HardwareSemaphore::Free()
{
    if (counter > 0)
    {
        counter--;
        if (counter == 0)
        {
            if (turnOFFfunc != nullptr && turnOFFfunc != NULL)
            {
                turnOFFfunc();
            }
        }
    }
}

void HardwareSemaphore::FreeAll()
{
    counter = 0;
    if (turnOFFfunc != nullptr && turnOFFfunc != NULL)
    {
        turnOFFfunc();
    }
}

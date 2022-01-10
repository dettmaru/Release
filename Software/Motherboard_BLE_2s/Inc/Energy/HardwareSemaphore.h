/**
 * @brief Hardware energy management (turn on and off)
 * 
 * @file HardwareSemaphore.h
 * @author Daniel Lohmann
 * @date 2019-05-08
 */
#ifndef __HARDWARESEMAPHORE_H_
#define __HARDWARESEMAPHORE_H_

class HardwareSemaphore
{
public:
    HardwareSemaphore(
        void (*turnONfunc)(),
        void (*turnOFFfunc)()
    );
    void Request();
    void Free();
    void FreeAll();
private:
    void (*turnONfunc)();
    void (*turnOFFfunc)();
    int counter;
};

#endif /* __HARDWARESEMAPHORE_H_ */
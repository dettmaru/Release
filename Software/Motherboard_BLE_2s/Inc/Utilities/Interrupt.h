/**
 * @brief Interrupt helper functions
 * 
 * @file Interrupt.h
 * @author Daniel Lohmann
 * @date 2019-08-2
 */
#ifndef __INTERRUPT_H_
#define __INTERRUPT_H_

#include "stm32l4xx_hal.h"
#ifndef __cplusplus
#include <stdbool.h>
#endif

/** 
 * @brief Check if isInterrupt is called in an interrupt (true) or not (false)
 */
inline bool isInterrupt()
{
    return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0 ;
}

#endif /* __INTERRUPT_H_ */

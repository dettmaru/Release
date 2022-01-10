#!/bin/sh

~/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI --connect port=SWD mode=UR -ob displ
~/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI --connect port=SWD mode=UR -ob IWDG_STDBY=0x0
~/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI --connect port=SWD mode=UR -ob displ

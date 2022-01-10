#!/bin/sh

~/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI --connect port=SWD --upload 0x08080000 0xFFFF FlashContent.hex
/**
 * @brief Logging of messages in the microcontroller program
 * 
 * @file Logging.h
 * @author Daniel Lohmann
 * @date 2019-07-22
 */
#ifndef __LOGGING_H_
#define __LOGGING_H_

#ifdef __cplusplus
enum class LogSeverity {
    Error=0,
    Info=1,
    InfoSuccess=2,
    InfoWarning=3,
    InfoError=4,
    Warning=5,
    Verbose=6
};
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "main.h"
#include "fatfs.h"
#include "Interrupt.h"

#define FILENAME_LOG "log.txt"

#define LOG_SDCARD true
#define LOG_UART true
#define LOG_USB false
#define LOG_TRANSMIT false

#ifdef __cplusplus
enum class ConsoleColor
{
    Black = 0,
    Red = 1,
    Green = 2,
    Yellow = 3,
    Blue = 4,
    Purple = 5,
    Cyan = 6,
    White = 7,
    BrightBlack = 60,
    BrightRed = 61,
    BrightGreen = 62,
    BrightYellow = 63,
    BrightBlue = 64,
    BrightPurple = 65,
    BrightCyan = 66,
    BrightWhite = 67,
    None = 0xFFFF
};

enum class FontType
{
    Normal = 0,
    Bold = 1,
    Faint = 2,
    Italic = 3,
    Underlined = 4,
    SlowBlink = 5,
    RapidBlink = 6,
    None = 0xFFFF
};
#endif

//#ifdef DEBUG

/* Log destinations:
 *  - SD Card
 *  - UART, debug output
 *  - USB, debug output (future)
 *  - Transmitted to SERVER (future)
 */

#define LOGE(tag, f_, ...) { WriteLogMessage(0, tag, f_, ##__VA_ARGS__);}
#define LOGI(tag, f_, ...) { WriteLogMessage(1, tag, f_, ##__VA_ARGS__);}
#define LOGIS(tag, f_, ...) { WriteLogMessage(2, tag, f_, ##__VA_ARGS__);}
#define LOGIW(tag, f_, ...) { WriteLogMessage(3, tag, f_, ##__VA_ARGS__);}
#define LOGIE(tag, f_, ...) { WriteLogMessage(4, tag, f_, ##__VA_ARGS__);}
#define LOGW(tag, f_, ...) { WriteLogMessage(5, tag, f_, ##__VA_ARGS__);}
#define LOGV(tag, f_, ...) { WriteLogMessage(6, tag, f_, ##__VA_ARGS__);}

#ifdef __cplusplus
extern "C" {
#endif
    void WriteLogMessage(int severity, const TCHAR* tag, const TCHAR* f_, ...);
    void DisableTag(const TCHAR* tag);
    void EnableTags();
#ifdef __cplusplus
}
#endif

#else 
// This is the logging setup in release mode

#define LOGE(tag, f_, ...) {}
#define LOGI(tag, f_, ...) {}
#define LOGIS(tag, f_, ...) {}
#define LOGIW(tag, f_, ...) {}
#define LOGIE(tag, f_, ...) {}
#define LOGW(tag, f_, ...) {}
#define LOGV(tag, f_, ...) {}

void WriteLogMessage(int severity, const TCHAR* tag, const TCHAR* f_, ...);
void DisableTag(const TCHAR* tag);
void EnableTags();

#endif

//#endif /* __LOGGING_H_ */

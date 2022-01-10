#include "main.h"
#include "SDCard.h"
#include "TimeHelper.h"
#include "Logging.h"
#include <string.h>

#ifdef TESTING
	#include <exception>
	#include <stdexcept>
#endif

char error_str_buffer[1000];
char tagDisabled[10] = "NotValid!"; 

/** 
 * @brief Disables a tag for logging. This is for example needed for the log 
 *        destinations, because e.g. the SD Card logging will write on the 
 *        during this process further log messages could occur, but must of the 
 *        messages are not as interesting, e.g. no SD card available, therefore 
 *        the SDCard tag can then be disabled for logging
 * @param[in] tag Name of tag to be disabled. Max. string length 9 characters 
 */
void DisableTag(const TCHAR* tag)
{
    memcpy(tagDisabled, tag, 10);
    tagDisabled[9] = '\0';
}

/**
 * @brief Enable all tags \see DisableTag 
 */
void EnableTags()
{
    memcpy(tagDisabled, "NotValid!", 10);
}

/**
 * @brief Writes color and style escape sequences into the printf log output
 * @param[in] type Type of font
 * @param[in] foreground Color of the text
 * @param[in] background Color of the text
 * @remark To reset the stype give all parameters with value None
 */
void ConsoleColorPrint(FontType type, ConsoleColor foreground, ConsoleColor background)
{
    memset(error_str_buffer, '\0', sizeof(error_str_buffer));

    if (type != FontType::None || 
        foreground != ConsoleColor::None ||
        background != ConsoleColor::None)
    {
        if (type != FontType::None)
        {
            sprintf(error_str_buffer, "\e[%dm", (int)type);
        }
        if (foreground != ConsoleColor::None)
        {

            sprintf(error_str_buffer + strlen(error_str_buffer), "\e[38:5:%dm", (int)foreground);
        }
        if (background != ConsoleColor::None)
        {
            sprintf(error_str_buffer + strlen(error_str_buffer), "\e[48:5:%dm", (int)background);
        }
    }
    else
    {
        strcat(error_str_buffer, "\x1b[0m");
    }
    printf("%s", error_str_buffer);
}

/**
 * @brief Logs a message to the enabled log destinations, every log message must
 *        be associatated with a tag. The format and optional parameters can be 
 *        used like printf.
 */
void WriteLogMessage(int severity, const TCHAR* tag, const TCHAR* format, ...)
{
    if (strcmp(tag, tagDisabled)==0)
    {
        return;
    }

    va_list args;
    if (LOG_UART)
    {
        switch (severity)
        {
            case (int)LogSeverity::Error:
                ConsoleColorPrint(
                    FontType::Bold,
                    ConsoleColor::Red,
                    ConsoleColor::None);
                break;
            case (int)LogSeverity::Warning:
                ConsoleColorPrint(
                    FontType::Bold,
                    ConsoleColor::Yellow,
                    ConsoleColor::None);
                break;
            case (int)LogSeverity::Info:
                ConsoleColorPrint(
                    FontType::Normal,
                    ConsoleColor::Green,
                    ConsoleColor::None);
                break;
            case (int)LogSeverity::InfoSuccess:
                ConsoleColorPrint(
                    FontType::None,
                    ConsoleColor::Blue,
                    ConsoleColor::None);
                break;
            case (int)LogSeverity::InfoWarning:
                ConsoleColorPrint(
                    FontType::Normal,
                    ConsoleColor::BrightYellow,
                    ConsoleColor::None);
                break;
            case (int)LogSeverity::InfoError:
                ConsoleColorPrint(
                    FontType::Normal,
                    ConsoleColor::BrightRed,
                    ConsoleColor::None);
                break;
            case (int)LogSeverity::Verbose:
                break;
        }

        char buffer[80];
        struct tm timeinfo;
        Time::Get(&timeinfo);
        strftime(buffer,80,"%F %H:%M:%S ", &timeinfo);
        printf("%s", buffer);

        switch (severity)
		{
			case (int)LogSeverity::Error:
				printf("E (%9s): ", tag);
				break;
			case (int)LogSeverity::Warning:
				printf("W (%9s): ", tag);
				break;
			case (int)LogSeverity::Info:
				printf("I (%9s): ", tag);
				break;
            case (int)LogSeverity::InfoError:
				printf("IE(%9s): ", tag);
				break;
            case (int)LogSeverity::InfoSuccess:
				printf("IS(%9s): ", tag);
				break;
            case (int)LogSeverity::InfoWarning:
				printf("IW(%9s): ", tag);
				break;
			case (int)LogSeverity::Verbose:
				printf("V (%9s): ", tag);
				break;
		}

        va_start(args, format);
        vprintf(format, args);
        va_end(args);

        ConsoleColorPrint(
            FontType::None,
            ConsoleColor::None,
            ConsoleColor::None);

        printf("\r\n");
    }
    if (LOG_SDCARD)
    {
        DisableTag("SDCard");
        bool wasInitialized = SDCard::Initialized();

        if (SDCard::Init() == HAL_OK)
		{
			__disable_irq(); // TODO: this is a work around for the, problem that currently interupts can damage the sd card while writing to it

            FIL fileHandler;
            if (f_open(&fileHandler, (const TCHAR*)FILENAME_LOG, FA_OPEN_APPEND | FA_WRITE) == FR_OK)
            {
                switch (severity)
                {
                    case (int)LogSeverity::Error:
                        f_printf(&fileHandler, "E (%9s): ", tag);
                        break;
                    case (int)LogSeverity::Warning:
                        f_printf(&fileHandler, "W (%9s): ", tag);
                        break;
                    case (int)LogSeverity::Info:
                        f_printf(&fileHandler, "I (%9s): ", tag);
                        break;
                    case (int)LogSeverity::InfoSuccess:
                        f_printf(&fileHandler, "IS(%9s): ", tag);
                        break;
                    case (int)LogSeverity::InfoWarning:
                        f_printf(&fileHandler, "IW(%9s): ", tag);
                        break;
                    case (int)LogSeverity::InfoError:
                        f_printf(&fileHandler, "IE(%9s): ", tag);
                        break;
                    case (int)LogSeverity::Verbose:
                        f_printf(&fileHandler, "V (%9s): ", tag);
                        break;
                }

                char buffer[80];
                struct tm timeinfo;
                Time::Get(&timeinfo);
                strftime(buffer,80,"%F %H:%M:%S ", &timeinfo);
                f_printf(&fileHandler, "%s", buffer);

                va_start(args, format);
                vsprintf(error_str_buffer, format, args);
                va_end(args);
                f_printf(&fileHandler, "%s\n", error_str_buffer);
                
                f_close(&fileHandler);
            }

			__enable_irq();
		}

        if (!wasInitialized)
        {
			if (SDCard::DeInit() == HAL_OK)
			{

			}
        }
        EnableTags();
    }

#ifdef TESTING
    if (severity == (int)LogSeverity::Error || severity == (int)LogSeverity::InfoError)
    {
	    throw std::out_of_range("An error message would have been printed");
    }
#endif
}


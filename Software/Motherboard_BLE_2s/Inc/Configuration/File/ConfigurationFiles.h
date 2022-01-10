/**
 * @brief Configuration handler
 * 
 * @file ConfigurationFiles.h
 * @author Daniel Lohmann
 * @date 2019-03-13
 */
#ifndef __CONFIGURATION_FILE_H_
#define __CONFIGURATION_FILE_H_

#include "Configuration.h"
#include "ConfigurationFileOption.h"
#include "LineInterpretationState.h"
#include "SDCard.h"
#include <list>

#define DEBUG_CONFIGFILES false

#define FILENAME_BASICSETUP_CONF    "basicsetup.conf.txt"
#define FILENAME_SETTINGS_CONF      "settings.conf.txt"
#define FILENAME_INFOFILE_CONF      "information.txt"

class ConfigurationFiles
{
public:
    ConfigurationFiles();
    ~ConfigurationFiles();
    void ReadBasicSetupFile();
    void ReadSettingsFile();
    void WriteInfoFile();

    #if DEBUG_CONFIGFILES
    static void DumpOptions();
    #endif
private:
    void InterpretLine(char* line, std::list<ConfigurationFileOption>& configurationOptions, LineInterpretationState* state);
    void ReadConfigFile(char* filename, std::list<ConfigurationFileOption>& configurationOptions);
    const std::list<ConfigurationFileOption> GetOptionsListBasicSetup() const;
    const std::list<ConfigurationFileOption> GetOptionsListSettings() const;

    #if DEBUG_CONFIGFILES
    void DumpOptionsList(std::list<ConfigurationFileOption>& configurationOptions);
    #endif
};

#endif /* __CONFIGURATION_FILE_H_ */

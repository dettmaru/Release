/*
 * BooleanConfigurationOptions.h
 *
 *  Created on: Mar 6, 2019
 *      Author: Daniel Lohmann
 *
 * This header contains an enum of boolean configuration options
 */

#ifndef CONFIGURATION_BOOLEANCONFIGURATIONOPTIONS_H_
#define CONFIGURATION_BOOLEANCONFIGURATIONOPTIONS_H_

enum class BoolOption {
    GotoStandby,
    SaveToSDCard,
    SendWLAN,
    SendNB_IoT,
    SendLoRa,
    SyncTimeNTP,
    SyncTImeGPS,
    END
};

#endif /* CONFIGURATION_BOOLEANCONFIGURATIONOPTIONS_H_ */
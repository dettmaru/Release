/*
 * StringDataConfigurationOptions.h
 *
 *  Created on: Mar 6, 2019
 *      Author: Daniel Lohmann
 *
 * This header contains an enum of string or general data configuration options
 */

#ifndef CONFIGURATION_STRINGDATACONFIGURATIONOPTIONS_H_
#define CONFIGURATION_STRINGDATACONFIGURATIONOPTIONS_H_

enum class StringOption {
    WLAN_SSID,
    WLAN_Password,
    MQTT_Broker,
    MQTT_Username,
    MQTT_Password,
    MQTT_Topic,
    NBIoT_APN,
    NBIoT_DestinationIP,
    NBIoT_Network,
    NTPTimeServer,
    CA_Certificate,
    END
};

#endif /* CONFIGURATION_STRINGDATACONFIGURATIONOPTIONS_H_ */
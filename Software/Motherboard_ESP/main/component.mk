#
# Main Makefile. This is basically the same as a component makefile.
#
# This Makefile should, at the very least, just include $(SDK_PATH)/make/component_common.mk. By default, 
# this will take the sources in the src/ directory, compile them and link them into 
# lib(subdirectory_name).a in the build directory. This behaviour is entirely configurable,
# please read the ESP-IDF documents if you need to do this.
#

COMPONENT_ADD_INCLUDEDIRS := . MQTTClient-C/src MQTTClient-C/src/mbedtls MQTTPacket/src

COMPONENT_SRCDIRS := $(COMPONENT_ADD_INCLUDEDIRS)

# embed files from the "certs" directory as binary data symbols
# in the app
COMPONENT_EMBED_TXTFILES := wpa2_ca.pem
COMPONENT_EMBED_TXTFILES += wpa2_client.crt
COMPONENT_EMBED_TXTFILES += wpa2_client.key

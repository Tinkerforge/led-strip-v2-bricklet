/* led-strip-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.h: TFP protocol message handling
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/bootloader/bootloader.h"

// Default functions
BootloaderHandleMessageResponse handle_message(const void *data, void *response);
void communication_tick(void);
void communication_init(void);

// Constants
#define LED_STRIP_V2_CHIP_TYPE_WS2801 2801
#define LED_STRIP_V2_CHIP_TYPE_WS2811 2811
#define LED_STRIP_V2_CHIP_TYPE_WS2812 2812
#define LED_STRIP_V2_CHIP_TYPE_LPD8806 8806
#define LED_STRIP_V2_CHIP_TYPE_APA102 102

#define LED_STRIP_V2_CHANNEL_MAPPING_RGB 6
#define LED_STRIP_V2_CHANNEL_MAPPING_RBG 9
#define LED_STRIP_V2_CHANNEL_MAPPING_BRG 33
#define LED_STRIP_V2_CHANNEL_MAPPING_BGR 36
#define LED_STRIP_V2_CHANNEL_MAPPING_GRB 18
#define LED_STRIP_V2_CHANNEL_MAPPING_GBR 24
#define LED_STRIP_V2_CHANNEL_MAPPING_RGBW 27
#define LED_STRIP_V2_CHANNEL_MAPPING_RGWB 30
#define LED_STRIP_V2_CHANNEL_MAPPING_RBGW 39
#define LED_STRIP_V2_CHANNEL_MAPPING_RBWG 45
#define LED_STRIP_V2_CHANNEL_MAPPING_RWGB 54
#define LED_STRIP_V2_CHANNEL_MAPPING_RWBG 57
#define LED_STRIP_V2_CHANNEL_MAPPING_GRWB 78
#define LED_STRIP_V2_CHANNEL_MAPPING_GRBW 75
#define LED_STRIP_V2_CHANNEL_MAPPING_GBWR 108
#define LED_STRIP_V2_CHANNEL_MAPPING_GBRW 99
#define LED_STRIP_V2_CHANNEL_MAPPING_GWBR 120
#define LED_STRIP_V2_CHANNEL_MAPPING_GWRB 114
#define LED_STRIP_V2_CHANNEL_MAPPING_BRGW 135
#define LED_STRIP_V2_CHANNEL_MAPPING_BRWG 141
#define LED_STRIP_V2_CHANNEL_MAPPING_BGRW 147
#define LED_STRIP_V2_CHANNEL_MAPPING_BGWR 156
#define LED_STRIP_V2_CHANNEL_MAPPING_BWRG 177
#define LED_STRIP_V2_CHANNEL_MAPPING_BWGR 180
#define LED_STRIP_V2_CHANNEL_MAPPING_WRBG 201
#define LED_STRIP_V2_CHANNEL_MAPPING_WRGB 198
#define LED_STRIP_V2_CHANNEL_MAPPING_WGBR 216
#define LED_STRIP_V2_CHANNEL_MAPPING_WGRB 210
#define LED_STRIP_V2_CHANNEL_MAPPING_WBGR 228
#define LED_STRIP_V2_CHANNEL_MAPPING_WBRG 225

#define LED_STRIP_V2_BOOTLOADER_MODE_BOOTLOADER 0
#define LED_STRIP_V2_BOOTLOADER_MODE_FIRMWARE 1
#define LED_STRIP_V2_BOOTLOADER_MODE_BOOTLOADER_WAIT_FOR_REBOOT 2
#define LED_STRIP_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_REBOOT 3
#define LED_STRIP_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_ERASE_AND_REBOOT 4

#define LED_STRIP_V2_BOOTLOADER_STATUS_OK 0
#define LED_STRIP_V2_BOOTLOADER_STATUS_INVALID_MODE 1
#define LED_STRIP_V2_BOOTLOADER_STATUS_NO_CHANGE 2
#define LED_STRIP_V2_BOOTLOADER_STATUS_ENTRY_FUNCTION_NOT_PRESENT 3
#define LED_STRIP_V2_BOOTLOADER_STATUS_DEVICE_IDENTIFIER_INCORRECT 4
#define LED_STRIP_V2_BOOTLOADER_STATUS_CRC_MISMATCH 5

#define LED_STRIP_V2_STATUS_LED_CONFIG_OFF 0
#define LED_STRIP_V2_STATUS_LED_CONFIG_ON 1
#define LED_STRIP_V2_STATUS_LED_CONFIG_SHOW_HEARTBEAT 2
#define LED_STRIP_V2_STATUS_LED_CONFIG_SHOW_STATUS 3

// Function and callback IDs and structs
#define FID_SET_LED_VALUES_LOW_LEVEL 1
#define FID_GET_LED_VALUES_LOW_LEVEL 2
#define FID_SET_FRAME_DURATION 3
#define FID_GET_FRAME_DURATION 4
#define FID_GET_SUPPLY_VOLTAGE 5
#define FID_SET_CLOCK_FREQUENCY 7
#define FID_GET_CLOCK_FREQUENCY 8
#define FID_SET_CHIP_TYPE 9
#define FID_GET_CHIP_TYPE 10
#define FID_SET_CHANNEL_MAPPING 11
#define FID_GET_CHANNEL_MAPPING 12
#define FID_SET_FRAME_STARTED_CALLBACK_CONFIGURATION 13
#define FID_GET_FRAME_STARTED_CALLBACK_CONFIGURATION 14

#define FID_CALLBACK_FRAME_STARTED 6

typedef struct {
	TFPMessageHeader header;
	uint16_t index;
	uint16_t value_length;
	uint16_t value_chunk_offset;
	char value_chunk_data[58];
} __attribute__((__packed__)) SetLEDValuesLowLevel;

typedef struct {
	TFPMessageHeader header;
	uint16_t index;
	uint16_t length;
} __attribute__((__packed__)) GetLEDValuesLowLevel;

typedef struct {
	TFPMessageHeader header;
	uint16_t value_length;
	uint16_t value_chunk_offset;
	char value_chunk_data[60];
} __attribute__((__packed__)) GetLEDValuesLowLevel_Response;

typedef struct {
	TFPMessageHeader header;
	uint16_t duration;
} __attribute__((__packed__)) SetFrameDuration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetFrameDuration;

typedef struct {
	TFPMessageHeader header;
	uint16_t duration;
} __attribute__((__packed__)) GetFrameDuration_Response;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetSupplyVoltage;

typedef struct {
	TFPMessageHeader header;
	uint16_t voltage;
} __attribute__((__packed__)) GetSupplyVoltage_Response;

typedef struct {
	TFPMessageHeader header;
	uint16_t length;
} __attribute__((__packed__)) FrameStarted_Callback;

typedef struct {
	TFPMessageHeader header;
	uint32_t frequency;
} __attribute__((__packed__)) SetClockFrequency;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetClockFrequency;

typedef struct {
	TFPMessageHeader header;
	uint32_t frequency;
} __attribute__((__packed__)) GetClockFrequency_Response;

typedef struct {
	TFPMessageHeader header;
	uint16_t chip;
} __attribute__((__packed__)) SetChipType;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetChipType;

typedef struct {
	TFPMessageHeader header;
	uint16_t chip;
} __attribute__((__packed__)) GetChipType_Response;

typedef struct {
	TFPMessageHeader header;
	uint8_t mapping;
} __attribute__((__packed__)) SetChannelMapping;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetChannelMapping;

typedef struct {
	TFPMessageHeader header;
	uint8_t mapping;
} __attribute__((__packed__)) GetChannelMapping_Response;

typedef struct {
	TFPMessageHeader header;
	bool enable;
} __attribute__((__packed__)) SetFrameStartedCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) GetFrameStartedCallbackConfiguration;

typedef struct {
	TFPMessageHeader header;
	bool enable;
} __attribute__((__packed__)) GetFrameStartedCallbackConfiguration_Response;


// Function prototypes
BootloaderHandleMessageResponse set_led_values_low_level(const SetLEDValuesLowLevel *data);
BootloaderHandleMessageResponse get_led_values_low_level(const GetLEDValuesLowLevel *data, GetLEDValuesLowLevel_Response *response);
BootloaderHandleMessageResponse set_frame_duration(const SetFrameDuration *data);
BootloaderHandleMessageResponse get_frame_duration(const GetFrameDuration *data, GetFrameDuration_Response *response);
BootloaderHandleMessageResponse get_supply_voltage(const GetSupplyVoltage *data, GetSupplyVoltage_Response *response);
BootloaderHandleMessageResponse set_clock_frequency(const SetClockFrequency *data);
BootloaderHandleMessageResponse get_clock_frequency(const GetClockFrequency *data, GetClockFrequency_Response *response);
BootloaderHandleMessageResponse set_chip_type(const SetChipType *data);
BootloaderHandleMessageResponse get_chip_type(const GetChipType *data, GetChipType_Response *response);
BootloaderHandleMessageResponse set_channel_mapping(const SetChannelMapping *data);
BootloaderHandleMessageResponse get_channel_mapping(const GetChannelMapping *data, GetChannelMapping_Response *response);
BootloaderHandleMessageResponse set_frame_started_callback_configuration(const SetFrameStartedCallbackConfiguration *data);
BootloaderHandleMessageResponse get_frame_started_callback_configuration(const GetFrameStartedCallbackConfiguration *data, GetFrameStartedCallbackConfiguration_Response *response);

// Callbacks
bool handle_frame_started_callback(void);

#define COMMUNICATION_CALLBACK_TICK_WAIT_MS 1
#define COMMUNICATION_CALLBACK_HANDLER_NUM 1
#define COMMUNICATION_CALLBACK_LIST_INIT \
	handle_frame_started_callback, \


#endif

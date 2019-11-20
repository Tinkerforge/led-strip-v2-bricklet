/* led-strip-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.c: TFP protocol message handling
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

#include "communication.h"

#include "bricklib2/utility/communication_callback.h"
#include "bricklib2/utility/util_definitions.h"
#include "bricklib2/protocols/tfp/tfp.h"

#include "led.h"

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_SET_LED_VALUES_LOW_LEVEL: return set_led_values_low_level(message);
		case FID_GET_LED_VALUES_LOW_LEVEL: return get_led_values_low_level(message, response);
		case FID_SET_FRAME_DURATION: return set_frame_duration(message);
		case FID_GET_FRAME_DURATION: return get_frame_duration(message, response);
		case FID_GET_SUPPLY_VOLTAGE: return get_supply_voltage(message, response);
		case FID_SET_CLOCK_FREQUENCY: return set_clock_frequency(message);
		case FID_GET_CLOCK_FREQUENCY: return get_clock_frequency(message, response);
		case FID_SET_CHIP_TYPE: return set_chip_type(message);
		case FID_GET_CHIP_TYPE: return get_chip_type(message, response);
		case FID_SET_CHANNEL_MAPPING: return set_channel_mapping(message);
		case FID_GET_CHANNEL_MAPPING: return get_channel_mapping(message, response);
		case FID_SET_FRAME_STARTED_CALLBACK_CONFIGURATION: return set_frame_started_callback_configuration(message);
		case FID_GET_FRAME_STARTED_CALLBACK_CONFIGURATION: return get_frame_started_callback_configuration(message, response);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}


BootloaderHandleMessageResponse set_led_values_low_level(const SetLEDValuesLowLevel *data) {
	uint16_t leds_length = data->index + data->value_length;

	if((leds_length > LED_BUFFER_SIZE) || (data->value_chunk_offset > data->value_length)) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	uint8_t length = MIN(58, data->value_length - data->value_chunk_offset);

	memcpy(&led.buffer_copy[data->index + data->value_chunk_offset], data->value_chunk_data, length);

	// End of stream.
	if((length < 58) || ((data->value_chunk_offset + length) == data->value_length)) {
		led.buffer_valid_length_next = leds_length;
	}

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_led_values_low_level(const GetLEDValuesLowLevel *data, GetLEDValuesLowLevel_Response *response) {
	static uint16_t stream_chunk_offset = 0;

	if(data->index + data->length > led.buffer_valid_length) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	const uint16_t stream_cur_index = data->index + stream_chunk_offset;
	const uint8_t chunk_length = MIN(MIN(60, led.buffer_valid_length - stream_cur_index), data->length);

	response->header.length      = sizeof(GetLEDValuesLowLevel_Response);
	response->value_chunk_offset = stream_chunk_offset;
	response->value_length       = data->length;
	memcpy(response->value_chunk_data, led.buffer_copy + stream_cur_index, chunk_length);

	stream_chunk_offset += chunk_length;
	if(stream_chunk_offset >= data->length) {
		stream_chunk_offset = 0;
	}

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_frame_duration(const SetFrameDuration *data) {
	led.frame_duration = data->duration;

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_frame_duration(const GetFrameDuration *data, GetFrameDuration_Response *response) {
	response->header.length = sizeof(GetFrameDuration_Response);
	response->duration      = led.frame_duration;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse get_supply_voltage(const GetSupplyVoltage *data, GetSupplyVoltage_Response *response) {
	response->header.length = sizeof(GetSupplyVoltage_Response);
	response->voltage       = led.voltage;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_clock_frequency(const SetClockFrequency *data) {
	if(data->frequency < 10000 || data->frequency > 2000000) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	led.clock_frequency = data->frequency;
	led_update_baudrate();

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_clock_frequency(const GetClockFrequency *data, GetClockFrequency_Response *response) {
	response->header.length = sizeof(GetClockFrequency_Response);
	response->frequency     = led.clock_frequency;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_chip_type(const SetChipType *data) {
	if((data->chip != LED_STRIP_V2_CHIP_TYPE_WS2801) &&
	   (data->chip != LED_STRIP_V2_CHIP_TYPE_WS2811) &&
	   (data->chip != LED_STRIP_V2_CHIP_TYPE_WS2812) &&
	   (data->chip != LED_STRIP_V2_CHIP_TYPE_LPD8806) &&
	   (data->chip != LED_STRIP_V2_CHIP_TYPE_APA102)) {
		return HANDLE_MESSAGE_RESPONSE_INVALID_PARAMETER;
	}

	led.chip_type = data->chip;
	led_update_chip_type();

	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_chip_type(const GetChipType *data, GetChipType_Response *response) {
	response->header.length = sizeof(GetChipType_Response);
	response->chip          = led.chip_type;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_channel_mapping(const SetChannelMapping *data) {
	led.mapping = data->mapping;
	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_channel_mapping(const GetChannelMapping *data, GetChannelMapping_Response *response) {
	response->header.length = sizeof(GetChannelMapping_Response);
	response->mapping       = led.mapping;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}

BootloaderHandleMessageResponse set_frame_started_callback_configuration(const SetFrameStartedCallbackConfiguration *data) {
	led.frame_started_callback_enabled = data->enable;
	return HANDLE_MESSAGE_RESPONSE_EMPTY;
}

BootloaderHandleMessageResponse get_frame_started_callback_configuration(const GetFrameStartedCallbackConfiguration *data, GetFrameStartedCallbackConfiguration_Response *response) {
	response->header.length = sizeof(GetFrameStartedCallbackConfiguration_Response);
	response->enable        = led.frame_started_callback_enabled;

	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}


bool handle_frame_started_callback(void) {
	static bool is_buffered = false;
	static FrameStarted_Callback cb;

	if(!is_buffered) {
		if((led.frame_started_length != 0) && led.frame_started_callback_enabled) {
			tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(FrameStarted_Callback), FID_CALLBACK_FRAME_STARTED);
			cb.length = led.frame_started_length;
			led.frame_started_length = 0;
		} else {
			return false;
		}
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(FrameStarted_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	communication_callback_init();
}

/* led-strip-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * led.c: LED communication handling
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

#include "led.h"

#include "configs/config.h"
#include "configs/config_led.h"

#include "bricklib2/hal/system_timer/system_timer.h"

#include "bricklib2/utility/util_definitions.h"

#include "communication.h"

#include "xmc_gpio.h"
#include "xmc_vadc.h"

#define led_tx_irq_handler_wo_clock IRQ_Hdlr_11
#define led_tx_irq_handler_w_clock IRQ_Hdlr_12

extern uint32_t ws281x_lut[];
LED led;


void __attribute__((optimize("-O3"))) __attribute__ ((section (".ram_code"))) led_tx_irq_handler_w_clock(void) {
	while(!XMC_USIC_CH_TXFIFO_IsFull(LED_USIC)) {
		LED_USIC->IN[0] = led.buffer[led.buffer_index];
		led.buffer_index++;
		if(led.buffer_index >= led.buffer_valid_length_irq) {
			led.frame_sending = false;
			XMC_USIC_CH_TXFIFO_DisableEvent(LED_USIC, XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD);
			XMC_USIC_CH_TXFIFO_ClearEvent(LED_USIC, XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD);
			return;
		}
	}
}

void __attribute__((optimize("-O3"))) __attribute__ ((section (".ram_code"))) led_tx_irq_handler_wo_clock(void) {
	while(XMC_USIC_CH_TXFIFO_GetLevel(LED_USIC) < 30) {
		const uint8_t value = led.buffer[led.buffer_index];
		LED_USIC->IN[0] = (ws281x_lut[value] >> 16) & 0xFFFF;
		LED_USIC->IN[0] = ws281x_lut[value] & 0xFFFF;
		led.buffer_index++;
		if(led.buffer_index >= led.buffer_valid_length_irq) {
			led.frame_sending = false;
			XMC_USIC_CH_TXFIFO_DisableEvent(LED_USIC, XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD);
			XMC_USIC_CH_TXFIFO_ClearEvent(LED_USIC, XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD);
			return;
		}
	}
}


void led_update_baudrate(void) {
	uint32_t frequency = led.clock_frequency;
	if((led.chip_type == LED_STRIP_V2_CHIP_TYPE_WS2811) || (led.chip_type == LED_STRIP_V2_CHIP_TYPE_WS2812)) {
		frequency = 3200000;
	}

	XMC_SPI_CH_Stop(LED_USIC);
	const XMC_SPI_CH_CONFIG_t channel_config = {
		.baudrate       = frequency,
		.bus_mode       = XMC_SPI_CH_BUS_MODE_MASTER,
		.selo_inversion = XMC_SPI_CH_SLAVE_SEL_INV_TO_MSLS,
		.parity_mode    = XMC_USIC_CH_PARITY_MODE_NONE
	};

	XMC_SPI_CH_Init(LED_USIC, &channel_config);
	LED_USIC->SCTR &= ~USIC_CH_SCTR_PDL_Msk; // Set passive data level to 0
	XMC_SPI_CH_SetBitOrderMsbFirst(LED_USIC);

	if((led.chip_type == LED_STRIP_V2_CHIP_TYPE_WS2811) || (led.chip_type == LED_STRIP_V2_CHIP_TYPE_WS2812)) {
		XMC_SPI_CH_SetWordLength(LED_USIC, 16);
	} else {
		XMC_SPI_CH_SetWordLength(LED_USIC, 8);
	}

	XMC_SPI_CH_SetFrameLength(LED_USIC, 64);
	XMC_SPI_CH_SetTransmitMode(LED_USIC, XMC_SPI_CH_MODE_STANDARD);

	// Configure the clock polarity and clock delay
	XMC_SPI_CH_ConfigureShiftClockOutput(LED_USIC,
									     XMC_SPI_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_1_DELAY_ENABLED,
									     XMC_SPI_CH_BRG_SHIFT_CLOCK_OUTPUT_SCLK);
	// Configure Leading/Trailing delay
	XMC_SPI_CH_SetSlaveSelectDelay(LED_USIC, 2);

	XMC_SPI_CH_Start(LED_USIC);
}

void led_update_chip_type(void) {
	const XMC_GPIO_CONFIG_t config_with_clock = {
		.mode             = LED_SCLK_PIN_AF,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	const XMC_GPIO_CONFIG_t config_without_clock = {
		.mode             = XMC_GPIO_MODE_INPUT_TRISTATE,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH,
		.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD
	};

	switch(led.chip_type) {
		case LED_STRIP_V2_CHIP_TYPE_WS2811:
		case LED_STRIP_V2_CHIP_TYPE_WS2812: {
			NVIC_DisableIRQ((IRQn_Type)LED_IRQ_TX_W_CLOCK);
			XMC_USIC_CH_TXFIFO_SetInterruptNodePointer(LED_USIC, XMC_USIC_CH_TXFIFO_INTERRUPT_NODE_POINTER_STANDARD, LED_SERVICE_REQUEST_WO_CLOCK);
			NVIC_EnableIRQ((IRQn_Type)LED_IRQ_TX_WO_CLOCK);
			XMC_GPIO_Init(LED_SCLK_PIN, &config_without_clock);
			break;
		}
		case LED_STRIP_V2_CHIP_TYPE_WS2801:
		case LED_STRIP_V2_CHIP_TYPE_LPD8806:
		case LED_STRIP_V2_CHIP_TYPE_APA102: {
			NVIC_DisableIRQ((IRQn_Type)LED_IRQ_TX_WO_CLOCK);
			XMC_USIC_CH_TXFIFO_SetInterruptNodePointer(LED_USIC, XMC_USIC_CH_TXFIFO_INTERRUPT_NODE_POINTER_STANDARD, LED_SERVICE_REQUEST_W_CLOCK);
			NVIC_EnableIRQ((IRQn_Type)LED_IRQ_TX_W_CLOCK);
			XMC_GPIO_Init(LED_SCLK_PIN, &config_with_clock);

			break;
		}
	}

	// Always update baudrate after chip type is set,
	// to be sure that we switch correctly between
	// baudrate of chips with clock and without clock
	led_update_baudrate();
}

uint8_t led_get_channels(void) {
	return ((led.mapping >= 27) && (led.mapping != 33) && (led.mapping != 36)) ? 4 : 3;
}

void led_buffer_copy_ldp8806(void) {
	const uint8_t m[] = {
		(led.mapping >> 4) & 0b11,
		(led.mapping >> 2) & 0b11,
		(led.mapping >> 0) & 0b11,
	};

	for(uint16_t i = 0; i < led.buffer_valid_length_next; i += 3) {
		led.buffer[i]   = ~(led.buffer_copy[i+m[0]] / 2 + 128);
		led.buffer[i+1] = ~(led.buffer_copy[i+m[1]] / 2 + 128);
		led.buffer[i+2] = ~(led.buffer_copy[i+m[2]] / 2 + 128);
	}

	uint16_t end_of_data = MAX(led.buffer_valid_length_next, led.buffer_valid_length);

	uint16_t latch_size = ((end_of_data / 3) + 31) / 32;
	for(uint8_t j = 0; j < latch_size; j++) {
		led.buffer[end_of_data + j] = 0xFF;
	}
}


void led_buffer_copy_apa102(void) {
	const uint8_t m[] = {
		(led.mapping >> 6) & 0b11,
		(led.mapping >> 4) & 0b11,
		(led.mapping >> 2) & 0b11,
		(led.mapping >> 0) & 0b11,
	};

	led.buffer[0] = 0xFF;
	led.buffer[1] = 0xFF;
	led.buffer[2] = 0xFF;
	led.buffer[3] = 0xFF;


	for(uint16_t i = 0; i < led.buffer_valid_length_next; i += 4) {
		led.buffer[i+4]   = ~((led.buffer_copy[i+m[0]]/8) | 0b11100000);  // 3-bit "1" and 5-bit brightness
		led.buffer[i+1+4] = ~led.buffer_copy[i+m[1]];
		led.buffer[i+2+4] = ~led.buffer_copy[i+m[2]];
		led.buffer[i+3+4] = ~(led.buffer_copy[i+m[3]]);
	}

	uint16_t end_of_data = MAX(led.buffer_valid_length_next, led.buffer_valid_length);

	// The datasheet says that there have to be a 4-byte endframe with all bits
	// set. But that create a fully white LED after the end of the configured
	// number of LEDs. The endframe is necessary without it the last configured
	// LED doesn't work correctly. Instead of sending the documented endframe,
	// only set the first three bit to 1. This works as well and dosn't create
	// a while LED at the end. Surprisingly it doesn't produce a black one either.
	// It just works as expected.
	led.buffer[end_of_data+4]   = 0xFF;
	led.buffer[end_of_data+4+1] = 0xFF;
	led.buffer[end_of_data+4+2] = 0xFF;
	led.buffer[end_of_data+4+3] = 0b00011111;
}

void led_buffer_copy_ws2801(void) {
	if(led_get_channels() == 3) {
		const uint8_t m[] = {
			(led.mapping >> 4) & 0b11,
			(led.mapping >> 2) & 0b11,
			(led.mapping >> 0) & 0b11,
		};

		for(uint16_t i = 0; i < led.buffer_valid_length_next; i += 3) {
			led.buffer[i]   = ~led.buffer_copy[i+m[0]];
			led.buffer[i+1] = ~led.buffer_copy[i+m[1]];
			led.buffer[i+2] = ~led.buffer_copy[i+m[2]];
		}
	} else {
		const uint8_t m[] = {
			(led.mapping >> 6) & 0b11,
			(led.mapping >> 4) & 0b11,
			(led.mapping >> 2) & 0b11,
			(led.mapping >> 0) & 0b11,
		};

		for(uint16_t i = 0; i < led.buffer_valid_length_next; i += 4) {
			led.buffer[i]   = ~led.buffer_copy[i+m[0]];
			led.buffer[i+1] = ~led.buffer_copy[i+m[1]];
			led.buffer[i+2] = ~led.buffer_copy[i+m[2]];
			led.buffer[i+3] = ~led.buffer_copy[i+m[3]];
		}
	}
}

void led_buffer_copy_ws281x(void) {
	if(led_get_channels() == 3) {
		const uint8_t m[] = {
			(led.mapping >> 4) & 0b11,
			(led.mapping >> 2) & 0b11,
			(led.mapping >> 0) & 0b11,
		};

		for(uint16_t i = 0; i < led.buffer_valid_length_next; i += 3) {
			led.buffer[i]   = led.buffer_copy[i+m[0]];
			led.buffer[i+1] = led.buffer_copy[i+m[1]];
			led.buffer[i+2] = led.buffer_copy[i+m[2]];
		}
	} else {
		const uint8_t m[] = {
			(led.mapping >> 6) & 0b11,
			(led.mapping >> 4) & 0b11,
			(led.mapping >> 2) & 0b11,
			(led.mapping >> 0) & 0b11,
		};

		for(uint16_t i = 0; i < led.buffer_valid_length_next; i += 4) {
			led.buffer[i]   = led.buffer_copy[i+m[0]];
			led.buffer[i+1] = led.buffer_copy[i+m[1]];
			led.buffer[i+2] = led.buffer_copy[i+m[2]];
			led.buffer[i+3] = led.buffer_copy[i+m[3]];
		}
	}
}

void led_measure_voltage(void) {
	static uint32_t last_time = 0;
	if(system_timer_is_time_elapsed_ms(last_time, 250)) {
		last_time = system_timer_get_ms();
		uint32_t result =  XMC_VADC_GROUP_GetDetailedResult(VADC_G0, 10);

		if((result & (1 << 31))) {
			// Resistor divisor is 1k to 1k so we have to go from 0-4095 to 0-3.3V with a multiplier of (1+11)/1 = 11
			// The multiplier of 12 has to be divided by 4 because of the used data accumulation mode.
			// 3300*11/(4095*4) = 605/273
			led.voltage = (result & 0xFFFF)*605/273;
		}
	}
}

void led_init_adc(void) {
	const XMC_GPIO_CONFIG_t voltage_pin_config = {
		.mode             = XMC_GPIO_MODE_INPUT_TRISTATE,
		.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_LARGE,
	};
	XMC_GPIO_Init(LED_VOLTAGE_PIN, &voltage_pin_config);

	// This structure contains the Global related Configuration.
	const XMC_VADC_GLOBAL_CONFIG_t adc_global_config = {
		.boundary0 = (uint32_t) 0, // Lower boundary value for Normal comparison mode
		.boundary1 = (uint32_t) 0, // Upper boundary value for Normal comparison mode

		.class0 = {
			.sample_time_std_conv     = 31,                      // The Sample time is (2*tadci)
			.conversion_mode_standard = XMC_VADC_CONVMODE_12BIT, // 12bit conversion Selected

		},
		.class1 = {
			.sample_time_std_conv     = 31,                      // The Sample time is (2*tadci)
			.conversion_mode_standard = XMC_VADC_CONVMODE_12BIT, // 12bit conversion Selected

		},

		.data_reduction_control         = 0b11, // Accumulate 4 result values
		.wait_for_read_mode             = 0, // GLOBRES Register will not be overwritten until the previous value is read
		.event_gen_enable               = 0, // Result Event from GLOBRES is disabled
		.disable_sleep_mode_control     = 0  // Sleep mode is enabled
	};


	// Global iclass0 configuration
	const XMC_VADC_GLOBAL_CLASS_t adc_global_iclass_config = {
		.conversion_mode_standard = XMC_VADC_CONVMODE_12BIT,
		.sample_time_std_conv	  = 31,
	};

	// Global Result Register configuration structure
	XMC_VADC_RESULT_CONFIG_t adc_global_result_config = {
		.data_reduction_control = 0b11,  // Accumulate 4 result values
		.post_processing_mode   = XMC_VADC_DMM_REDUCTION_MODE,
		.wait_for_read_mode  	= 1, // Enabled
		.part_of_fifo       	= 0, // No FIFO
		.event_gen_enable   	= 0  // Disable Result event
	};

	// LLD Background Scan Init Structure
	const XMC_VADC_BACKGROUND_CONFIG_t adc_background_config = {
		.conv_start_mode   = XMC_VADC_STARTMODE_CIR,       // Conversion start mode selected as cancel inject repeat
		.req_src_priority  = XMC_VADC_GROUP_RS_PRIORITY_1, // Priority of the Background request source in the VADC module
		.trigger_signal    = XMC_VADC_REQ_TR_A,            // If Trigger needed then this denotes the Trigger signal
		.trigger_edge      = XMC_VADC_TRIGGER_EDGE_NONE,   // If Trigger needed then this denotes Trigger edge selected
		.gate_signal       = XMC_VADC_REQ_GT_A,			   // If Gating needed then this denotes the Gating signal
		.timer_mode        = 0,							   // Timer Mode Disabled
		.external_trigger  = 0,                            // Trigger is Disabled
		.req_src_interrupt = 0,                            // Background Request source interrupt Disabled
		.enable_auto_scan  = 1,
		.load_mode         = XMC_VADC_SCAN_LOAD_OVERWRITE
	};

	const XMC_VADC_GROUP_CONFIG_t group_init_handle0 = {
		.emux_config = {
			.stce_usage                  = 0, 					           // Use STCE when the setting changes
			.emux_mode                   = XMC_VADC_GROUP_EMUXMODE_SWCTRL, // Mode for Emux conversion
			.emux_coding                 = XMC_VADC_GROUP_EMUXCODE_BINARY, // Channel progression - binary format
			.starting_external_channel   = 0,                              // Channel starts at 0 for EMUX
			.connected_channel           = 0                               // Channel connected to EMUX
		},
		.class0 = {
			.sample_time_std_conv        = 31,                             // The Sample time is (2*tadci)
			.conversion_mode_standard    = XMC_VADC_CONVMODE_12BIT,        // 12bit conversion Selected
			.sampling_phase_emux_channel = 0,                              // The Sample time is (2*tadci)
			.conversion_mode_emux        = XMC_VADC_CONVMODE_12BIT         // 12bit conversion Selected
		},
		.class1 = {
			.sample_time_std_conv        = 31,                             // The Sample time is (2*tadci)
			.conversion_mode_standard    = XMC_VADC_CONVMODE_12BIT,        // 12bit conversion Selected
			.sampling_phase_emux_channel = 0,                              // The Sample time is (2*tadci)
			.conversion_mode_emux        = XMC_VADC_CONVMODE_12BIT         // 12bit conversion Selected
		},

		.boundary0                       = 0,                              // Lower boundary value for Normal comparison mode
		.boundary1	                     = 0,                              // Upper boundary value for Normal comparison mode
		.arbitration_round_length        = 0,                              // 4 arbitration slots per round selected (tarb = 4*tadcd) */
		.arbiter_mode                    = XMC_VADC_GROUP_ARBMODE_ALWAYS,  // Determines when the arbiter should run.
	};


	XMC_VADC_CHANNEL_CONFIG_t  channel_a_config = {
		.input_class                =  XMC_VADC_CHANNEL_CONV_GLOBAL_CLASS0,    // Global ICLASS 0 selected
		.lower_boundary_select 	    =  XMC_VADC_CHANNEL_BOUNDARY_GROUP_BOUND0,
		.upper_boundary_select 	    =  XMC_VADC_CHANNEL_BOUNDARY_GROUP_BOUND0,
		.event_gen_criteria         =  XMC_VADC_CHANNEL_EVGEN_NEVER,           // Channel Event disabled
		.sync_conversion  		    =  0,                                      // Sync feature disabled
		.alternate_reference        =  XMC_VADC_CHANNEL_REF_INTREF,            // Internal reference selected
		.result_reg_number          =  10,                                     // GxRES[10] selected
		.use_global_result          =  0, 				                       // Use Group result register
		.result_alignment           =  XMC_VADC_RESULT_ALIGN_RIGHT,            // Result alignment - Right Aligned
		.broken_wire_detect_channel =  XMC_VADC_CHANNEL_BWDCH_VAGND,           // No Broken wire mode select
		.broken_wire_detect		    =  0,    		                           // No Broken wire detection
		.bfl                        =  0,                                      // No Boundary flag
		.channel_priority           =  0,                   		           // Lowest Priority 0 selected
		.alias_channel              =  -1                                      // Channel is Aliased
	};

	XMC_VADC_RESULT_CONFIG_t channel_a_result_config =
	{
		.data_reduction_control = 0b11,                         // Accumulate 4 result values
		.post_processing_mode   = XMC_VADC_DMM_REDUCTION_MODE,  // Use reduction mode
		.wait_for_read_mode  	= 1,                            // Enabled
		.part_of_fifo       	= 0 ,                           // No FIFO
		.event_gen_enable   	= 0                             // Disable Result event
	};


	XMC_VADC_GLOBAL_Init(VADC, &adc_global_config);

    XMC_VADC_GROUP_Init(VADC_G0, &group_init_handle0);
    XMC_VADC_GROUP_SetPowerMode(VADC_G0, XMC_VADC_GROUP_POWERMODE_NORMAL);

    XMC_VADC_GLOBAL_SHS_EnableAcceleratedMode(SHS0, XMC_VADC_GROUP_INDEX_0);
    XMC_VADC_GLOBAL_SHS_SetAnalogReference(SHS0, XMC_VADC_GLOBAL_SHS_AREF_EXTERNAL_VDD_UPPER_RANGE);

	XMC_VADC_GLOBAL_StartupCalibration(VADC);

	// Initialize the Global Conversion class 0
	XMC_VADC_GLOBAL_InputClassInit(VADC, adc_global_iclass_config, XMC_VADC_GROUP_CONV_STD, 0);

	// Initialize the Background Scan hardware
	XMC_VADC_GLOBAL_BackgroundInit(VADC, &adc_background_config);

	// Initialize the global result register
	XMC_VADC_GLOBAL_ResultInit(VADC, &adc_global_result_config);

    /* Initialize for configured channels*/
    XMC_VADC_GROUP_ChannelInit(VADC_G0, LED_VOLTAGE_CHANNEL, &channel_a_config);

    /* Initialize for configured result registers */
    XMC_VADC_GROUP_ResultInit(VADC_G0, channel_a_config.result_reg_number, &channel_a_result_config);

	XMC_VADC_GLOBAL_BackgroundAddChannelToSequence(VADC, 0, LED_VOLTAGE_CHANNEL);
	XMC_VADC_GLOBAL_SetResultEventInterruptNode(VADC, XMC_VADC_SR_SHARED_SR0);

	XMC_VADC_GLOBAL_BackgroundTriggerConversion(VADC);
}

void led_tick(void) {
	led_measure_voltage();

	if(!led.frame_sending) {
		if(led.buffer_valid_length_next != 0) {
			switch(led.chip_type) {
				case LED_STRIP_V2_CHIP_TYPE_WS2801:  led_buffer_copy_ws2801();  break;
				case LED_STRIP_V2_CHIP_TYPE_WS2811:
				case LED_STRIP_V2_CHIP_TYPE_WS2812:  led_buffer_copy_ws281x();  break;
				case LED_STRIP_V2_CHIP_TYPE_LPD8806: led_buffer_copy_ldp8806(); break;
				case LED_STRIP_V2_CHIP_TYPE_APA102:  led_buffer_copy_apa102();  break;
			}

			if(led.buffer_valid_length_next > led.buffer_valid_length) {
				led.buffer_valid_length = led.buffer_valid_length_next;
			}

			led.buffer_valid_length_irq = led.buffer_valid_length;
			if(led.chip_type == LED_STRIP_V2_CHIP_TYPE_APA102) {
				led.buffer_valid_length_irq += 8; // Add 8 bytes for pre/post fix of APA102 protocol
			} else if(led.chip_type == LED_STRIP_V2_CHIP_TYPE_LPD8806) {
				led.buffer_valid_length_irq += ((led.buffer_valid_length / 3) + 31) / 32; // Add latch bytes for LPD8806 protocol
			}

			led.buffer_valid_length_next = 0;
		}

		if(system_timer_is_time_elapsed_ms(led.frame_start, led.frame_duration)) {
			led.frame_start += led.frame_duration;

			// If the duration is smaller then possible to send, we make sure that
			// the frame_start variable does not start to completely drift away
			// from the current time.
			if(system_timer_is_time_elapsed_ms(led.frame_start, led.frame_duration)) {
				led.frame_start = system_timer_get_ms() - led.frame_duration;
			}

			if(led.buffer_valid_length == 0) {
				// There is no data available
				return;
			}

			led.frame_sending = true;
			led.buffer_index = 0;
			led.frame_started_length = led.buffer_valid_length;

			XMC_USIC_CH_TXFIFO_EnableEvent(LED_USIC, XMC_USIC_CH_TXFIFO_EVENT_CONF_STANDARD);
			switch(led.chip_type) {
				case LED_STRIP_V2_CHIP_TYPE_WS2811:
				case LED_STRIP_V2_CHIP_TYPE_WS2812: XMC_USIC_CH_TriggerServiceRequest(LED_USIC, LED_SERVICE_REQUEST_WO_CLOCK); break;
				case LED_STRIP_V2_CHIP_TYPE_WS2801:
				case LED_STRIP_V2_CHIP_TYPE_LPD8806:
				case LED_STRIP_V2_CHIP_TYPE_APA102: XMC_USIC_CH_TriggerServiceRequest(LED_USIC, LED_SERVICE_REQUEST_W_CLOCK); break;
			}

		}
	}
}

void led_init(void) {
	// Initialize the whole struct as zero
	memset(&led, 0, sizeof(LED));

	led.clock_frequency = LED_BAUDRATE_DEFAULT;
	led.frame_duration  = 100;
	led.chip_type       = LED_STRIP_V2_CHIP_TYPE_WS2801;
	led.mapping         = LED_STRIP_V2_CHANNEL_MAPPING_BGR;
	led.frame_started_callback_enabled = true;

	// USIC channel configuration
	const XMC_SPI_CH_CONFIG_t channel_config = {
		.baudrate       = LED_BAUDRATE_DEFAULT,
		.bus_mode       = XMC_SPI_CH_BUS_MODE_MASTER,
		.selo_inversion = XMC_SPI_CH_SLAVE_SEL_INV_TO_MSLS,
		.parity_mode    = XMC_USIC_CH_PARITY_MODE_NONE
	};

	// MOSI pin configuration
	const XMC_GPIO_CONFIG_t mosi_pin_config = {
		.mode             = LED_MOSI_PIN_AF,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	// SCLK pin configuration
	const XMC_GPIO_CONFIG_t sclk_pin_config = {
		.mode             = LED_SCLK_PIN_AF,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH
	};

	// Voltage pin configuration
	const XMC_GPIO_CONFIG_t voltage_pin_config = {
		.mode             = XMC_GPIO_MODE_INPUT_TRISTATE,
		.output_level     = XMC_GPIO_OUTPUT_LEVEL_HIGH,
		.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD
	};


	// Configure GPIO pins
	XMC_GPIO_Init(LED_VOLTAGE_PIN, &voltage_pin_config);

	// Initialize USIC channel in SPI master mode
	XMC_SPI_CH_Init(LED_USIC, &channel_config);
	LED_USIC->SCTR = ~USIC_CH_SCTR_PDL_Msk; // Set passive data level to 0

	XMC_SPI_CH_SetBitOrderMsbFirst(LED_USIC);

	XMC_SPI_CH_SetWordLength(LED_USIC, 8);
	XMC_SPI_CH_SetFrameLength(LED_USIC, 64);

	XMC_SPI_CH_SetTransmitMode(LED_USIC, XMC_SPI_CH_MODE_STANDARD);

	// Configure the clock polarity and clock delay
	XMC_SPI_CH_ConfigureShiftClockOutput(LED_USIC,
									     XMC_SPI_CH_BRG_SHIFT_CLOCK_PASSIVE_LEVEL_1_DELAY_ENABLED,
									     XMC_SPI_CH_BRG_SHIFT_CLOCK_OUTPUT_SCLK);
	// Configure Leading/Trailing delay
	XMC_SPI_CH_SetSlaveSelectDelay(LED_USIC, 2);

	// Configure transmit FIFO
	XMC_USIC_CH_TXFIFO_Configure(LED_USIC, 32, XMC_USIC_CH_FIFO_SIZE_32WORDS, 16);

	// Set service request for tx FIFO transmit interrupt
	XMC_USIC_CH_TXFIFO_SetInterruptNodePointer(LED_USIC, XMC_USIC_CH_TXFIFO_INTERRUPT_NODE_POINTER_STANDARD, LED_SERVICE_REQUEST_W_CLOCK);

	//Set priority and enable NVIC node for transmit interrupt
	NVIC_SetPriority(LED_IRQ_TX_WO_CLOCK, LED_IRQ_TX_PRIORITY);
	NVIC_SetPriority(LED_IRQ_TX_W_CLOCK, LED_IRQ_TX_PRIORITY);
	NVIC_EnableIRQ(LED_IRQ_TX_W_CLOCK);

	// Start SPI
	XMC_SPI_CH_Start(LED_USIC);

	// Configure SCLK pin
	XMC_GPIO_Init(LED_SCLK_PIN, &sclk_pin_config);

	//Configure MOSI pin
	XMC_GPIO_Init(LED_MOSI_PIN, &mosi_pin_config);

	led_init_adc();

	led_update_chip_type();
}

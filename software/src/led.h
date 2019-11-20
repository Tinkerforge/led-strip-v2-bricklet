/* led-strip-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * led.h: LED communication handling
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

#ifndef LED_H
#define LED_H

#include <stdint.h>
#include <stdbool.h>

#define LED_BUFFER_SIZE (2048*3)
#define LED_BAUDRATE_DEFAULT 1660000

typedef struct {
	uint8_t buffer[LED_BUFFER_SIZE+100];
	uint16_t buffer_index;
	uint16_t buffer_valid_length;
	uint16_t buffer_valid_length_irq;
	uint16_t buffer_valid_length_next;

	uint8_t buffer_copy[LED_BUFFER_SIZE];

	uint16_t frame_duration;
	uint32_t clock_frequency;
	uint16_t chip_type;
	uint8_t mapping;
	uint16_t frame_started_length;

	uint32_t frame_start;

	uint16_t voltage;

	bool frame_sending;
	bool frame_started_callback_enabled;
} LED;
extern LED led;

void led_update_baudrate(void);
void led_update_chip_type(void);

void led_init(void);
void led_tick(void);

#endif

/* led-strip-v2-bricklet
 * Copyright (C) 2018 Olaf Lüke <olaf@tinkerforge.com>
 *
 * config_led.h: Configuration for LED strip clock/data
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

#ifndef CONFIG_LED_H
#define CONFIG_LED_H

#include "xmc_gpio.h"
#include "xmc_spi.h"

#define LED_USIC_CHANNEL             USIC0_CH1
#define LED_USIC                     XMC_SPI0_CH1

#define LED_SCLK_PIN                 P0_8
#define LED_SCLK_PIN_AF              (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7 | P0_8_AF_U0C1_SCLKOUT)

#define LED_MOSI_PIN                 P0_6
#define LED_MOSI_PIN_AF              (XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT7 | P0_6_AF_U0C1_DOUT0)

#define LED_SERVICE_REQUEST_WO_CLOCK 2
#define LED_SERVICE_REQUEST_W_CLOCK  3

#define LED_IRQ_TX_WO_CLOCK          11
#define LED_IRQ_TX_W_CLOCK           12
#define LED_IRQ_TX_PRIORITY          3

#define LED_VOLTAGE_PIN              P2_1
#define LED_VOLTAGE_CHANNEL          6

#endif

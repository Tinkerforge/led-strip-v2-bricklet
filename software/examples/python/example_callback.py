#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your LED Strip Bricklet 2.0
NUM_LEDS = 16

rgb = [0, 0, 0]*NUM_LEDS
index = 0

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_led_strip_v2 import BrickletLEDStripV2

# Use frame started callback to move the active LED every frame
def cb_frame_started(length, ls):
    global index

    rgb[index] = 0

    index += 3
    if index >= NUM_LEDS:
        index = 0

    rgb[index] = 255

    # Set new data for next render cycle
    ls.set_led_values(0, rgb)

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    ls = BrickletLEDStripV2(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Set frame duration to 50ms (20 frames per second)
    ls.set_frame_duration(50)

    # Register frame started callback to function cb_frame_started
    ls.register_callback(ls.CALLBACK_FRAME_STARTED,
                         lambda x: cb_frame_started(x, ls))

    # Set initial rgb values to get started
    ls.set_led_values(0, rgb)

    input("Press key to exit\n") # Use raw_input() in Python 2
    ipcon.disconnect()

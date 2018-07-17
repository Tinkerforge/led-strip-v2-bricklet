#!/usr/bin/env python
# -*- coding: utf-8 -*-

# FIXME: This example is incomplete

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your LED Strip Bricklet 2.0

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_led_strip_v2 import BrickletLEDStripV2

# Use frame started callback to move the active LED every frame
def cb_frame_started(length):
    print("Length: " + str(length))

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    ls = BrickletLEDStripV2(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Set frame duration to 50ms (20 frames per second)
    ls.set_frame_duration(50)

    # Register frame started callback to function cb_frame_started
    ls.register_callback(ls.CALLBACK_FRAME_STARTED, cb_frame_started)

    raw_input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()

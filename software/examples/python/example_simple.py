#!/usr/bin/env python
# -*- coding: utf-8 -*-

# FIXME: This example is incomplete

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your LED Strip Bricklet 2.0

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_led_strip_v2 import BrickletLEDStripV2

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    ls = BrickletLEDStripV2(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # TODO: Add example code here

    raw_input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()

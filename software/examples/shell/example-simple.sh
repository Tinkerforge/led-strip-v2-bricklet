#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your LED Strip Bricklet 2.0

# Set first 3 LEDs to red, green and blue
tinkerforge call led-strip-v2-bricklet $uid set-led-values 0 255,0,0,0,255,0,0,0,255

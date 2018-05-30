#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

# FIXME: This example is incomplete

uid=XYZ # Change XYZ to the UID of your LED Strip Bricklet 2.0

# Set frame duration to 50ms (20 frames per second)
tinkerforge call led-strip-v2-bricklet $uid set-frame-duration 50

# Use frame rendered callback to move the active LED every frame
tinkerforge dispatch led-strip-v2-bricklet $uid frame-rendered &

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background

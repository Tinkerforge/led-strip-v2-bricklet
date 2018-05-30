#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

# FIXME: This example is incomplete

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_led_strip_v2'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your LED Strip Bricklet 2.0

ipcon = IPConnection.new # Create IP connection
ls = BrickletLEDStripV2.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# TODO: Add example code here

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect

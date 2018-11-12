#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletLEDStripV2;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your LED Strip Bricklet 2.0

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $ls = Tinkerforge::BrickletLEDStripV2->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Set first 3 LEDs to red, green and blue
$ls->set_led_values(0, [255, 0, 0, 0, 255, 0, 0, 0, 255]);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();

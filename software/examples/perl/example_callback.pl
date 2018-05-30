#!/usr/bin/perl

# FIXME: This example is incomplete

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletLEDStripV2;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your LED Strip Bricklet 2.0

# Use frame rendered callback to move the active LED every frame
sub cb_frame_rendered
{
    my ($length) = @_;

    print "Length: $length\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $ls = Tinkerforge::BrickletLEDStripV2->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Set frame duration to 50ms (20 frames per second)
$ls->set_frame_duration(50);

# Register frame rendered callback to subroutine cb_frame_rendered
$ls->register_callback($ls->CALLBACK_FRAME_RENDERED, 'cb_frame_rendered');

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();

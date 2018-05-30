<?php

// FIXME: This example is incomplete

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletLEDStripV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletLEDStripV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your LED Strip Bricklet 2.0

// Use frame rendered callback to move the active LED every frame
function cb_frameRendered($length)
{
    echo "Length: $length\n";
}

$ipcon = new IPConnection(); // Create IP connection
$ls = new BrickletLEDStripV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Set frame duration to 50ms (20 frames per second)
$ls->setFrameDuration(50);

// Register frame rendered callback to function cb_frameRendered
$ls->registerCallback(BrickletLEDStripV2::CALLBACK_FRAME_RENDERED, 'cb_frameRendered');

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>

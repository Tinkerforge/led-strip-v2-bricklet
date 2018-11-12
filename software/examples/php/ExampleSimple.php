<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletLEDStripV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletLEDStripV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your LED Strip Bricklet 2.0

$ipcon = new IPConnection(); // Create IP connection
$ls = new BrickletLEDStripV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Set first 3 LEDs to red, green and blue
$ls->setLEDValues(0, array(255, 0, 0, 0, 255, 0, 0, 0, 255));

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>

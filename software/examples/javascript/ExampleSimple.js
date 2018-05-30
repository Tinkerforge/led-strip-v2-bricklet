var Tinkerforge = require('tinkerforge');

// FIXME: This example is incomplete

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change XYZ to the UID of your LED Strip Bricklet 2.0

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var ls = new Tinkerforge.BrickletLEDStripV2(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function (error) {
        console.log('Error: ' + error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function (connectReason) {
        // TODO: Add example code here
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);

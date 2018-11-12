var Tinkerforge = require('tinkerforge');

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
        // Set first 3 LEDs to red, green and blue
        ls.setLEDValues(0, [255, 0, 0, 0, 255, 0, 0, 0, 255]);
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);

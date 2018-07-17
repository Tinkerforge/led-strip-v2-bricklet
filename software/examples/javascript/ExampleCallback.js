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
        // Set frame duration to 50ms (20 frames per second)
        ls.setFrameDuration(50);
    }
);

// Register frame started callback
ls.on(Tinkerforge.BrickletLEDStripV2.CALLBACK_FRAME_STARTED,
    // Use frame started callback to move the active LED every frame
    function (length) {
        console.log('Length: ' + length);
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);

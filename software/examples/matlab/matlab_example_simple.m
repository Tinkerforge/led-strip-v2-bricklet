function matlab_example_simple()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletLEDStripV2;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your LED Strip Bricklet 2.0

    ipcon = IPConnection(); % Create IP connection
    ls = handle(BrickletLEDStripV2(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Set first 3 LEDs to red, green and blue
    ls.setLEDValues(0, [255 0 0 0 255 0 0 0 255]);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

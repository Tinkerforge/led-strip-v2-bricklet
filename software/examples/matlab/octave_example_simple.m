function octave_example_simple()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your LED Strip Bricklet 2.0

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    ls = javaObject("com.tinkerforge.BrickletLEDStripV2", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Set first 3 LEDs to red, green and blue
    ls.setLEDValues(0, [255 0 0 0 255 0 0 0 255]);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

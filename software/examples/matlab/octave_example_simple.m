function octave_example_simple()
    more off;

    % FIXME: This example is incomplete

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your LED Strip Bricklet 2.0

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    ls = javaObject("com.tinkerforge.BrickletLEDStripV2", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    # TODO: Add example code here

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

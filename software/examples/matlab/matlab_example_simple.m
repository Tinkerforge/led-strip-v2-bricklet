function matlab_example_simple()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletLEDStripV2;

    % FIXME: This example is incomplete

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your LED Strip Bricklet 2.0

    ipcon = IPConnection(); % Create IP connection
    ls = handle(BrickletLEDStripV2(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % TODO: Add example code here

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

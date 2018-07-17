function octave_example_callback()
    more off;

    % FIXME: This example is incomplete

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your LED Strip Bricklet 2.0

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    ls = javaObject("com.tinkerforge.BrickletLEDStripV2", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Set frame duration to 50ms (20 frames per second)
    ls.setFrameDuration(50);

    % Register frame started callback to function cb_frame_started
    ls.addFrameStartedCallback(@cb_frame_started);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Use frame started callback to move the active LED every frame
function cb_frame_started(e)
    fprintf("Length: %d\n", e.length);
end

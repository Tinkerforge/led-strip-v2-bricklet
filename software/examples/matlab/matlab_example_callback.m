function matlab_example_callback()
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

    % Set frame duration to 50ms (20 frames per second)
    ls.setFrameDuration(50);

    % Register frame rendered callback to function cb_frame_rendered
    set(ls, 'FrameRenderedCallback', @(h, e) cb_frame_rendered(e));

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Use frame rendered callback to move the active LED every frame
function cb_frame_rendered(e)
    fprintf('Length: %i\n', e.length);
end

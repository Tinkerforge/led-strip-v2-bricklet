use std::{error::Error, io, thread};
use tinkerforge::{ip_connection::IpConnection, led_strip_v2_bricklet::*};

// FIXME: This example is incomplete

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your LED Strip Bricklet 2.0

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let led_strip_v2_bricklet = LedStripV2Bricklet::new(UID, &ipcon); // Create device object

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd
                                          // Don't use device before ipcon is connected

    // Set frame duration to 50ms (20 frames per second)
    led_strip_v2_bricklet.set_frame_duration(50);

    let frame_started_receiver = led_strip_v2_bricklet.get_frame_started_callback_receiver();

    // Spawn thread to handle received events.
    // This thread ends when the `led_strip_v2_bricklet` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for event in frame_started_receiver {
            println!("Length: {}", event);
        }
    });

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}

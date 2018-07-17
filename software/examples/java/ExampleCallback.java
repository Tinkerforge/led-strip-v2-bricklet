import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletLEDStripV2;

// FIXME: This example is incomplete

public class ExampleCallback {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your LED Strip Bricklet 2.0
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletLEDStripV2 ls = new BrickletLEDStripV2(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Set frame duration to 50ms (20 frames per second)
		ls.setFrameDuration(50);

		// Use frame started callback to move the active LED every frame
		ls.addFrameStartedListener(new BrickletLEDStripV2.FrameStartedListener() {
			public void frameStarted(int length) {
				System.out.println("Length: " + length);
			}
		});

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}

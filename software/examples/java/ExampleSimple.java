import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletLEDStripV2;

public class ExampleSimple {
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

		// Set first 3 LEDs to red, green and blue
		ls.setLEDValues(0, new int[]{255, 0, 0, 0, 255, 0, 0, 0, 255});

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}

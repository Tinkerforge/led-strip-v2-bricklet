using System;
using Tinkerforge;

// FIXME: This example is incomplete

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your LED Strip Bricklet 2.0

	// Use frame started callback to move the active LED every frame
	static void FrameStartedCB(BrickletLEDStripV2 sender, int length)
	{
		Console.WriteLine("Length: " + length);
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletLEDStripV2 ls = new BrickletLEDStripV2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Set frame duration to 50ms (20 frames per second)
		ls.SetFrameDuration(50);

		// Register frame started callback to function FrameStartedCB
		ls.FrameStartedCallback += FrameStartedCB;

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}

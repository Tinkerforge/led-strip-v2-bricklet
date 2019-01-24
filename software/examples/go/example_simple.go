package main

import (
	"fmt"
	"github.com/Tinkerforge/go-api-bindings/ipconnection"
	"github.com/Tinkerforge/go-api-bindings/led_strip_v2_bricklet"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your LED Strip Bricklet 2.0.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	ls, _ := led_strip_v2_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	// Set first 3 LEDs to red, green and blue
	ls.SetLEDValues(0, []uint8{255, 0, 0, 0, 255, 0, 0, 0, 255})

	fmt.Print("Press enter to exit.")
	fmt.Scanln()
}

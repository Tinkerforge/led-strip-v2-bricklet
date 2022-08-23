// This example is not self-contained.
// It requires usage of the example driver specific to your platform.
// See the HAL documentation.

#include "src/bindings/hal_common.h"
#include "src/bindings/bricklet_led_strip_v2.h"

void check(int rc, const char *msg);
void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);

static TF_LEDStripV2 ls;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_led_strip_v2_create(&ls, NULL, hal), "create device object");

	// Set first 3 LEDs to red, green and blue
	uint8_t value[9] = {255, 0, 0, 0, 255, 0, 0, 0, 255};
	check(tf_led_strip_v2_set_led_values(&ls, 0, value, 9), "call set_led_values");
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}

#include "bindings/hal_common.h"
#include "bindings/bricklet_led_strip_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your LED Strip Bricklet 2.0

void example_setup(TF_HalContext *hal);
void example_loop(TF_HalContext *hal);

void check(int rc, const char* msg);

#define NUM_LEDS 16

static uint8_t rgb[NUM_LEDS * 3] = {0};
static uint16_t idx = 0;

static bool frame_started = false;
// Use frame started callback to trigger drawing the next frame.
static void frame_started_handler(TF_LEDStripV2 *device, uint16_t length, void *user_data) {
	(void)device; (void)length; (void)user_data; // avoid unused parameter warning

	frame_started = true;
}

static TF_LEDStripV2 ls;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_led_strip_v2_create(&ls, UID, hal), "create device object");

	// Set frame duration to 50ms (20 frames per second)
	check(tf_led_strip_v2_set_frame_duration(&ls, 50), "call set_frame_duration");

	// Register frame started callback to function frame_started_handler
	tf_led_strip_v2_register_frame_started_callback(&ls,
	                                                frame_started_handler,
	                                                NULL);

	// Write one frame to trigger the frame started callback.
	tf_led_strip_v2_set_led_values(&ls, 0, rgb, NUM_LEDS * 3);
}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	// Polling with 0 will process one packet at most, so we can't miss a frame.
	tf_hal_callback_tick(hal, 0);
	if (!frame_started) {
		return;
	}

	frame_started = false;

	rgb[idx] = 0;
	idx += 3;
	if (idx >= NUM_LEDS * 3) {
		idx = 0;
	}
	rgb[idx] = 255;

	tf_led_strip_v2_set_led_values(&ls, 0, rgb, NUM_LEDS * 3);
}

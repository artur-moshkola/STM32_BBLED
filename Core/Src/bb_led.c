#include "main.h"
#include "bb_led.h"
#include "ws2812-spi.h"

uint32_t bb_led_tck;

uint8_t ihndlr;
uint8_t (*bb_led_frame_handler)(uint8_t);

void bb_led_init(void) {
	bb_led_tck = 0;
}

void bb_led_tick(void) {
	uint32_t tick = HAL_GetTick();
	if (tick - bb_led_tck < FRAME)
		return;
	bb_led_tck = tick;
	if (bb_led_frame_handler(ihndlr))
		ws2812_send_spi_DMA();
	ihndlr = 0;
}

void bb_led_set_handler(uint8_t (*handler)(uint8_t)) {
	if (bb_led_frame_handler == handler)
		return;
	ihndlr = 1;
	bb_led_frame_handler = handler;
}

uint32_t bb_led_no(uint32_t x, uint32_t y) {
	return (y | 1) * BB_LED_W + ((0 - (y & 1)) ^ x);
}
/*
 *  0    x         25-x    25..0     00
 *  1    51-x      x+26    26..51    01
 *  2    x+52      77-x    77..52    10
 *  3    103-x     x+78    78..103   11
 *                        129..104  100
 */

uint8_t bb_led_fhdl_running(uint8_t init) {
	static uint16_t frame;
	if (init) frame = 0;
	uint8_t changed = 0;
	static const ws2812_color colors[] = {
		{ 128, 0, 0}, {0, 128, 0}, {0, 0, 128}
	};
	for (uint8_t i = 0; i < 9; i++) {
		for (uint8_t j = 0; j < 3; j++) {
			uint32_t x = i*3+((frame+j) % 3);
			if (x < BB_LED_W) {
				for (uint32_t y = 0; y < BB_LED_H; y++)
				   changed |= ws2812_set_led_step(bb_led_no(x, y), STEP, colors[j]);
			}
		}
	}

	if (!changed)
		frame++;

	return changed;
}

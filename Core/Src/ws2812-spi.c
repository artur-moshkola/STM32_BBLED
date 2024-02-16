#include "main.h"
#include "ws2812-spi.h"
#include <string.h>

uint8_t ws2812_leds[WS2812_NUM_LEDS][3];
uint8_t ws2812_buffer[WS2812_BUFFER_SIZE];

void ws2812_init(void) {
    memset(ws2812_leds, 0, sizeof(ws2812_leds));
    memset(ws2812_buffer, 0, WS2812_BUFFER_SIZE);
    ws2812_send_spi();
}

void ws2812_convert(void) {
	uint8_t *bptr = ws2812_buffer + WS2812_RESET_BYTES;
	for (uint32_t i = 0; i < WS2812_NUM_LEDS; i++) {
		for (uint32_t j = 0; j < 3; j++) {
			uint32_t bit;
			uint32_t data = 0;
			uint8_t offset = 21;
			for (uint8_t mask = 0b10000000; mask; mask >>= 1) {
				if (ws2812_leds[i][j] & mask) bit = WS2812_SPI_1;
				else bit = WS2812_SPI_0;
				data |= bit << offset;
				offset -= 3;
			}
			*bptr++ = data >> 16;
			*bptr++ = data >> 8;
			*bptr++ = data;
		}
	}
}

void ws2812_send_spi(void) {
	ws2812_convert();
	HAL_SPI_Transmit(&WS2812_SPI_HANDLE, ws2812_buffer, WS2812_BUFFER_SIZE, HAL_MAX_DELAY);
}

void ws2812_send_spi_DMA(void) {
	ws2812_convert();
    HAL_SPI_Transmit_DMA(&WS2812_SPI_HANDLE, ws2812_buffer, WS2812_BUFFER_SIZE);
}

#define WS2812_SET_LED(LED_NO) \
	ws2812_leds[LED_NO][WS2812_G] = g; \
	ws2812_leds[LED_NO][WS2812_R] = r; \
	ws2812_leds[LED_NO][WS2812_B] = b;

void ws2812_set_led(uint32_t led_no, uint8_t r, uint8_t g, uint8_t b) {
	WS2812_SET_LED(led_no);
}

void ws2812_set_led_safe(uint32_t led_no, uint8_t r, uint8_t g, uint8_t b) {
	if (led_no >= WS2812_NUM_LEDS)
		return;

	WS2812_SET_LED(led_no)
}

#define WS2812_SET_LED_STEP_COLOR(LED_NO, COLOR_I, TARGET) \
	diff = TARGET - ws2812_leds[LED_NO][COLOR_I]; \
	changed |= (diff != 0); \
	if (diff > 0) \
		ws2812_leds[LED_NO][COLOR_I] += (diff < step) ? diff : step; \
	else \
		ws2812_leds[LED_NO][COLOR_I] += (diff > -step) ? diff : -step;

uint8_t ws2812_set_led_step(uint32_t led_no, uint8_t step, uint8_t r, uint8_t g, uint8_t b) {
	uint8_t changed = 0;
	if (led_no >= WS2812_NUM_LEDS)
		return changed;

	int16_t diff;
	WS2812_SET_LED_STEP_COLOR(led_no, WS2812_G, g)
	WS2812_SET_LED_STEP_COLOR(led_no, WS2812_R, r)
	WS2812_SET_LED_STEP_COLOR(led_no, WS2812_B, b)

	return changed;
}

void ws2812_set_all_leds(uint8_t r, uint8_t g, uint8_t b) {
    for (uint16_t i = 0; i < WS2812_NUM_LEDS; i++) {
    	WS2812_SET_LED(i)
    }
}

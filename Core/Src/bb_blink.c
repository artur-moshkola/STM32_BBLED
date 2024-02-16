#include "main.h"
#include "bb_blink.h"

uint32_t bb_blink_pattern;

void bb_blink_init(void) {
#ifdef DEBUG
	bb_blink_pattern = 0b10101010101010101010101010101010;
#else
	bb_blink_pattern = 0b1010;
#endif
}

void bb_blink_tick(void) {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, (bb_blink_pattern >> ((HAL_GetTick() % 3200) / 100)) & 1 ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

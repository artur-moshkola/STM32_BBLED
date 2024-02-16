#include "main.h"
#include "bb_keys.h"

typedef struct bb_keys_btn {
	uint32_t tick;
	uint8_t state;
} bb_keys_btn;

bb_keys_btn bb_keys_intr[BUTTONS];
uint8_t bb_keys_states[BUTTONS];
uint8_t bb_keys_flags[BUTTONS];

void bb_keys_init(void) {
	for (uint8_t i = 0; i < BUTTONS; i++) {
		bb_keys_intr[i].tick = 0;
		bb_keys_intr[i].state = 0;
	}
}

void bb_keys_tick(void) {
	for (uint8_t i = 0; i < BUTTONS; i++) {
		GPIO_PinState pin = HAL_GPIO_ReadPin(bb_keys_pins[i].GPIOx, bb_keys_pins[i].GPIO_Pin);
		if (pin == GPIO_PIN_RESET) { // Button is down
			if (bb_keys_intr[i].state == 1) { // Debouncing down
				if (HAL_GetTick() - bb_keys_intr[i].tick > 50) { // Actual button down
					bb_keys_states[i] <<= 1;
					bb_keys_states[i] |= 1;
					bb_keys_intr[i].state = 0;
					bb_keys_flags[i] |= ((bb_keys_states[i] & 0b11) == 0b01);
				}
			} else if (bb_keys_intr[i].state == 0 && !(bb_keys_states[i] & 1)) { // Just pressed
				bb_keys_intr[i].state = 1;
				bb_keys_intr[i].tick = HAL_GetTick();
			}
		} else { // Button is up
			if (bb_keys_intr[i].state == 2) { // Debouncing up
				if (HAL_GetTick() - bb_keys_intr[i].tick > 50) { // Actual button up
					bb_keys_states[i] <<= 1;
					bb_keys_intr[i].state = 0;
				}
			} else if (bb_keys_intr[i].state == 0 && (bb_keys_states[i] & 1)) { // Just released
				bb_keys_intr[i].state = 2;
				bb_keys_intr[i].tick = HAL_GetTick();
			}
		}
	}
}

uint8_t bb_keys_check(uint8_t btn) {
	return bb_keys_flags[btn];
}

uint8_t bb_keys_check_reset(uint8_t btn) {
	uint8_t val = bb_keys_flags[btn];
	bb_keys_flags[btn] = 0;
	return val;
}

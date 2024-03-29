#define BUTTONS 4

#define BTN_X 0
#define BTN_R 1
#define BTN_G 2
#define BTN_B 3

static const struct {
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
} bb_keys_pins[BUTTONS] = {
	{ GPIOA, GPIO_PIN_8 },
	{ GPIOA, GPIO_PIN_9 },
	{ GPIOA, GPIO_PIN_10 },
	{ GPIOA, GPIO_PIN_11 }
};

void bb_keys_init(void);
void bb_keys_tick(void);

uint8_t bb_keys_check(uint8_t btn);
uint8_t bb_keys_check_reset(uint8_t btn);

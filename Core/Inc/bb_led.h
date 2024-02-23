#define FRAME 40
#define STEP 8
#define BB_LED_W 26
#define BB_LED_H 4

void bb_led_init(void);
void bb_led_tick(void);
void bb_led_set_handler(uint8_t (*handler)(uint8_t));

uint8_t bb_led_fhdl_running(uint8_t init);

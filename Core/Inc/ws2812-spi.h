#define WS2812_NUM_LEDS 104
#define WS2812_SPI_HANDLE hspi1

#define WS2812_RESET_BYTES 20
#define WS2812_BUFFER_SIZE (WS2812_NUM_LEDS * 9 + WS2812_RESET_BYTES * 2)
#define WS2812_SPI_0 0b100
#define WS2812_SPI_1 0b110

#define WS2812_G 0
#define WS2812_R 1
#define WS2812_B 2

extern SPI_HandleTypeDef WS2812_SPI_HANDLE;
extern uint8_t ws2812_leds[WS2812_NUM_LEDS][3];

void ws2812_init(void);
void ws2812_send_spi(void);
void ws2812_send_spi_DMA(void);
void ws2812_set_led(uint32_t led_no, uint8_t r, uint8_t g, uint8_t b);
void ws2812_set_all_leds(uint8_t r, uint8_t g, uint8_t b);
void ws2812_set_led_safe(uint32_t led_no, uint8_t r, uint8_t g, uint8_t b);
uint8_t ws2812_set_led_step(uint32_t led_no, uint8_t step, uint8_t r, uint8_t g, uint8_t b);

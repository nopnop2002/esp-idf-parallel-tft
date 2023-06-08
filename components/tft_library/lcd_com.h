#ifndef   __LCD_COM_H__
#define   __LCD_COM_H__

#include "i2s_lcd_driver.h"
#include "driver/adc.h"

#define TFTLCD_DELAY	0xFFFF
#define TFTLCD_DELAY16	0xFFFF
#define TFTLCD_DELAY8	0x7F

typedef enum {INTERFACE_I2S, INTERFACE_GPIO, INTERFACE_REG} INTERFACE;


typedef enum {MODE_RESET, MODE_OUTPUT, MODE_INPUT} MODE;

#define GPIO_PORT_NUM  0
#define NUM_SAMPLES    2 // Number of samples
#define COMP_TOLERANCE 2 // Coordinate tolerance
#define AVERAGE_TIME   4 // Number of samples when averaging

typedef struct {
	uint16_t _width;
	uint16_t _height;
	uint16_t _offsetx;
	uint16_t _offsety;
	uint16_t _font_direction;
	uint16_t _font_fill;
	uint16_t _font_fill_color;
	uint16_t _font_underline;
	uint16_t _font_underline_color;
	int16_t _rd;
	int16_t _wr;
	int16_t _rs;
	int16_t _cs;
	int16_t _d0;
	int16_t _d1;
	int16_t _d2;
	int16_t _d3;
	int16_t _d4;
	int16_t _d5;
	int16_t _d6;
	int16_t _d7;
	int16_t _delay;
	int16_t _interface;
	bool _debug;
	i2s_lcd_handle_t i2s_lcd_handle;
	adc1_channel_t _adc_yp;
	adc1_channel_t _adc_xm;
	int16_t _gpio_xp;
	int16_t _gpio_xm;
	int16_t _gpio_yp;
	int16_t _gpio_ym;
	int16_t _rawxp;
	int16_t _rawyp;
	bool _calibration;
	int16_t _min_xp; // Minimum xp calibration
	int16_t _min_yp; // Minimum yp calibration
	int16_t _max_xp; // Maximum xp calibration
	int16_t _max_yp; // Maximum yp calibration
	int16_t _min_xc; // Minimum x coordinate
	int16_t _min_yc; // Minimum y coordinate
	int16_t _max_xc; // Maximum x coordinate
	int16_t _max_yc; // Maximum y coordinate
} TFT_t;

void gpio_digital_write(int GPIO_PIN, uint8_t data);
void gpio_lcd_write_data(int dummy1, unsigned char *data, size_t size);
void reg_lcd_write_data(int dummy1, unsigned char *data, size_t size);

void lcd_write_table(TFT_t * dev, const void *table, int16_t size);
void lcd_write_table16(TFT_t * dev, const void *table, int16_t size);
void lcd_write_comm_byte(TFT_t * dev, uint8_t cmd);
void lcd_write_comm_word(TFT_t * dev, uint16_t cmd);
void lcd_write_data_byte(TFT_t * dev, uint8_t data);
void lcd_write_data_word(TFT_t * dev, uint16_t data);
void lcd_write_addr(TFT_t * dev, uint16_t addr1, uint16_t addr2);
void lcd_write_color(TFT_t * dev, uint16_t color, uint16_t size);
void lcd_write_colors(TFT_t * dev, uint16_t * colors, uint16_t size);
void lcd_delay_ms(int delay_time);
void lcd_write_register_word(TFT_t * dev, uint16_t addr, uint16_t data);
void lcd_write_register_byte(TFT_t * dev, uint8_t addr, uint16_t data);
esp_err_t lcd_interface_cfg(TFT_t * dev, int interface);

void touch_interface_cfg(TFT_t * dev, int adc_yp, int adc_xm, int gpio_xp, int gpio_xm, int gpio_yp, int gpio_ym);
int touch_avr_analog(adc1_channel_t channel, int averagetime);
void touch_gpio(int gpio, int mode, int level);
int touch_getx(TFT_t * dev);
int touch_gety(TFT_t * dev);
int touch_getz(TFT_t * dev);
void touch_getxyz(TFT_t * dev, int *xp, int *yp, int *zp);
bool touch_getxy(TFT_t *dev, int *xp, int *yp);

#endif

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "soc/soc.h"
#include "soc/dport_reg.h"
#include "driver/gpio.h"
#include "lcd_com.h"
#include "i2s_lcd.h"

#define TAG "LCD_COM"

#if CONFIG_CUSTOM_GPIO_CONFIG
#define LCD_D0_PIN  CONFIG_D0_GPIO
#define LCD_D1_PIN  CONFIG_D1_GPIO
#define LCD_D2_PIN  CONFIG_D2_GPIO
#define LCD_D3_PIN  CONFIG_D3_GPIO
#define LCD_D4_PIN  CONFIG_D4_GPIO
#define LCD_D5_PIN  CONFIG_D5_GPIO
#define LCD_D6_PIN  CONFIG_D6_GPIO
#define LCD_D7_PIN  CONFIG_D7_GPIO
#define LCD_RD_PIN  CONFIG_RD_GPIO
#define LCD_WR_PIN  CONFIG_WR_GPIO
#define LCD_RS_PIN  CONFIG_RS_GPIO
#define LCD_CS_PIN  CONFIG_CS_GPIO
#define LCD_RESET_PIN CONFIG_RESET_GPIO
#else
#define LCD_D0_PIN  (19)
#define LCD_D1_PIN  (21)
#define LCD_D2_PIN  (25)
#define LCD_D3_PIN  (22)
#define LCD_D4_PIN  (23)
#define LCD_D5_PIN  (33)
#define LCD_D6_PIN  (32)
#define LCD_D7_PIN  (27)
#define LCD_RD_PIN  (2)
#define LCD_WR_PIN  (13)
#define LCD_RS_PIN  (4)
#define LCD_CS_PIN  (14)
#define LCD_RESET_PIN (15)
#endif

void lcd_write_table(TFT_t * dev, const void *table, int16_t size)
{
    int i;
    uint8_t *p = (uint8_t *) table;
    while (size > 0) {
        uint8_t cmd = *p++;
        uint8_t len = *p++;
        if (cmd == TFTLCD_DELAY8) {
            lcd_delay_ms(len);
            len = 0;
        } else {
            lcd_write_comm_byte(dev, cmd );
            for (i = 0; i < len; i++) {
              uint8_t data = *p++;
              lcd_write_data_byte(dev, data );
            }
        }
        size -= len + 2;
    }
}

void lcd_write_table16(TFT_t * dev, const void *table, int16_t size)
{
	uint16_t *p = (uint16_t *) table;
	while (size > 0) {
		uint16_t cmd = *p++;
		uint16_t dat = *p++;
		if (cmd == TFTLCD_DELAY16)
		    lcd_delay_ms(dat);
		else {
			lcd_write_register_word(dev, cmd, dat);
		}
		size -= 2 * sizeof(int16_t);
	}
}


void lcd_write_comm_byte(TFT_t * dev, uint8_t cmd)
{
	char c[1];
    c[0] = cmd;

	gpio_set_level(dev->_cs, 0);
	gpio_set_level(dev->_rs, 0);
	gpio_set_level(dev->_rd, 1);
    i2s_lcd_write_data(I2S_PORT_NUM, c, 1, 100);
	gpio_set_level(dev->_cs, 1);
	if (dev->_delay != 0) ets_delay_us(dev->_delay);
}

void lcd_write_comm_word(TFT_t * dev, uint16_t cmd)
{
	char c[2];
	c[0] = (cmd >> 8) & 0xFF;
	c[1] = cmd & 0xFF;

	gpio_set_level(dev->_cs, 0);
	gpio_set_level(dev->_rs, 0);
	gpio_set_level(dev->_rd, 1);
    i2s_lcd_write_data(I2S_PORT_NUM, c, 2, 100);
	gpio_set_level(dev->_cs, 1);
	if (dev->_delay != 0) ets_delay_us(dev->_delay);
}

void lcd_write_data_byte(TFT_t * dev, uint8_t data)
{
	char d[1];
    d[0] = data;

	gpio_set_level(dev->_cs, 0);
	gpio_set_level(dev->_rs, 1);
	gpio_set_level(dev->_rd, 1);
    i2s_lcd_write_data(I2S_PORT_NUM, d, 1, 100);
	gpio_set_level(dev->_cs, 1);
	if (dev->_delay != 0) ets_delay_us(dev->_delay);
}


void lcd_write_data_word(TFT_t * dev, uint16_t data)
{
	char d[2];
	d[0] = (data >> 8) & 0xFF;
	d[1] = data & 0xFF;

	gpio_set_level(dev->_cs, 0);
	gpio_set_level(dev->_rs, 1);
	gpio_set_level(dev->_rd, 1);
    i2s_lcd_write_data(I2S_PORT_NUM, d, 2, 100);
	gpio_set_level(dev->_cs, 1);
	if (dev->_delay != 0) ets_delay_us(dev->_delay);
}

void lcd_write_addr(TFT_t * dev, uint16_t addr1, uint16_t addr2)
{
	char c[4];
	c[0] = (addr1 >> 8) & 0xFF;
	c[1] = addr1 & 0xFF;
	c[2] = (addr2 >> 8) & 0xFF;
	c[3] = addr2 & 0xFF;
	//ESP_LOGI(__FUNCTION__, "c=%02x-%02x-%02x-%02x",c[0],c[1],c[2],c[3]);

	gpio_set_level(dev->_cs, 0);
	gpio_set_level(dev->_rs, 1);
	gpio_set_level(dev->_rd, 1);
    i2s_lcd_write_data(I2S_PORT_NUM, c, 4, 100);
	gpio_set_level(dev->_cs, 1);
	if (dev->_delay != 0) ets_delay_us(dev->_delay);
}

void lcd_write_color(TFT_t * dev, uint16_t color, uint16_t size)
{
	char *data;
	if ((data = malloc(size * 2)) == NULL) return;
	int index = 0;
	for(int i=0;i<size;i++) {
		data[index++] = (color >> 8) & 0xFF;
		data[index++] = color & 0xFF;
	}

	gpio_set_level(dev->_cs, 0);
	gpio_set_level(dev->_rs, 1);
	gpio_set_level(dev->_rd, 1);
    i2s_lcd_write_data(I2S_PORT_NUM, data, size*2, 100);
	gpio_set_level(dev->_cs, 1);
	free(data);
	if (dev->_delay != 0) ets_delay_us(dev->_delay);
}

void lcd_write_colors(TFT_t * dev, uint16_t * colors, uint16_t size)
{
	char *data;
	if ((data = malloc(size * 2)) == NULL) return;
	int index = 0;
	for(int i=0;i<size;i++) {
		data[index++] = (colors[i] >> 8) & 0xFF;
		data[index++] = colors[i] & 0xFF;
	}

	gpio_set_level(dev->_cs, 0);
	gpio_set_level(dev->_rs, 1);
	gpio_set_level(dev->_rd, 1);
    i2s_lcd_write_data(I2S_PORT_NUM, data, size*2, 100);
	gpio_set_level(dev->_cs, 1);
	free(data);
	if (dev->_delay != 0) ets_delay_us(dev->_delay);
}

void lcd_delay_ms(int delay_time)
{
    vTaskDelay(delay_time/portTICK_RATE_MS);
}

void lcd_write_register_word(TFT_t * dev, uint16_t addr, uint16_t data)
{
	lcd_write_comm_word(dev, addr);
	lcd_write_data_word(dev, data);
}

void lcd_write_register_byte(TFT_t * dev, uint8_t addr, uint16_t data)
{
	lcd_write_comm_byte(dev, addr);
	lcd_write_data_word(dev, data);
}

void lcd_interface_cfg(TFT_t * dev)
{
	ESP_LOGI(TAG, "LCD_CS_PIN=%d",LCD_CS_PIN);
	gpio_pad_select_gpio( LCD_CS_PIN );
	gpio_set_direction( LCD_CS_PIN, GPIO_MODE_OUTPUT );
	gpio_set_level( LCD_CS_PIN, 1 );

	ESP_LOGI(TAG, "LCD_RS_PIN=%d",LCD_RS_PIN);
	gpio_pad_select_gpio( LCD_RS_PIN );
    gpio_set_direction(LCD_RS_PIN, GPIO_MODE_OUTPUT);
	gpio_set_level( LCD_RS_PIN, 1 );

#if 0
	ESP_LOGI(TAG, "LCD_WR_PIN=%d",LCD_WR_PIN);
	gpio_pad_select_gpio( LCD_WR_PIN );
	gpio_set_direction( LCD_WR_PIN, GPIO_MODE_OUTPUT );
	gpio_set_level( LCD_WR_PIN, 1 );
#endif

	ESP_LOGI(TAG, "LCD_RD_PIN=%d",LCD_RD_PIN);
	gpio_pad_select_gpio( LCD_RD_PIN );
	gpio_set_direction( LCD_RD_PIN, GPIO_MODE_OUTPUT );
	gpio_set_level( LCD_RD_PIN, 1 );

#if 1
    i2s_paral_pin_config_t pin_conf = {
        .data_width = 8,
        .data_io_num = {
            LCD_D0_PIN,  LCD_D1_PIN,  LCD_D2_PIN,  LCD_D3_PIN,
            LCD_D4_PIN,  LCD_D5_PIN,  LCD_D6_PIN,  LCD_D7_PIN,
    },
#if 1
        .ws_io_num = LCD_WR_PIN,
#endif
    };
    i2s_lcd_driver_install(I2S_PORT_NUM);
    i2s_lcd_pin_config(I2S_PORT_NUM, &pin_conf);

#endif

	ESP_LOGI(TAG, "LCD_RESET_PIN=%d",LCD_RESET_PIN);
	gpio_pad_select_gpio( LCD_RESET_PIN );
	gpio_set_direction( LCD_RESET_PIN, GPIO_MODE_OUTPUT );
	gpio_set_level( LCD_RESET_PIN, 1 );
	vTaskDelay( pdMS_TO_TICKS( 100 ) );
	gpio_set_level( LCD_RESET_PIN, 0 );
	vTaskDelay( pdMS_TO_TICKS( 100 ) );
	gpio_set_level( LCD_RESET_PIN, 1 );

	dev->_rd = LCD_RD_PIN;
	dev->_wr = LCD_WR_PIN;
	dev->_rs = LCD_RS_PIN;
	dev->_cs = LCD_CS_PIN;
}

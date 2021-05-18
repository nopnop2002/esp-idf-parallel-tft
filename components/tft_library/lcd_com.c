#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "soc/soc.h"
#include "soc/dport_reg.h"
#include "lcd_com.h"
#include "i2s_lcd_driver.h"
#include "driver/gpio.h"

#define TAG "LCD_COM"

#if CONFIG_CUSTOM_GPIO_CONFIG
#define LCD_D0_PIN	CONFIG_D0_GPIO
#define LCD_D1_PIN	CONFIG_D1_GPIO
#define LCD_D2_PIN	CONFIG_D2_GPIO
#define LCD_D3_PIN	CONFIG_D3_GPIO
#define LCD_D4_PIN	CONFIG_D4_GPIO
#define LCD_D5_PIN	CONFIG_D5_GPIO
#define LCD_D6_PIN	CONFIG_D6_GPIO
#define LCD_D7_PIN	CONFIG_D7_GPIO
#define LCD_RD_PIN	CONFIG_RD_GPIO
#define LCD_WR_PIN	CONFIG_WR_GPIO
#define LCD_RS_PIN	CONFIG_RS_GPIO
#define LCD_CS_PIN	CONFIG_CS_GPIO
#define LCD_RESET_PIN CONFIG_RESET_GPIO
#else
#ifdef CONFIG_IDF_TARGET_ESP32
#define LCD_D0_PIN	(12)
#define LCD_D1_PIN	(13)
#define LCD_D2_PIN	(26)
#define LCD_D3_PIN	(25)
#define LCD_D4_PIN	(17)
#define LCD_D5_PIN	(16)
#define LCD_D6_PIN	(27)
#define LCD_D7_PIN	(14)
#define LCD_RD_PIN	(2)
#define LCD_WR_PIN	(4)
#define LCD_RS_PIN	(15)
#define LCD_CS_PIN	(33)
#define LCD_RESET_PIN (32)
#elif defined CONFIG_IDF_TARGET_ESP32S2
#define LCD_D0_PIN	(1)
#define LCD_D1_PIN	(2)
#define LCD_D2_PIN	(3)
#define LCD_D3_PIN	(4)
#define LCD_D4_PIN	(5)
#define LCD_D5_PIN	(6)
#define LCD_D6_PIN	(7)
#define LCD_D7_PIN	(8)
#define LCD_RD_PIN	(39)
#define LCD_WR_PIN	(40)
#define LCD_RS_PIN	(41)
#define LCD_CS_PIN	(42)
#define LCD_RESET_PIN (45)
#endif
#endif

#define gpio_digital_write(GPIO_PIN, data) \
	do { \
		if (data) { \
			gpio_set_level( GPIO_PIN, 1 ); \
		} else { \
			gpio_set_level( GPIO_PIN, 0 ); \
		} \
	} while (0)
	

#define reg_digital_write(GPIO_PIN, data) \
	do { \
		if (data) { \
			GPIO.out_w1ts = (1 << GPIO_PIN); \
		} else { \
			GPIO.out_w1tc = (1 << GPIO_PIN); \
		} \
	} while (0)

void gpio_lcd_write_data(int dummy1, unsigned char *data, size_t size) {
	for (int i=0;i<size;i++) {
		gpio_digital_write(LCD_D0_PIN, data[i] & 0x01);
		gpio_digital_write(LCD_D1_PIN, data[i] & 0x02);
		gpio_digital_write(LCD_D2_PIN, data[i] & 0x04);
		gpio_digital_write(LCD_D3_PIN, data[i] & 0x08);
		gpio_digital_write(LCD_D4_PIN, data[i] & 0x10); 
		gpio_digital_write(LCD_D5_PIN, data[i] & 0x20);
		gpio_digital_write(LCD_D6_PIN, data[i] & 0x40);
		gpio_digital_write(LCD_D7_PIN, data[i] & 0x80);  
		gpio_set_level( LCD_WR_PIN, 0 );
		gpio_set_level( LCD_WR_PIN, 1 );
	}
}

void reg_lcd_write_data(int dummy1, unsigned char *data, size_t size) {
	for (int i=0;i<size;i++) {
		reg_digital_write(LCD_D0_PIN, data[i] & 0x01);
		reg_digital_write(LCD_D1_PIN, data[i] & 0x02);
		reg_digital_write(LCD_D2_PIN, data[i] & 0x04);
		reg_digital_write(LCD_D3_PIN, data[i] & 0x08);
		reg_digital_write(LCD_D4_PIN, data[i] & 0x10); 
		reg_digital_write(LCD_D5_PIN, data[i] & 0x20);
		reg_digital_write(LCD_D6_PIN, data[i] & 0x40);
		reg_digital_write(LCD_D7_PIN, data[i] & 0x80);	
		gpio_set_level( LCD_WR_PIN, 0 );
		gpio_set_level( LCD_WR_PIN, 1 );
	}
}


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
	unsigned char c[1];
	c[0] = cmd;

	gpio_set_level(dev->_cs, 0);
	gpio_set_level(dev->_rs, 0);
	//gpio_set_level(dev->_rd, 1);
	if (dev->_interface == INTERFACE_I2S) {
		i2s_lcd_write(dev->i2s_lcd_handle, c, 1);
	} else if (dev->_interface == INTERFACE_GPIO) {
		gpio_lcd_write_data(GPIO_PORT_NUM, c, 1);
	} else if (dev->_interface == INTERFACE_REG) {
		reg_lcd_write_data(GPIO_PORT_NUM, c, 1);
	}
	gpio_set_level(dev->_cs, 1);
	if (dev->_delay != 0) ets_delay_us(dev->_delay);
}

void lcd_write_comm_word(TFT_t * dev, uint16_t cmd)
{
	unsigned char c[2];
	c[0] = (cmd >> 8) & 0xFF;
	c[1] = cmd & 0xFF;

	gpio_set_level(dev->_cs, 0);
	gpio_set_level(dev->_rs, 0);
	//gpio_set_level(dev->_rd, 1);
	if (dev->_interface == INTERFACE_I2S) {
		i2s_lcd_write(dev->i2s_lcd_handle, c, 2);
	} else if (dev->_interface == INTERFACE_GPIO) {
		gpio_lcd_write_data(GPIO_PORT_NUM, c, 2);
	} else if (dev->_interface == INTERFACE_REG) {
		reg_lcd_write_data(GPIO_PORT_NUM, c, 2);
	}
	gpio_set_level(dev->_cs, 1);
	if (dev->_delay != 0) ets_delay_us(dev->_delay);
}

void lcd_write_data_byte(TFT_t * dev, uint8_t data)
{
	unsigned char d[1];
	d[0] = data;

	gpio_set_level(dev->_cs, 0);
	gpio_set_level(dev->_rs, 1);
	//gpio_set_level(dev->_rd, 1);
	if (dev->_interface == INTERFACE_I2S) {
		i2s_lcd_write(dev->i2s_lcd_handle, d, 1);
	} else if (dev->_interface == INTERFACE_GPIO) {
		gpio_lcd_write_data(GPIO_PORT_NUM, d, 1);
	} else if (dev->_interface == INTERFACE_REG) {
		reg_lcd_write_data(GPIO_PORT_NUM, d, 1);
	}
	gpio_set_level(dev->_cs, 1);
	if (dev->_delay != 0) ets_delay_us(dev->_delay);
}


void lcd_write_data_word(TFT_t * dev, uint16_t data)
{
	unsigned char d[2];
	d[0] = (data >> 8) & 0xFF;
	d[1] = data & 0xFF;

	gpio_set_level(dev->_cs, 0);
	gpio_set_level(dev->_rs, 1);
	//gpio_set_level(dev->_rd, 1);
	if (dev->_interface == INTERFACE_I2S) {
		i2s_lcd_write(dev->i2s_lcd_handle, d, 2);
	} else if (dev->_interface == INTERFACE_GPIO) {
		gpio_lcd_write_data(GPIO_PORT_NUM, d, 2);
	} else if (dev->_interface == INTERFACE_REG) {
		reg_lcd_write_data(GPIO_PORT_NUM, d, 2);
	}
	gpio_set_level(dev->_cs, 1);
	if (dev->_delay != 0) ets_delay_us(dev->_delay);
}

void lcd_write_addr(TFT_t * dev, uint16_t addr1, uint16_t addr2)
{
	unsigned char c[4];
	c[0] = (addr1 >> 8) & 0xFF;
	c[1] = addr1 & 0xFF;
	c[2] = (addr2 >> 8) & 0xFF;
	c[3] = addr2 & 0xFF;
	//ESP_LOGI(__FUNCTION__, "c=%02x-%02x-%02x-%02x",c[0],c[1],c[2],c[3]);

	gpio_set_level(dev->_cs, 0);
	gpio_set_level(dev->_rs, 1);
	//gpio_set_level(dev->_rd, 1);
	if (dev->_interface == INTERFACE_I2S) {
		i2s_lcd_write(dev->i2s_lcd_handle, c, 4);
	} else if (dev->_interface == INTERFACE_GPIO) {
		gpio_lcd_write_data(GPIO_PORT_NUM, c, 4);
	} else if (dev->_interface == INTERFACE_REG) {
		reg_lcd_write_data(GPIO_PORT_NUM, c, 4);
	}
	gpio_set_level(dev->_cs, 1);
	if (dev->_delay != 0) ets_delay_us(dev->_delay);
}

void lcd_write_color(TFT_t * dev, uint16_t color, uint16_t size)
{
	unsigned char *data;
	if ((data = malloc(size * 2)) == NULL) return;
	int index = 0;
	for(int i=0;i<size;i++) {
		data[index++] = (color >> 8) & 0xFF;
		data[index++] = color & 0xFF;
	}

	gpio_set_level(dev->_cs, 0);
	gpio_set_level(dev->_rs, 1);
	//gpio_set_level(dev->_rd, 1);
	if (dev->_interface == INTERFACE_I2S) {
		i2s_lcd_write(dev->i2s_lcd_handle, data, size*2);
	} else if (dev->_interface == INTERFACE_GPIO) {
		gpio_lcd_write_data(GPIO_PORT_NUM, data, size*2);
	} else if (dev->_interface == INTERFACE_REG) {
		reg_lcd_write_data(GPIO_PORT_NUM, data, size*2);
	}
	gpio_set_level(dev->_cs, 1);
	free(data);
	if (dev->_delay != 0) ets_delay_us(dev->_delay);
}

void lcd_write_colors(TFT_t * dev, uint16_t * colors, uint16_t size)
{
	unsigned char *data;
	if ((data = malloc(size * 2)) == NULL) return;
	int index = 0;
	for(int i=0;i<size;i++) {
		data[index++] = (colors[i] >> 8) & 0xFF;
		data[index++] = colors[i] & 0xFF;
	}

	gpio_set_level(dev->_cs, 0);
	gpio_set_level(dev->_rs, 1);
	//gpio_set_level(dev->_rd, 1);
	if (dev->_interface == INTERFACE_I2S) {
		i2s_lcd_write(dev->i2s_lcd_handle, data, size*2);
	} else if (dev->_interface == INTERFACE_GPIO) {
		gpio_lcd_write_data(GPIO_PORT_NUM, data, size*2);
	} else if (dev->_interface == INTERFACE_REG) {
		reg_lcd_write_data(GPIO_PORT_NUM, data, size*2);
	}
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

#define BOARD_LCD_I2S_BITWIDTH 8

esp_err_t lcd_interface_cfg(TFT_t * dev, int interface)
{
	if (interface == INTERFACE_I2S) {
		ESP_LOGI(TAG, "interface=I2S");
	} else if (interface == INTERFACE_GPIO) {
		ESP_LOGI(TAG, "interface=GPIO");
	}
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

	if (interface == INTERFACE_I2S) {
		ESP_LOGI(TAG, "INTERFACE is I2S");
		i2s_lcd_config_t i2s_lcd_cfg = {
			.data_width  = BOARD_LCD_I2S_BITWIDTH,
			.pin_data_num = {
				LCD_D0_PIN,
				LCD_D1_PIN,
				LCD_D2_PIN,
				LCD_D3_PIN,
				LCD_D4_PIN,
				LCD_D5_PIN,
				LCD_D6_PIN,
				LCD_D7_PIN,
				//BOARD_LCD_I2S_D8_PIN,
				//BOARD_LCD_I2S_D9_PIN,
				//BOARD_LCD_I2S_D10_PIN,
				//BOARD_LCD_I2S_D11_PIN,
				//BOARD_LCD_I2S_D12_PIN,
				//BOARD_LCD_I2S_D13_PIN,
				//BOARD_LCD_I2S_D14_PIN,
				//BOARD_LCD_I2S_D15_PIN,
			},
			.pin_num_cs = LCD_CS_PIN,
			.pin_num_wr = LCD_WR_PIN,
			.pin_num_rs = LCD_RS_PIN,
	
			.clk_freq = 20000000,
			.i2s_port = I2S_NUM_0,
			.buffer_size = 32000,
			.swap_data = false,
		};


		//i2s_lcd_handle_t i2s_lcd_handle;
        dev->i2s_lcd_handle = i2s_lcd_driver_init(&i2s_lcd_cfg);
        if (NULL == dev->i2s_lcd_handle) {
            ESP_LOGE(TAG, "%s:%d (%s):%s", __FILE__, __LINE__, __FUNCTION__, "screen 8080 interface create failed");
            return ESP_FAIL;
        }

	} else if (interface == INTERFACE_GPIO || interface == INTERFACE_REG) {
		if (interface == INTERFACE_GPIO) {
			ESP_LOGI(TAG, "INTERFACE is GPIO");
		} else {
			ESP_LOGI(TAG, "INTERFACE is REGISTER I/O");
		}
		gpio_pad_select_gpio( LCD_D0_PIN );
		gpio_pad_select_gpio( LCD_D1_PIN );
		gpio_pad_select_gpio( LCD_D2_PIN );
		gpio_pad_select_gpio( LCD_D3_PIN );
		gpio_pad_select_gpio( LCD_D4_PIN );
		gpio_pad_select_gpio( LCD_D5_PIN );
		gpio_pad_select_gpio( LCD_D6_PIN );
		gpio_pad_select_gpio( LCD_D7_PIN );
		gpio_pad_select_gpio( LCD_WR_PIN );
		gpio_set_direction( LCD_D0_PIN, GPIO_MODE_OUTPUT );
		gpio_set_direction( LCD_D1_PIN, GPIO_MODE_OUTPUT );
		gpio_set_direction( LCD_D2_PIN, GPIO_MODE_OUTPUT );
		gpio_set_direction( LCD_D3_PIN, GPIO_MODE_OUTPUT );
		gpio_set_direction( LCD_D4_PIN, GPIO_MODE_OUTPUT );
		gpio_set_direction( LCD_D5_PIN, GPIO_MODE_OUTPUT );
		gpio_set_direction( LCD_D6_PIN, GPIO_MODE_OUTPUT );
		gpio_set_direction( LCD_D7_PIN, GPIO_MODE_OUTPUT );
		gpio_set_direction( LCD_WR_PIN, GPIO_MODE_OUTPUT );
		gpio_set_level( LCD_WR_PIN, 1 );
	}


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
	dev->_interface = interface;
	
	return ESP_OK;
}

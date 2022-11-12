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
#include "hal/gpio_ll.h" // idf-py ver5
#include "driver/gpio.h"
//#include "driver/adc_common.h"

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
#elif defined CONFIG_IDF_TARGET_ESP32S2 || CONFIG_IDF_TARGET_ESP32S3
#define LCD_D0_PIN	(1)
#define LCD_D1_PIN	(2)
#define LCD_D2_PIN	(3)
#define LCD_D3_PIN	(4)
#define LCD_D4_PIN	(5)
#define LCD_D5_PIN	(6)
#define LCD_D6_PIN	(7) // (11)
#define LCD_D7_PIN	(8) // (12)
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
	if (dev->_delay != 0) esp_rom_delay_us(dev->_delay);
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
	if (dev->_delay != 0) esp_rom_delay_us(dev->_delay);
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
	if (dev->_delay != 0) esp_rom_delay_us(dev->_delay);
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
	if (dev->_delay != 0) esp_rom_delay_us(dev->_delay);
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
	if (dev->_delay != 0) esp_rom_delay_us(dev->_delay);
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
	if (dev->_delay != 0) esp_rom_delay_us(dev->_delay);
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
	if (dev->_delay != 0) esp_rom_delay_us(dev->_delay);
}

void lcd_delay_ms(int delay_time)
{
	vTaskDelay(delay_time/portTICK_PERIOD_MS);
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
#if 0
	if (interface == INTERFACE_I2S) {
		ESP_LOGI(TAG, "interface=I2S");
	} else if (interface == INTERFACE_GPIO) {
		ESP_LOGI(TAG, "interface=GPIO");
	}
#endif
	ESP_LOGI(TAG, "LCD_CS_PIN=%d",LCD_CS_PIN);
	gpio_reset_pin( LCD_CS_PIN );
	gpio_set_direction( LCD_CS_PIN, GPIO_MODE_OUTPUT );
	gpio_set_level( LCD_CS_PIN, 1 );

	ESP_LOGI(TAG, "LCD_RS_PIN=%d",LCD_RS_PIN);
	gpio_reset_pin( LCD_RS_PIN );
	gpio_set_direction(LCD_RS_PIN, GPIO_MODE_OUTPUT);
	gpio_set_level( LCD_RS_PIN, 1 );

#if 0
	ESP_LOGI(TAG, "LCD_WR_PIN=%d",LCD_WR_PIN);
	gpio_reset_pin( LCD_WR_PIN );
	gpio_set_direction( LCD_WR_PIN, GPIO_MODE_OUTPUT );
	gpio_set_level( LCD_WR_PIN, 1 );
#endif

	ESP_LOGI(TAG, "LCD_RD_PIN=%d",LCD_RD_PIN);
	gpio_reset_pin( LCD_RD_PIN );
	gpio_set_direction( LCD_RD_PIN, GPIO_MODE_OUTPUT );
	gpio_set_level( LCD_RD_PIN, 1 );

	ESP_LOGI(TAG, "LCD_D0_PIN=%d",LCD_D0_PIN);
	ESP_LOGI(TAG, "LCD_D1_PIN=%d",LCD_D1_PIN);
	ESP_LOGI(TAG, "LCD_D2_PIN=%d",LCD_D2_PIN);
	ESP_LOGI(TAG, "LCD_D3_PIN=%d",LCD_D3_PIN);
	ESP_LOGI(TAG, "LCD_D4_PIN=%d",LCD_D4_PIN);
	ESP_LOGI(TAG, "LCD_D5_PIN=%d",LCD_D5_PIN);
	ESP_LOGI(TAG, "LCD_D6_PIN=%d",LCD_D6_PIN);
	ESP_LOGI(TAG, "LCD_D7_PIN=%d",LCD_D7_PIN);

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
		gpio_reset_pin( LCD_D0_PIN );
		gpio_reset_pin( LCD_D1_PIN );
		gpio_reset_pin( LCD_D2_PIN );
		gpio_reset_pin( LCD_D3_PIN );
		gpio_reset_pin( LCD_D4_PIN );
		gpio_reset_pin( LCD_D5_PIN );
		gpio_reset_pin( LCD_D6_PIN );
		gpio_reset_pin( LCD_D7_PIN );
		gpio_reset_pin( LCD_WR_PIN );
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
		dev->_d0 = LCD_D0_PIN;
		dev->_d1 = LCD_D1_PIN;
		dev->_d2 = LCD_D2_PIN;
		dev->_d3 = LCD_D3_PIN;
		dev->_d4 = LCD_D4_PIN;
		dev->_d5 = LCD_D5_PIN;
		dev->_d6 = LCD_D6_PIN;
		dev->_d7 = LCD_D7_PIN;
	}


	ESP_LOGI(TAG, "LCD_RESET_PIN=%d",LCD_RESET_PIN);
	gpio_reset_pin( LCD_RESET_PIN );
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

void touch_interface_cfg(TFT_t * dev, int adc_yp, int adc_xm, int gpio_xp, int gpio_xm, int gpio_yp, int gpio_ym)
{
	ESP_LOGI(TAG, "_adc_yp=%d _adc_xm=%d", dev->_adc_yp, dev->_adc_xm);
	ESP_LOGI(TAG, "_gpio_xp=%d _gpio_xm=%d", gpio_xp, gpio_xm);
	dev->_calibration = true;
	dev->_adc_yp = adc_yp;
	dev->_adc_xm = adc_xm;
	//dev->_gpio_xp = dev->_d6;
	//dev->_gpio_xm = dev->_rs;
	//dev->_gpio_yp = dev->_wr;
	//dev->_gpio_ym = dev->_d7;
	dev->_gpio_xp = gpio_xp;
	dev->_gpio_xm = gpio_xm;
	dev->_gpio_yp = gpio_yp;
	dev->_gpio_ym = gpio_ym;
	ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_DEFAULT));
}

int touch_avr_analog(adc1_channel_t channel, int averagetime)
{
	if (averagetime > 2) {
		int sum = 0;
		int max = 0;
		int min = 1024;
		for(int i = 0; i<averagetime; i++)
		{
			int tmp = adc1_get_raw(channel);
			if (ADC_WIDTH_BIT_DEFAULT == 2) {
				tmp = tmp / 2; // 11 bits -> 10 bits.
			}
			if (ADC_WIDTH_BIT_DEFAULT == 3) {
				tmp = tmp / 4; // 12 bits -> 10 bits.
			}
			if (ADC_WIDTH_BIT_DEFAULT == 4) {
				tmp = tmp / 8; // 13 bits -> 10 bits.
			}
			if(tmp > max)max = tmp;
			if(tmp < min)min = tmp;
			sum += tmp;
			//sum+=analogRead(adpin);
		}
		return (sum-min-max)/(averagetime-2);
	} else {
		return 0;
	}
}

void touch_gpio(int gpio, int mode, int level)
{
	esp_log_level_set("gpio", ESP_LOG_WARN);
	gpio_reset_pin(gpio);
	gpio_config_t io_conf = {};
	io_conf.pin_bit_mask = (1ULL<<gpio);
	if (mode == MODE_OUTPUT) {
		io_conf.mode = GPIO_MODE_OUTPUT;
		ESP_ERROR_CHECK(gpio_config(&io_conf));
		gpio_set_level(gpio, level);
	} else if(mode == MODE_INPUT) {
		io_conf.mode = GPIO_MODE_INPUT;
		io_conf.intr_type = GPIO_INTR_DISABLE;
		io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
		ESP_ERROR_CHECK(gpio_config(&io_conf));
	}
}

int touch_getx(TFT_t * dev)
{
	int xp = 0;
	touch_gpio(dev->_gpio_yp, MODE_INPUT, 0);
	touch_gpio(dev->_gpio_ym, MODE_INPUT, 0);
	touch_gpio(dev->_gpio_xp, MODE_OUTPUT, 1);
	touch_gpio(dev->_gpio_xm, MODE_OUTPUT, 0);

	//ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_DEFAULT));
	ESP_ERROR_CHECK(adc1_config_channel_atten(dev->_adc_yp, ADC_ATTEN_DB_11));
	int samples[NUMSAMPLES];
	for (int i=0; i<NUMSAMPLES; i++) {
		samples[i] = touch_avr_analog(dev->_adc_yp, AVERAGETIME);
	}
	int icomp =  samples[0] > samples[1]? samples[0] - samples[1]: samples[1] -  samples[0];
	ESP_LOGD(TAG, "touch_getx adc=%d samples[0]=%d samples[1]=%d icomp=%d COMP=%d", dev->_adc_yp, samples[0], samples[1], icomp, COMP);
	if(icomp <= COMP) xp = samples[0] + samples[1];
	return xp;
}

int touch_gety(TFT_t * dev)
{
	int yp = 0;
	touch_gpio(dev->_gpio_yp, MODE_OUTPUT, 1);
	touch_gpio(dev->_gpio_ym, MODE_OUTPUT, 0);
	touch_gpio(dev->_gpio_xp, MODE_INPUT, 0);
	touch_gpio(dev->_gpio_xm, MODE_INPUT, 0);

	//ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_DEFAULT));
	ESP_ERROR_CHECK(adc1_config_channel_atten(dev->_adc_xm, ADC_ATTEN_DB_11));
	int samples[NUMSAMPLES];
	for (int i=0; i<NUMSAMPLES; i++) {
		samples[i] = touch_avr_analog(dev->_adc_xm, AVERAGETIME);
	}
	int icomp =  samples[0] > samples[1]? samples[0] - samples[1]: samples[1] -  samples[0];
	ESP_LOGD(TAG, "adc=%d samples[0]=%d samples[1]=%d icomp=%d COMP=%d", dev->_adc_xm, samples[0], samples[1], icomp, COMP);
	if(icomp <= COMP) yp = samples[0] + samples[1];
	return yp;

}

int touch_getz(TFT_t * dev)
{
	touch_gpio(dev->_gpio_yp, MODE_INPUT, 0);
	touch_gpio(dev->_gpio_ym, MODE_OUTPUT, 1);
	touch_gpio(dev->_gpio_xp, MODE_OUTPUT, 0);
	touch_gpio(dev->_gpio_xm, MODE_INPUT, 0);

	//ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_DEFAULT));
	ESP_ERROR_CHECK(adc1_config_channel_atten(dev->_adc_yp, ADC_ATTEN_DB_11));
	ESP_ERROR_CHECK(adc1_config_channel_atten(dev->_adc_xm, ADC_ATTEN_DB_11));
	int z1 =	adc1_get_raw(dev->_adc_yp);
	int z2 =	adc1_get_raw(dev->_adc_xm);

	if (ADC_WIDTH_BIT_DEFAULT == 2) {
		z1 = z1 / 2; // 11 bits -> 10 bits.
		z2 = z2 / 2; // 11 bits -> 10 bits.
	}
	if (ADC_WIDTH_BIT_DEFAULT == 3) {
		z1 = z1 / 4; // 12 bits -> 10 bits.
		z2 = z2 / 4; // 12 bits -> 10 bits.
	}
	if (ADC_WIDTH_BIT_DEFAULT == 4) {
		z1 = z1 / 8; // 13 bits -> 10 bits.
		z2 = z2 / 8; // 13 bits -> 10 bits.
	}

	int icomp =  z1 > z2? z1 - z2: z2 -  z1;
	ESP_LOGD(TAG, "z1=%d z2=%d icomp=%d", z1, z2, icomp);
	return icomp;

#if 0
	float rtouch = 0;
	rtouch	= z2;
	rtouch /= z1;
	rtouch -= 1;
	rtouch *= (2046-dev->_rawxp)/2;
	rtouch *= RXPLATE;
	rtouch /= 1024;
	ESP_LOGI(pcTaskGetTaskName(0), "rtouch=%f", rtouch);
	return rtouch;
#endif
}

void touch_getxyz(TFT_t * dev, int *xp, int *yp, int *zp)
{
#if CONFIG_SWAP_XY
	*yp = touch_getx(dev);
	*xp = touch_gety(dev);
#else
	*xp = touch_getx(dev);
	*yp = touch_gety(dev);
#endif
	*zp = touch_getz(dev);
	touch_gpio(dev->_gpio_yp, MODE_OUTPUT, 0);
	touch_gpio(dev->_gpio_ym, MODE_OUTPUT, 0);
	touch_gpio(dev->_gpio_xp, MODE_OUTPUT, 0);
	touch_gpio(dev->_gpio_xm, MODE_OUTPUT, 0);
}

bool touch_getxy(TFT_t *dev, int *xp, int *yp)
{
	int _xp;
	int _yp;
	int _zp;
	touch_getxyz(dev, &_xp, &_yp, &_zp);
	if (_xp == 0) return false;
	if (_yp == 0) return false;
	if (_zp == 1023) return false;
	*xp = _xp;
	*yp = _yp;
	return true;
}	

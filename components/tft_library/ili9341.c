#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "lcd_com.h"
#include "lcd_lib.h"
#include "ili9341.h"

#define TAG "ILI9341"

#if CONFIG_ILI9340 || CONFIG_ILI9341

void ili9341_lcdInit(TFT_t * dev, int width, int height, int offsetx, int offsety)
{
	lcdInitDevice(dev, width, height, offsetx, offsety);

#if CONFIG_ILI9340
	ESP_LOGI(TAG,"Your TFT is ILI9340");
#elif CONFIG_ILI9341
	ESP_LOGI(TAG,"Your TFT is ILI9341");
#endif
	ESP_LOGI(TAG,"Screen width:%d",width);
	ESP_LOGI(TAG,"Screen height:%d",height);

	// Initailize TFT
	static const uint8_t reset_off[] = {
		0x01, 0,			//Soft Reset
		TFTLCD_DELAY8, 150,
		0x28, 0,			//Display Off
		0x3A, 1, 0x55,		//Pixel read=565, write=565.
	};

	static const uint8_t wake_on[] = {
		0x11, 0,			//Sleep Out
		TFTLCD_DELAY8, 150,
		0x29, 0,			//Display On
	};

	static const uint8_t regValues[] = {
		0xC0, 1, 0x23,
		0xC1, 1, 0x10,
		0xC5, 2, 0x3E, 0x28,
		0xC7, 1, 0x86,
		0x36, 1, 0xC8,
		0xB1, 2, 0x00, 0x18,
		0xB6, 4, 0x0A, 0xA2, 0x27, 0x04,
		0x26, 1, 0x01,
		0xE0,15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
		0xE1,15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
	};
	lcd_write_table(dev, reset_off, sizeof(reset_off));
	lcd_write_table(dev, regValues, sizeof(regValues));
	lcd_write_table(dev, wake_on, sizeof(wake_on));

	// ili9341 custom function
	DrawPixel = ili9341_lcdDrawPixel;
	DrawMultiPixels = ili9341_lcdDrawMultiPixels;
	DrawFillRect = ili9341_lcdDrawFillRect;
	DisplayOff = ili9341_lcdDisplayOff;
	DisplayOn = ili9341_lcdDisplayOn;
	InversionOff = ili9341_lcdInversionOff;
	InversionOn = ili9341_lcdInversionOn;
	EnableScroll = ili9341_lcdEnableScroll;
	SetScrollArea = ili9341_lcdSetScrollArea;
	ResetScrollArea = ili9341_lcdResetScrollArea;
	StartScroll = ili9341_lcdStartScroll;

}
#endif

#if CONFIG_ILI9327 || CONFIG_ILI9340 || CONFIG_ILI9341 || CONFIG_ILI9342 || CONFIG_ILI9481 || CONFIG_ILI9486 || CONFIG_ST7796 || CONFIG_ILI9488

// Draw pixel
// x:X coordinate
// y:Y coordinate
// color:color
void ili9341_lcdDrawPixel(TFT_t * dev, uint16_t x, uint16_t y, uint16_t color){
	if (x >= dev->_width) return;
	if (y >= dev->_height) return;

	uint16_t _x = x + dev->_offsetx;
	uint16_t _y = y + dev->_offsety;

	lcd_write_comm_byte(dev, 0x2A);	// set column(x) address
	lcd_write_addr(dev, _x, _x);
	lcd_write_comm_byte(dev, 0x2B);	// set Page(y) address
	lcd_write_addr(dev, _y, _y);
	lcd_write_comm_byte(dev, 0x2C);	//	Memory Write
	lcd_write_data_word(dev, color);
}

// Draw multi pixel
// x:X coordinate
// y:Y coordinate
// size:Number of colors
// colors:colors
void ili9341_lcdDrawMultiPixels(TFT_t * dev, uint16_t x, uint16_t y, uint16_t size, uint16_t * colors) {
	if (x+size > dev->_width) return;
	if (y >= dev->_height) return;

	ESP_LOGD(TAG,"offset(x)=%d offset(y)=%d",dev->_offsetx,dev->_offsety);
	uint16_t _x1 = x + dev->_offsetx;
	uint16_t _x2 = _x1 + size;
	uint16_t _y1 = y + dev->_offsety;
	uint16_t _y2 = _y1;
	ESP_LOGD(TAG,"_x1=%d _x2=%d _y1=%d _y2=%d",_x1, _x2, _y1, _y2);

	lcd_write_comm_byte(dev, 0x2A);	// set column(x) address
	lcd_write_addr(dev, _x1, _x2);
	lcd_write_comm_byte(dev, 0x2B);	// set Page(y) address
	lcd_write_addr(dev, _y1, _y2);
	lcd_write_comm_byte(dev, 0x2C);	//	Memory Write
	lcd_write_colors(dev, colors, size);
}




// Draw rectangle of filling
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End X coordinate
// y2:End Y coordinate
// color:color
void ili9341_lcdDrawFillRect(TFT_t * dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
	if (x1 >= dev->_width) return;
	if (x2 >= dev->_width) x2=dev->_width-1;
	if (y1 >= dev->_height) return;
	if (y2 >= dev->_height) y2=dev->_height-1;

	ESP_LOGD(TAG,"offset(x)=%d offset(y)=%d",dev->_offsetx,dev->_offsety);
	uint16_t _x1 = x1 + dev->_offsetx;
	uint16_t _x2 = x2 + dev->_offsetx;
	uint16_t _y1 = y1 + dev->_offsety;
	uint16_t _y2 = y2 + dev->_offsety;

	lcd_write_comm_byte(dev, 0x2A);	// set column(x) address
	lcd_write_addr(dev, _x1, _x2);
	lcd_write_comm_byte(dev, 0x2B);	// set Page(y) address
	lcd_write_addr(dev, _y1, _y2);
	lcd_write_comm_byte(dev, 0x2C);	//	Memory Write
	for(int i=_x1;i<=_x2;i++) {
		uint16_t size = _y2-_y1+1;
		lcd_write_color(dev, color, size);
	}
}

// Display OFF
void ili9341_lcdDisplayOff(TFT_t * dev) {
	lcd_write_comm_byte(dev, 0x28);
}
 
// Display ON
void ili9341_lcdDisplayOn(TFT_t * dev) {
	lcd_write_comm_byte(dev, 0x29);
}

// Display Inversion OFF
void ili9341_lcdInversionOff(TFT_t * dev) {
	lcd_write_comm_byte(dev, 0x20);
}

// Display Inversion ON
void ili9341_lcdInversionOn(TFT_t * dev) {
	lcd_write_comm_byte(dev, 0x21);
}

// Enable Vertical Scrolling
bool ili9341_lcdEnableScroll(TFT_t * dev){
	return true;
}


// Vertical Scrolling Definition
// tfa:Top Fixed Area
// vsa:Vertical Scrolling Area
// bfa:Bottom Fixed Area
void ili9341_lcdSetScrollArea(TFT_t * dev, uint16_t tfa, uint16_t vsa, uint16_t bfa){
	lcd_write_comm_byte(dev, 0x33);	// Vertical Scrolling Definition
	lcd_write_data_word(dev, tfa);
	lcd_write_data_word(dev, vsa);
	lcd_write_data_word(dev, bfa);
	//lcd_write_comm_byte(dev, 0x12);	// Partial Mode ON
}

void ili9341_lcdResetScrollArea(TFT_t * dev, uint16_t vsa){
	lcd_write_comm_byte(dev, 0x33);	// Vertical Scrolling Definition
	lcd_write_data_word(dev, 0);
	//lcd_write_data_word(dev, 0x140);
	lcd_write_data_word(dev, vsa);
	lcd_write_data_word(dev, 0);
}

// Vertical Scrolling Start Address
// vsp:Vertical Scrolling Start Address
void ili9341_lcdStartScroll(TFT_t * dev, uint16_t vsp){
	lcd_write_comm_byte(dev, 0x37);	// Vertical Scrolling Start Address
	lcd_write_data_word(dev, vsp);
}
#endif

#include <string.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "lcd_com.h"
#include "lcd_lib.h"
#include "ili9325.h"

#define TAG "ILI9325"

#if CONFIG_ILI9325 || CONFIG_SPFD5408 || CONFIG_R61505

void ili9325_lcdInit(TFT_t * dev, int width, int height, int offsetx, int offsety)
{
	lcdInitDevive(dev, width, height, offsetx, offsety);

#if CONFIG_ILI9325
	ESP_LOGI(TAG,"Your TFT is ILI9325");
#elif CONFIG_SPFD5408
	ESP_LOGI(TAG,"Your TFT is SPFD5408");
#elif CONFIG_R61505
	ESP_LOGI(TAG,"Your TFT is R61505");
#endif
	ESP_LOGI(TAG,"Screen width:%d",width);
	ESP_LOGI(TAG,"Screen height:%d",height);

	lcd_write_register_word(dev, 0x00e5,0x8000);
	lcd_write_register_word(dev, 0x0000,0x0001);
	lcd_write_register_word(dev, 0x0001,0x0000);
	lcd_write_register_word(dev, 0x0002,0x0700);
	lcd_write_register_word(dev, 0x0003,0x1030);
	lcd_write_register_word(dev, 0x0004,0x0000);
	lcd_write_register_word(dev, 0x0008,0x0202);
	lcd_write_register_word(dev, 0x0009,0x0000);
	lcd_write_register_word(dev, 0x000a,0x0000);
	lcd_write_register_word(dev, 0x000c,0x0000);
	lcd_write_register_word(dev, 0x000d,0x0000);
	lcd_write_register_word(dev, 0x000f,0x0000);
	lcd_write_register_word(dev, 0x0010,0x0000);
	lcd_write_register_word(dev, 0x0011,0x0000);
	lcd_write_register_word(dev, 0x0012,0x0000);
	lcd_write_register_word(dev, 0x0013,0x0000);
	lcd_write_register_word(dev, 0x0010,0x17b0);
	lcd_write_register_word(dev, 0x0011,0x0037);
	lcd_write_register_word(dev, 0x0012,0x0138);
	lcd_write_register_word(dev, 0x0013,0x1700);
	lcd_write_register_word(dev, 0x0029,0x000d);
	lcd_write_register_word(dev, 0x0020,0x0000);
	lcd_write_register_word(dev, 0x0021,0x0000);
	lcd_write_register_word(dev, 0x0030,0x0001);
	lcd_write_register_word(dev, 0x0031,0x0606);
	lcd_write_register_word(dev, 0x0032,0x0304);
	lcd_write_register_word(dev, 0x0033,0x0202);
	lcd_write_register_word(dev, 0x0034,0x0202);
	lcd_write_register_word(dev, 0x0035,0x0103);
	lcd_write_register_word(dev, 0x0036,0x011d);
	lcd_write_register_word(dev, 0x0037,0x0404);
	lcd_write_register_word(dev, 0x0038,0x0404);
	lcd_write_register_word(dev, 0x0039,0x0404);
	lcd_write_register_word(dev, 0x003c,0x0700);
	lcd_write_register_word(dev, 0x003d,0x0a1f);
	lcd_write_register_word(dev, 0x0050,0x0000);
	lcd_write_register_word(dev, 0x0051,0x00ef);
	lcd_write_register_word(dev, 0x0052,0x0000);
	lcd_write_register_word(dev, 0x0053,0x013f);
#if CONFIG_ILI9325
	lcd_write_register_word(dev, 0x0060,0x2700);
#elif CONFIG_SPFD5408
	lcd_write_register_word(dev, 0x0060,0xA700);
#elif CONFIG_R61505
	lcd_write_register_word(dev, 0x0060,0xA700);
#endif
	lcd_write_register_word(dev, 0x0061,0x0001);
	lcd_write_register_word(dev, 0x006a,0x0000);
	lcd_write_register_word(dev, 0x0090,0x0010);
	lcd_write_register_word(dev, 0x0092,0x0000);
	lcd_write_register_word(dev, 0x0093,0x0003);
	lcd_write_register_word(dev, 0x0095,0x0101);
	lcd_write_register_word(dev, 0x0097,0x0000);
	lcd_write_register_word(dev, 0x0098,0x0000);
	lcd_write_register_word(dev, 0x0007,0x0021);
	lcd_write_register_word(dev, 0x0007,0x0031);
	lcd_write_register_word(dev, 0x0007,0x0173);

	// ili9325 custom function
	DrawPixel = ili9325_lcdDrawPixel;
	DrawMultiPixels = ili9325_lcdDrawMultiPixels;
	DrawFillRect = ili9325_lcdDrawFillRect;
	DisplayOff = ili9325_lcdDisplayOff;
	DisplayOn = ili9325_lcdDisplayOn;
	InversionOff = ili9325_lcdInversionOff;
	InversionOn = ili9325_lcdInversionOn;
	EnableScroll = ili9325_lcdEnableScroll;
	SetScrollArea = ili9325_lcdSetScrollArea;
	ResetScrollArea = ili9325_lcdResetScrollArea;
	StartScroll = ili9325_lcdStartScroll;

}
#endif

#if CONFIG_ILI9325 || CONFIG_SPFD5408 || CONFIG_R61505 || CONFIG_LGDP4532 || CONFIG_ST7781 || CONFIG_ST7783

// Draw pixel
// x:X coordinate
// y:Y coordinate
// color:color
void ili9325_lcdDrawPixel(TFT_t * dev, uint16_t x, uint16_t y, uint16_t color){
	if (x >= dev->_width) return;
	if (y >= dev->_height) return;

	uint16_t _x = x + dev->_offsetx;
	uint16_t _y = y + dev->_offsety;

   lcd_write_register_word(dev, 0x0020, _x); // RAM Address Set 1
   lcd_write_register_word(dev, 0x0021, _y); // RAM Address Set 2
   lcd_write_register_word(dev, 0x0022, color); // Write Data to GRAM
}

// Draw multi pixel
// x:X coordinate
// y:Y coordinate
// size:Number of colors
// colors:colors
void ili9325_lcdDrawMultiPixels(TFT_t * dev, uint16_t x, uint16_t y, uint16_t size, uint16_t * colors) {
	if (x+size > dev->_width) return;
	if (y >= dev->_height) return;

	ESP_LOGD(TAG,"offset(x)=%d offset(y)=%d",dev->_offsetx,dev->_offsety);
	uint16_t _x1 = x + dev->_offsetx;
	//uint16_t _x2 = _x1 + size;
	uint16_t _y1 = y + dev->_offsety;
	//uint16_t _y2 = _y1;

	lcd_write_register_word(dev, 0x0020, _x1); // RAM Address Set 1
	lcd_write_register_word(dev, 0x0021, _y1); // RAM Address Set 2
	lcd_write_comm_word(dev, 0x0022); // Write Data to GRAM
	lcd_write_colors(dev, colors, size);
}




// Draw rectangle of filling
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End X coordinate
// y2:End Y coordinate
// color:color
void ili9325_lcdDrawFillRect(TFT_t * dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
	if (x1 >= dev->_width) return;
	if (x2 >= dev->_width) x2=dev->_width-1;
	if (y1 >= dev->_height) return;
	if (y2 >= dev->_height) y2=dev->_height-1;

	ESP_LOGD(TAG,"offset(x)=%d offset(y)=%d",dev->_offsetx,dev->_offsety);
	uint16_t _x1 = x1 + dev->_offsetx;
	//uint16_t _x2 = x2 + dev->_offsetx;
	uint16_t _y1 = y1 + dev->_offsety;
	uint16_t _y2 = y2 + dev->_offsety;
	uint16_t _size = x2 - x1 + 1;

	for(uint16_t _y=_y1;_y<=_y2;_y++){
		lcd_write_register_word(dev, 0x0020, _x1); // RAM Address Set 1
		lcd_write_register_word(dev, 0x0021, _y); // RAM Address Set 2
		lcd_write_comm_word(dev, 0x0022); // Write Data to GRAM
		lcd_write_color(dev, color, _size);
	}
}

// Display OFF
void ili9325_lcdDisplayOff(TFT_t * dev) {
	lcd_write_register_word(dev, 0x0007, 0x0000); // Set GON=0 DTE=0 D1=0, D0=0
}
 
// Display ON
void ili9325_lcdDisplayOn(TFT_t * dev) {
	lcd_write_register_word(dev, 0x0007, 0x0173); // Set GON=1 DTE=1 D1=1, D0=1
}

// Display Inversion OFF
void ili9325_lcdInversionOff(TFT_t * dev) {
}

// Display Inversion ON
void ili9325_lcdInversionOn(TFT_t * dev) {
}

// Enable Vertical Scrolling
bool ili9325_lcdEnableScroll(TFT_t * dev){
	return false;
}


// Vertical Scrolling Definition
// tfa:Top Fixed Area
// vsa:Vertical Scrolling Area
// bfa:Bottom Fixed Area
void ili9325_lcdSetScrollArea(TFT_t * dev, uint16_t tfa, uint16_t vsa, uint16_t bfa){
}

void ili9325_lcdResetScrollArea(TFT_t * dev, uint16_t vsa){
}

// Vertical Scrolling Start Address
// vsp:Vertical Scrolling Start Address
void ili9325_lcdStartScroll(TFT_t * dev, uint16_t vsp){
}

#endif

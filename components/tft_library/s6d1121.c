#include <string.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#if CONFIG_S6D1121

#include "lcd_com.h"
#include "lcd_lib.h"
#include "s6d1121.h"

#define TAG "S6D1121"

void s6d1121_lcdInit(TFT_t * dev, int width, int height, int offsetx, int offsety)
{
	lcdInitDevice(dev, width, height, offsetx, offsety);

	ESP_LOGI(TAG,"Your TFT is S6D1121");
	ESP_LOGI(TAG,"Screen width:%d",width);
	ESP_LOGI(TAG,"Screen height:%d",height);

#if 1
	lcd_write_register_word(dev, 0x0011, 0x2004);
	lcd_write_register_word(dev, 0x0013, 0xCC00);
	lcd_write_register_word(dev, 0x0015, 0x2600);
	lcd_write_register_word(dev, 0x0014, 0x252A);
	lcd_write_register_word(dev, 0x0012, 0x0033);
	lcd_write_register_word(dev, 0x0013, 0xCC04);
	lcd_write_register_word(dev, 0x0013, 0xCC06);
	lcd_write_register_word(dev, 0x0013, 0xCC4F);
	lcd_write_register_word(dev, 0x0013, 0x674F);
	lcd_write_register_word(dev, 0x0011, 0x2003);
	lcd_write_register_word(dev, 0x0030, 0x2609);
	lcd_write_register_word(dev, 0x0031, 0x242C);
	lcd_write_register_word(dev, 0x0032, 0x1F23);
	lcd_write_register_word(dev, 0x0033, 0x2425);
	lcd_write_register_word(dev, 0x0034, 0x2226);
	lcd_write_register_word(dev, 0x0035, 0x2523);
	lcd_write_register_word(dev, 0x0036, 0x1C1A);
	lcd_write_register_word(dev, 0x0037, 0x131D);
	lcd_write_register_word(dev, 0x0038, 0x0B11);
	lcd_write_register_word(dev, 0x0039, 0x1210);
	lcd_write_register_word(dev, 0x003A, 0x1315);
	lcd_write_register_word(dev, 0x003B, 0x3619);
	lcd_write_register_word(dev, 0x003C, 0x0D00);
	lcd_write_register_word(dev, 0x003D, 0x000D);
	lcd_write_register_word(dev, 0x0016, 0x0007);
	lcd_write_register_word(dev, 0x0002, 0x0013);
	lcd_write_register_word(dev, 0x0001, 0x0127);
	//lcd_write_register_word(dev, 0x0001, 0x0027);
	lcd_write_register_word(dev, 0x0003, 0x0003);
	//lcd_write_register_word(dev, 0x0003, 0x0000);
	lcd_write_register_word(dev, 0x0008, 0x0303);
	lcd_write_register_word(dev, 0x000A, 0x000B);
	lcd_write_register_word(dev, 0x000B, 0x0003);
	lcd_write_register_word(dev, 0x000C, 0x0000);
	lcd_write_register_word(dev, 0x0041, 0x0000);
	lcd_write_register_word(dev, 0x0050, 0x0000);
	lcd_write_register_word(dev, 0x0060, 0x0005);
	lcd_write_register_word(dev, 0x0070, 0x000B);
	lcd_write_register_word(dev, 0x0071, 0x0000);
	lcd_write_register_word(dev, 0x0078, 0x0000);
	lcd_write_register_word(dev, 0x007A, 0x0000);
	lcd_write_register_word(dev, 0x0079, 0x0007);
	lcd_write_register_word(dev, 0x0007, 0x0051);
	lcd_write_register_word(dev, 0x0007, 0x0053);
	lcd_write_register_word(dev, 0x0079, 0x0000);
	//lcd_write_comm_word(dev, 0x0022);
#endif

#if 0
	static const uint16_t S6D1121_regValues[] = {
		0x0011, 0x2004,
		0x0013, 0xCC00,
		0x0015, 0x2600,
		0x0014, 0x252A,
		0x0012, 0x0033,
		0x0013, 0xCC04,
		0x0013, 0xCC06,
		0x0013, 0xCC4F,
		0x0013, 0x674F,
		0x0011, 0x2003,
		0x0030, 0x2609,
		0x0031, 0x242C,
		0x0032, 0x1F23,
		0x0033, 0x2425,
		0x0034, 0x2226,
		0x0035, 0x2523,
		0x0036, 0x1C1A,
		0x0037, 0x131D,
		0x0038, 0x0B11,
		0x0039, 0x1210,
		0x003A, 0x1315,
		0x003B, 0x3619,
		0x003C, 0x0D00,
		0x003D, 0x000D,
		0x0016, 0x0007,
		0x0002, 0x0013,
		0x0003, 0x0003,
		//0x0003, 0x000B,
		0x0001, 0x0127,
		0x0008, 0x0303,
		0x000A, 0x000B,
		0x000B, 0x0003,
		0x000C, 0x0000,
		0x0041, 0x0000,
		0x0050, 0x0000,
		0x0060, 0x0005,
		0x0070, 0x000B,
		0x0071, 0x0000,
		0x0078, 0x0000,
		0x007A, 0x0000,
		0x0079, 0x0007,
		0x0007, 0x0051,
		0x0007, 0x0053,
	};
	lcd_write_table16(dev, S6D1121_regValues, sizeof(S6D1121_regValues));
#endif

	// s6d1121 custom function
	DrawPixel = s6d1121_lcdDrawPixel;
	DrawMultiPixels = s6d1121_lcdDrawMultiPixels;
	DrawFillRect = s6d1121_lcdDrawFillRect;
	DisplayOff = s6d1121_lcdDisplayOff;
	DisplayOn = s6d1121_lcdDisplayOn;
	InversionOff = s6d1121_lcdInversionOff;
	InversionOn = s6d1121_lcdInversionOn;
	EnableScroll = s6d1121_lcdEnableScroll;
	SetScrollArea = s6d1121_lcdSetScrollArea;
	ResetScrollArea = s6d1121_lcdResetScrollArea;
	StartScroll = s6d1121_lcdStartScroll;

}

// Draw pixel
// x:X coordinate
// y:Y coordinate
// color:color
void s6d1121_lcdDrawPixel(TFT_t * dev, uint16_t x, uint16_t y, uint16_t color){
	if (x >= dev->_width) return;
	if (y >= dev->_height) return;

	uint16_t _x = x + dev->_offsetx;
	uint16_t _y = y + dev->_offsety;

	lcd_write_register_word(dev, 0x0046, (_x << 8) | _x);
	lcd_write_register_word(dev, 0x0047, _y);
	lcd_write_register_word(dev, 0x0048, _y);
	lcd_write_register_word(dev, 0x0020, _x); // RAM Address Set 1
	lcd_write_register_word(dev, 0x0021, _y); // RAM Address Set 2
	lcd_write_register_word(dev, 0x0022, color); // Write Data to GRAM
}

// Draw multi pixel
// x:X coordinate
// y:Y coordinate
// size:Number of colors
// colors:colors
void s6d1121_lcdDrawMultiPixels(TFT_t * dev, uint16_t x, uint16_t y, uint16_t size, uint16_t * colors) {
	if (x+size > dev->_width) return;
	if (y >= dev->_height) return;

	ESP_LOGD(TAG,"offset(x)=%d offset(y)=%d",dev->_offsetx,dev->_offsety);
	uint16_t _x1 = x + dev->_offsetx;
	uint16_t _x2 = _x1 + size;
	uint16_t _y1 = y + dev->_offsety;
	//uint16_t _y2 = _y1;

	lcd_write_register_word(dev, 0x0046, (_x2 << 8) | _x1);
	lcd_write_register_word(dev, 0x0047, _y1);
	lcd_write_register_word(dev, 0x0048, _y1);
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
void s6d1121_lcdDrawFillRect(TFT_t * dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
	if (x1 >= dev->_width) return;
	if (x2 >= dev->_width) x2=dev->_width-1;
	if (y1 >= dev->_height) return;
	if (y2 >= dev->_height) y2=dev->_height-1;

	ESP_LOGD(TAG,"offset(x)=%d offset(y)=%d",dev->_offsetx,dev->_offsety);
	uint16_t _x1 = x1 + dev->_offsetx;
	uint16_t _x2 = x2 + dev->_offsetx;
	uint16_t _y1 = y1 + dev->_offsety;
	uint16_t _y2 = y2 + dev->_offsety;
	uint16_t _size = x2 - x1 + 1;

	for(uint16_t _y=_y1;_y<=_y2;_y++){
		lcd_write_register_word(dev, 0x0046, (_x2 << 8) | _x1);
		lcd_write_register_word(dev, 0x0047, _y);
		lcd_write_register_word(dev, 0x0048, _y);
		lcd_write_register_word(dev, 0x0020, _x1); // RAM Address Set 1
		lcd_write_register_word(dev, 0x0021, _y); // RAM Address Set 2
		lcd_write_comm_word(dev, 0x0022); // Write Data to GRAM
		lcd_write_color(dev, color, _size);
	}
}

// Display OFF
void s6d1121_lcdDisplayOff(TFT_t * dev) {
	lcd_write_register_word(dev, 0x0007, 0x0000); // Set GON=0 DTE=0 D1=0, D0=0
}
 
// Display ON
void s6d1121_lcdDisplayOn(TFT_t * dev) {
	lcd_write_register_word(dev, 0x0007, 0x0173); // Set GON=1 DTE=1 D1=1, D0=1
}

// Display Inversion OFF
void s6d1121_lcdInversionOff(TFT_t * dev) {
}

// Display Inversion ON
void s6d1121_lcdInversionOn(TFT_t * dev) {
}

// Enable Vertical Scrolling
bool s6d1121_lcdEnableScroll(TFT_t * dev){
	return false;
}


// Vertical Scrolling Definition
// tfa:Top Fixed Area
// vsa:Vertical Scrolling Area
// bfa:Bottom Fixed Area
void s6d1121_lcdSetScrollArea(TFT_t * dev, uint16_t tfa, uint16_t vsa, uint16_t bfa){
}

void s6d1121_lcdResetScrollArea(TFT_t * dev, uint16_t vsa){
}

// Vertical Scrolling Start Address
// vsp:Vertical Scrolling Start Address
void s6d1121_lcdStartScroll(TFT_t * dev, uint16_t vsp){
}

#endif

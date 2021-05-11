#include <string.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "lcd_com.h"
#include "lcd_lib.h"
#include "r61509.h"

#define TAG "R61509"

#if CONFIG_R61509 || CONFIG_ST7793

void r61509_lcdInit(TFT_t * dev, int width, int height, int offsetx, int offsety)
{
	lcdInitDevice(dev, width, height, offsetx, offsety);

#if CONFIG_R61509
	ESP_LOGI(TAG,"Your TFT is R61509");
#elif CONFIG_ST7793
	ESP_LOGI(TAG,"Your TFT is ST7793");
#endif
	ESP_LOGI(TAG,"Screen width:%d",width);
	ESP_LOGI(TAG,"Screen height:%d",height);

	static const uint16_t R61509_regValues[] = {
		//0x0400, 0x6200,
		0x0008, 0x0808,
		0x0010, 0x0016,		 //69.5Hz		  0016
		0x0011, 0x0101,
		0x0012, 0x0000,
		0x0013, 0x0001,
		0x0100, 0x0330,		 //BT,AP
		0x0101, 0x0237,		 //DC0,DC1,VC
		0x0103, 0x0D00,		 //VDV
		0x0280, 0x6100,		 //VCM
		0x0102, 0xC1B0,		 //VRH,VCMR,PSON,PON
		TFTLCD_DELAY16,50,
		//	 0x0001, 0x0100,
		0x0001, 0x0000,
		0x0002, 0x0100,
		0x0003, 0x1030,		  //1030
		0x0009, 0x0001,
		0x000C, 0x0000,
		0x0090, 0x8000,
		0x000F, 0x0000,
		0x0210, 0x0000,
		0x0211, 0x00EF,
		0x0212, 0x0000,
		0x0213, 0x018F,		  //432=01AF,400=018F
		0x0500, 0x0000,
		0x0501, 0x0000,
		0x0502, 0x005F,
		//0x0400, 0x6200,
		0x0400, 0xE200,
		0x0401, 0x0001,
		0x0404, 0x0000,
		TFTLCD_DELAY16,50,
		0x0007, 0x0100,			//BASEE
		TFTLCD_DELAY16,50,
		0x0200, 0x0000,
		0x0201, 0x0000,
	};
	lcd_write_table16(dev, R61509_regValues, sizeof(R61509_regValues));


	// r61509 custom function
	DrawPixel = r61509_lcdDrawPixel;
	DrawMultiPixels = r61509_lcdDrawMultiPixels;
	DrawFillRect = r61509_lcdDrawFillRect;
	DisplayOff = r61509_lcdDisplayOff;
	DisplayOn = r61509_lcdDisplayOn;
	InversionOff = r61509_lcdInversionOff;
	InversionOn = r61509_lcdInversionOn;
	EnableScroll = r61509_lcdEnableScroll;
	SetScrollArea = r61509_lcdSetScrollArea;
	ResetScrollArea = r61509_lcdResetScrollArea;
	StartScroll = r61509_lcdStartScroll;

}


// Draw pixel
// x:X coordinate
// y:Y coordinate
// color:color
void r61509_lcdDrawPixel(TFT_t * dev, uint16_t x, uint16_t y, uint16_t color){
	if (x >= dev->_width) return;
	if (y >= dev->_height) return;

	uint16_t _x = x + dev->_offsetx;
	uint16_t _y = y + dev->_offsety;

   lcd_write_register_word(dev, 0x0200, _x); // RAM Address Set 1
   lcd_write_register_word(dev, 0x0201, _y); // RAM Address Set 2
   lcd_write_register_word(dev, 0x0202, color); // Write Data to GRAM
}

// Draw multi pixel
// x:X coordinate
// y:Y coordinate
// size:Number of colors
// colors:colors
void r61509_lcdDrawMultiPixels(TFT_t * dev, uint16_t x, uint16_t y, uint16_t size, uint16_t * colors) {
	if (x+size > dev->_width) return;
	if (y >= dev->_height) return;

	ESP_LOGD(TAG,"offset(x)=%d offset(y)=%d",dev->_offsetx,dev->_offsety);
	uint16_t _x1 = x + dev->_offsetx;
	//uint16_t _x2 = _x1 + size;
	uint16_t _y1 = y + dev->_offsety;
	//uint16_t _y2 = _y1;

	lcd_write_register_word(dev, 0x0200, _x1); // RAM Address Set 1
	lcd_write_register_word(dev, 0x0201, _y1); // RAM Address Set 2
	lcd_write_comm_word(dev, 0x0202); // Write Data to GRAM
	lcd_write_colors(dev, colors, size);
}




// Draw rectangle of filling
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End X coordinate
// y2:End Y coordinate
// color:color
void r61509_lcdDrawFillRect(TFT_t * dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
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
		lcd_write_register_word(dev, 0x0200, _x1); // RAM Address Set 1
		lcd_write_register_word(dev, 0x0201, _y); // RAM Address Set 2
		lcd_write_comm_word(dev, 0x0202); // Write Data to GRAM
		lcd_write_color(dev, color, _size);
	}
}

// Display OFF
void r61509_lcdDisplayOff(TFT_t * dev) {
	lcd_write_register_word(dev, 0x0007, 0x0000);	// Display Control 1
	//lcd_write_register_word(dev, 0x0007, 0x0000); // Set GON=0 DTE=0 D1=0, D0=0
}
 
// Display ON
void r61509_lcdDisplayOn(TFT_t * dev) {
	lcd_write_register_word(dev, 0x0007, 0x0100);	// Display Control 1
	//lcd_write_register_word(dev, 0x0007, 0x0173); // Set GON=1 DTE=1 D1=1, D0=1
}

// Display Inversion OFF
void r61509_lcdInversionOff(TFT_t * dev) {
}

// Display Inversion ON
void r61509_lcdInversionOn(TFT_t * dev) {
}

// Enable Vertical Scrolling
bool r61509_lcdEnableScroll(TFT_t * dev){
	return false;
}


// Vertical Scrolling Definition
// tfa:Top Fixed Area
// vsa:Vertical Scrolling Area
// bfa:Bottom Fixed Area
void r61509_lcdSetScrollArea(TFT_t * dev, uint16_t tfa, uint16_t vsa, uint16_t bfa){
}

void r61509_lcdResetScrollArea(TFT_t * dev, uint16_t vsa){
}

// Vertical Scrolling Start Address
// vsp:Vertical Scrolling Start Address
void r61509_lcdStartScroll(TFT_t * dev, uint16_t vsp){
}

#endif

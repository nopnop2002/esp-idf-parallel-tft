#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "lcd_com.h"
#include "lcd_lib.h"
#include "hx8347.h"

#define TAG "HX8347"

#if CONFIG_HX8347A || CONFIG_HX8347D || CONFIG_HX8347G || CONFIG_HX8347I

void hx8347_lcdInit(TFT_t * dev, int width, int height, int offsetx, int offsety)
{
	lcdInitDevice(dev, width, height, offsetx, offsety);

	ESP_LOGI(TAG,"Your TFT is HX8347");
	ESP_LOGI(TAG,"Screen width:%d",width);
	ESP_LOGI(TAG,"Screen height:%d",height);

	// Initailize TFT
	static const uint8_t regValues[] = {
		0xEA, 2, 0x00, 0x20,		//PTBA[15:0]
		0xEC, 2, 0x0C, 0xC4,		//STBA[15:0]
		0xE8, 1, 0x38,		//OPON[7:0]
		0xE9, 1, 0x10,		//OPON1[7:0]
		0xF1, 1, 0x01,		//OTPS1B
		0xF2, 1, 0x10,		//GEN
		//Gamma 2.2 Setting
		0x40, 13, 0x01, 0x00, 0x00, 0x10, 0x0E, 0x24, 0x04, 0x50, 0x02, 0x13, 0x19, 0x19, 0x16,
		0x50, 14, 0x1B, 0x31, 0x2F, 0x3F, 0x3F, 0x3E, 0x2F, 0x7B, 0x09, 0x06, 0x06, 0x0C, 0x1D, 0xCC,
		//Power Voltage Setting
		0x1B, 1, 0x1B,		//VRH=4.65V
		0x1A, 1, 0x01,		//BT (VGH~15V,VGL~-10V,DDVDH~5V)
		0x24, 1, 0x2F,		//VMH(VCOM High voltage ~3.2V)
		0x25, 1, 0x57,		//VML(VCOM Low voltage -1.2V)
		//****VCOM offset**///
		0x23, 1, 0x88,		//for Flicker adjust //can reload from OTP
		//Power on Setting
		0x18, 1, 0x34,		//I/P_RADJ,N/P_RADJ, Normal mode 60Hz
		0x19, 1, 0x01,		//OSC_EN='1', start Osc
		0x01, 1, 0x00,		//DP_STB='0', out deep sleep
		0x1F, 1, 0x88,		// GAS=1, VOMG=00, PON=0, DK=1, XDK=0, DVDH_TRI=0, STB=0
		TFTLCD_DELAY8, 5,
		0x1F, 1, 0x80,		// GAS=1, VOMG=00, PON=0, DK=0, XDK=0, DVDH_TRI=0, STB=0
		TFTLCD_DELAY8, 3,
		0x1F, 1, 0x90,		// GAS=1, VOMG=00, PON=1, DK=0, XDK=0, DVDH_TRI=0, STB=0
		TFTLCD_DELAY8, 5,
		0x1F, 1, 0xD0,		// GAS=1, VOMG=10, PON=1, DK=0, XDK=0, DDVDH_TRI=0, STB=0
		TFTLCD_DELAY8, 5,
		//262k/65k color selection
		0x17, 1, 0x05,		//default 0x06 262k color // 0x05 65k color
		//SET PANEL
		0x36, 1, 0x00,		//SS_P, GS_P,REV_P,BGR_P
		//Display ON Setting
		0x28, 1, 0x38,		//GON=1, DTE=1, D=1000
		TFTLCD_DELAY8, 40,
		//0x28, 1, 0x3F,	  //GON=1, DTE=1, D=1100
		0x28, 1, 0x3C,		//GON=1, DTE=1, D=1100

		0x16, 1, 0x98,		//MY=1, MX=0, MV=0, ML=1, BGR=1
		//0x16, 1, 0x48,		//MY=0, MX=1, MV=0, ML=0, BGR=1
	};
	lcd_write_table(dev, regValues, sizeof(regValues));
	// hx8347 custom function
	DrawPixel = hx8347_lcdDrawPixel;
	DrawMultiPixels = hx8347_lcdDrawMultiPixels;
	DrawFillRect = hx8347_lcdDrawFillRect;
	DisplayOff = hx8347_lcdDisplayOff;
	DisplayOn = hx8347_lcdDisplayOn;
	InversionOff = hx8347_lcdInversionOff;
	InversionOn = hx8347_lcdInversionOn;
	EnableScroll = hx8347_lcdEnableScroll;
	SetScrollArea = hx8347_lcdSetScrollArea;
	ResetScrollArea = hx8347_lcdResetScrollArea;
	StartScroll = hx8347_lcdStartScroll;

}

void hx8347_lcd_write_register(TFT_t * dev, uint8_t addr, uint16_t data)
{
	char d[2];
	d[0] = (data >> 8) & 0xFF;
	d[1] = data & 0xFF;
	lcd_write_comm_byte(dev, addr);
	lcd_write_data_byte(dev, d[0]);
	lcd_write_comm_byte(dev, addr+1);
	lcd_write_data_byte(dev, d[1]);
}

// Draw pixel
// x:X coordinate
// y:Y coordinate
// color:color
void hx8347_lcdDrawPixel(TFT_t * dev, uint16_t x, uint16_t y, uint16_t color){
	if (x >= dev->_width) return;
	if (y >= dev->_height) return;

	uint16_t _x = x + dev->_offsetx;
	uint16_t _y = y + dev->_offsety;

	hx8347_lcd_write_register(dev, 0x02, _x); // Column address start register
	hx8347_lcd_write_register(dev, 0x04, _x); // Column address end register
	hx8347_lcd_write_register(dev, 0x06, _y); // Row address start register
	hx8347_lcd_write_register(dev, 0x08, _y); // Row address end register
	lcd_write_comm_byte(dev, 0x22);	//	Memory Write
	lcd_write_data_word(dev, color);
}

// Draw multi pixel
// x:X coordinate
// y:Y coordinate
// size:Number of colors
// colors:colors
void hx8347_lcdDrawMultiPixels(TFT_t * dev, uint16_t x, uint16_t y, uint16_t size, uint16_t * colors) {
	if (x+size > dev->_width) return;
	if (y >= dev->_height) return;

	ESP_LOGD(TAG,"offset(x)=%d offset(y)=%d",dev->_offsetx,dev->_offsety);
	uint16_t _x1 = x + dev->_offsetx;
	uint16_t _x2 = _x1 + size;
	uint16_t _y1 = y + dev->_offsety;
	uint16_t _y2 = _y1;
	ESP_LOGD(TAG,"_x1=%d _x2=%d _y1=%d _y2=%d",_x1, _x2, _y1, _y2);

	hx8347_lcd_write_register(dev, 0x02, _x1); // Column address start register
	hx8347_lcd_write_register(dev, 0x04, _x2); // Column address end register
	hx8347_lcd_write_register(dev, 0x06, _y1); // Row address start register
	hx8347_lcd_write_register(dev, 0x08, _y2); // Row address end register
	lcd_write_comm_byte(dev, 0x22);	//	Memory Write
	lcd_write_colors(dev, colors, size);
}




// Draw rectangle of filling
// x1:Start X coordinate
// y1:Start Y coordinate
// x2:End X coordinate
// y2:End Y coordinate
// color:color
void hx8347_lcdDrawFillRect(TFT_t * dev, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
	if (x1 >= dev->_width) return;
	if (x2 >= dev->_width) x2=dev->_width-1;
	if (y1 >= dev->_height) return;
	if (y2 >= dev->_height) y2=dev->_height-1;

	ESP_LOGD(TAG,"offset(x)=%d offset(y)=%d",dev->_offsetx,dev->_offsety);
	uint16_t _x1 = x1 + dev->_offsetx;
	uint16_t _x2 = x2 + dev->_offsetx;
	uint16_t _y1 = y1 + dev->_offsety;
	uint16_t _y2 = y2 + dev->_offsety;

#if 0
	hx8347_lcd_write_register(dev, 0x02, _x1); // Column address start register
	hx8347_lcd_write_register(dev, 0x04, _x2); // Column address end register
	hx8347_lcd_write_register(dev, 0x06, _y1); // Row address start register
	hx8347_lcd_write_register(dev, 0x08, _y2); // Row address end register
	lcd_write_comm_byte(dev, 0x22);	//	Memory Write
#endif
	for(int _x=_x1;_x<=_x2;_x++) {
		hx8347_lcd_write_register(dev, 0x02, _x); // Column address start register
		hx8347_lcd_write_register(dev, 0x04, _x); // Column address end register
		hx8347_lcd_write_register(dev, 0x06, _y1); // Row address start register
		hx8347_lcd_write_register(dev, 0x08, _y2); // Row address end register
		lcd_write_comm_byte(dev, 0x22);	//	Memory Write
		uint16_t size = _y2-_y1+1;
		lcd_write_color(dev, color, size);
	}
}

// Display OFF
void hx8347_lcdDisplayOff(TFT_t * dev) {
	lcd_write_comm_byte(dev, 0x28);
	lcd_write_data_byte(dev, 0x30);
}
 
// Display ON
void hx8347_lcdDisplayOn(TFT_t * dev) {
	lcd_write_comm_byte(dev, 0x28);
	lcd_write_data_byte(dev, 0x3C);
}

// Display Inversion OFF
void hx8347_lcdInversionOff(TFT_t * dev) {
	lcd_write_comm_byte(dev, 0x01);
#if CONFIG_HX8347A
	lcd_write_data_byte(dev, 0x02);
#else
	lcd_write_data_byte(dev, 0x0A);
#endif
}

// Display Inversion ON
void hx8347_lcdInversionOn(TFT_t * dev) {
	lcd_write_comm_byte(dev, 0x01);
#if CONFIG_HX8347A
	lcd_write_data_byte(dev, 0x06);
#else
	lcd_write_data_byte(dev, 0x08);
#endif
}

// Enable Vertical Scrolling
bool hx8347_lcdEnableScroll(TFT_t * dev){
	return false;
}


// Vertical Scrolling Definition
// tfa:Top Fixed Area
// vsa:Vertical Scrolling Area
// bfa:Bottom Fixed Area
void hx8347_lcdSetScrollArea(TFT_t * dev, uint16_t tfa, uint16_t vsa, uint16_t bfa){
}

void hx8347_lcdResetScrollArea(TFT_t * dev, uint16_t vsa){
}

// Vertical Scrolling Start Address
// vsp:Vertical Scrolling Start Address
void hx8347_lcdStartScroll(TFT_t * dev, uint16_t vsp){
}
#endif

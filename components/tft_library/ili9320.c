#include <string.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "lcd_com.h"
#include "lcd_lib.h"
#include "ili9320.h"

#define TAG "ILI9320"

#if CONFIG_ILI9320 || CONFIG_SPFD5408 || CONFIG_R61505

void ili9320_lcdInit(TFT_t * dev, int width, int height, int offsetx, int offsety)
{
	lcdInitDevice(dev, width, height, offsetx, offsety);

#if CONFIG_ILI9320
	ESP_LOGI(TAG,"Your TFT is ILI9320");
#elif CONFIG_SPFD5408
	ESP_LOGI(TAG,"Your TFT is SPFD5408");
#endif
	ESP_LOGI(TAG,"Screen width:%d",width);
	ESP_LOGI(TAG,"Screen height:%d",height);

	static const uint16_t ILI9320_regValues[] = {
		0x00e5, 0x8000,
		0x0000, 0x0001,
		0x0001, 0x100,
		0x0002, 0x0700,
		0x0003, 0x1030,
		0x0004, 0x0000,
		0x0008, 0x0202,
		0x0009, 0x0000,
		0x000A, 0x0000,
		0x000C, 0x0000,
		0x000D, 0x0000,
		0x000F, 0x0000,
		//-----Power On sequence-----------------------
		0x0010, 0x0000,
		0x0011, 0x0007,
		0x0012, 0x0000,
		0x0013, 0x0000,
		TFTLCD_DELAY, 50,
		0x0010, 0x17B0,  //SAP=1, BT=7, APE=1, AP=3
		0x0011, 0x0007,  //DC1=0, DC0=0, VC=7
		TFTLCD_DELAY, 10,
		0x0012, 0x013A,  //VCMR=1, PON=3, VRH=10
		TFTLCD_DELAY, 10,
		0x0013, 0x1A00,  //VDV=26
		0x0029, 0x000c,  //VCM=12
		TFTLCD_DELAY, 10,
		//-----Gamma control-----------------------
		0x0030, 0x0000,
		0x0031, 0x0505,
		0x0032, 0x0004,
		0x0035, 0x0006,
		0x0036, 0x0707,
		0x0037, 0x0105,
		0x0038, 0x0002,
		0x0039, 0x0707,
		0x003C, 0x0704,
		0x003D, 0x0807,
		//-----Set RAM area-----------------------
		//0x0060, 0xA700,		//GS=1
		0x0060, 0x2700,		//GS=1
		0x0061, 0x0001,
		0x006A, 0x0000,
		0x0021, 0x0000,
		0x0020, 0x0000,
		//-----Partial Display Control------------
		0x0080, 0x0000,
		0x0081, 0x0000,
		0x0082, 0x0000,
		0x0083, 0x0000,
		0x0084, 0x0000,
		0x0085, 0x0000,
		//-----Panel Control----------------------
		0x0090, 0x0010,
		0x0092, 0x0000,
		0x0093, 0x0003,
		0x0095, 0x0110,
		0x0097, 0x0000,
		0x0098, 0x0000,
		//-----Display on-----------------------
		0x0007, 0x0173,
		TFTLCD_DELAY, 50,
	};
	lcd_write_table16(dev, ILI9320_regValues, sizeof(ILI9320_regValues));

	// ili9320 custom function
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

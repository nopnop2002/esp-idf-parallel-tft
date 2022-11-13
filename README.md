# esp-idf-parallel-tft
8 bit parallel TFT & 4-line resistance touch screen Driver for esp-idf.   
You can use such a TFT-Shield with esp32.   

![TFT-Shield](https://user-images.githubusercontent.com/6020549/104253960-10a71380-54b9-11eb-8789-a12c2c769ab4.JPG)

# Support driver

## Generic Shield   
- ILI9225   
- ILI9226(Same as ILI9225)   
- ILI9320   
- ILI9325   
- ILI9341   
- ILI9342   
- ILI9481   
- ILI9486   
- ILI9488   
- SPFD5408(Same as ILI9320)   
- R61505U(Same as ILI9320)   
- R61509V   
- LGDP4532   
- ST7775(Same as ILI9225)   
- ST7781   
- ST7783(Same as ST7781)   
- ST7793(Same as R61509)   
- ST7796(Same as ILI9486)   
- HX8347A(*2)   
- HX8347D(Almost the same as HX8347A)(*2)   
- HX8347G(Same as HX8347D)(*2)   
- HX8347I(Same as HX8347D)(*2)   

## OPEN-SMART Products   
- OPEN-SMART ILI9225 TFT-Shield(176x220)(*5)   
- OPEN-SMART ILI9327 TFT-Shield(240x400)(*3)(*5)   
- OPEN-SMART ILI9340 TFT-Shield(240x320)(*5)   
- OPEN-SMART S6D1121 16Pin-Parallel(240x320)(*1)(*5)   
- OPEN-SMART ST7775  16Pin-Parallel(176x220 Same as ILI9225)(*1)(*5)   
- OPEN-SMART ST7783  16Pin-Parallel(240x320)(*1)(*5)   
- OPEN-SMART R61509V 16Pin-Parallel(240x400)(*1)(*5)   
- OPEN-SMART ILI9488 16Pin-Parallel(320x400 Color inversion)(*1)(*4)(*5)   

(*1)   
I2S parallel does not work.   
I don't know why.   
GPIO parallel or REGISTER I/O parallel works.   
__LED pins connect to GND instead of 3.3V.__   

(*2)   
Very Slow.   
Most drivers require three commands to display one Pixel.   
This driver require 9 commands to display one Pixel.   
For some reason, the color of 0xFFFF does not appear.   

(*3)   
It has a GRAM offset.   
See below.   

(*4)   
Need RGB inverstion.   

(*5)   
4-line resistance touch screen available.   
See below.   


These are all 2.4 inch, 320x240 TFTs.
![TFT-SHIELD-2](https://user-images.githubusercontent.com/6020549/104244320-873a1600-54a5-11eb-93c0-9fad671fdfed.JPG)

3.95 inches is almost twice as large as 2.4 inches.
![TFT-SHIELD-3](https://user-images.githubusercontent.com/6020549/104244328-8903d980-54a5-11eb-8c9a-d26408e04f92.JPG)

These are OPEN-SMART 16Pin-Parallel Products.
![OPEN-SMART-16Pin](https://user-images.githubusercontent.com/6020549/110071604-ad9e7280-7dbf-11eb-8e09-a9ebfacfd795.JPG)

# Software requirements
esp-idf v4.4 or later.   
The i2s driver for esp32s2/s3 is supported.   

__Note for ESP-IDF V5.0__   
ESP-IDF V5.0 gives this warning, but work.   
```
#warning "legacy adc driver is deprecated, please migrate to use esp_adc/adc_oneshot.h and esp_adc/adc_continuous.h for oneshot mode and continuous mode drivers respectively"
```
Presumably, ESP-IDF V5.1 will completely obsolete the legacy ADC driver.   


# Installation

```
git clone https://github.com/nopnop2002/esp-idf-parallel-tft
cd esp-idf-parallel-tft/
idf.py set-target {esp32/esp32s2/esp32s3}
idf.py menuconfig
idf.py flash
```

__Note for ESP32-S2__   
tjpgd library does not exist in ESP32-S2 ROM. Therefore, the JPEG file cannot be displayed.   
PNG file cannot be displayed because the SRAM is small.   


# Configuration   
You have to set this config value with menuconfig.   
- CONFIG_DRIVER   
 ![](https://img.shields.io/badge/_IMPORTANT-important)  
 The information provided by sellers on Ebay or AliExpress is largely incorrect.   
 You waste time if you don't choose the right driver.   
 There are many [variations](http://domoticx.com/arduino-shield-2-4-tft-lcd-touch/) of the 2.4 inch shield.   
 You can use [this](https://github.com/prenticedavid/MCUFRIEND_kbv/tree/master/examples/LCD_ID_readreg) to find out your driver.   
 This is for Arduino UNO.   
 Do not use this on the ESP32 as the GPIO on the ESP32 is not 5V tolerant.   
 __Never believe the seller's information.__   
 I'll say it again.   
 __Never believe the seller's information.__   
- CONFIG_INTERFACE   
 Most drivers work using I2S parallel.   
 However, some drivers only work using GPIO parallels or REGISTER I/O parallels.   
 I2S parallel is the fastest when drawing to consecutive addresses.   
 However, REGISTER I/O parallel is the fastest when drawing to non-contiguous addresses.   
 When using REGISTER I/O parallel, GPIO from D0 to D7 is 1 to 31.(GPIO0 is boot mode pin)   
 GPIO parallel is most slow.   
- CONFIG_WIDTH   
- CONFIG_HEIGHT   
 Specifies the resolution of the TFT.   
- CONFIG_OFFSETX   
- CONFIG_OFFSETY   
 You can specify the GRAM offset.   
 You must specify Y offset = 32 for OPEN-SMART-ILI9327.   
- CONFIG_INVERSION   
 For some TFTs, the BGR may be inverted.   
 Specify if the colors are inverted.

![config-menu](https://user-images.githubusercontent.com/6020549/104242485-94093a80-54a2-11eb-934b-90eda9fb7bbe.jpg)

![config-app1](https://user-images.githubusercontent.com/6020549/106704254-27abd200-662f-11eb-8697-743e220c030d.jpg)

![config-app2](https://user-images.githubusercontent.com/6020549/106686442-9d06ab00-660d-11eb-9a53-912e221278ce.jpg)

![config-app4](https://user-images.githubusercontent.com/6020549/106704258-28dcff00-662f-11eb-8a75-2070c3fe746a.jpg)

# Wireing  

|TFT||ESP32|ESP32S2/S3||
|:-:|:-:|:-:|:-:|:-:|
|LDC_RST|--|GPIO32|GPIO45|*1|
|LDC_CS|--|GPIO33|GPIO42|*1|
|LDC_RS|--|GPIO15|GPIO41|*1|
|LDC_WR|--|GPIO4|GPIO40|*1|
|LDC_RD|--|GPIO2|GPIO39|*1|
|LDC_D0|--|GPIO12|GPIO1|*1 *2|
|LDC_D1|--|GPIO13|GPIO2|*1 *2|
|LDC_D2|--|GPIO26|GPIO3|*1 *2|
|LDC_D3|--|GPIO25|GPIO4|*1 *2|
|LDC_D4|--|GPIO17|GPIO5|*1 *2|
|LDC_D5|--|GPIO16|GPIO6|*1 *2|
|LDC_D6|--|GPIO27|GPIO7|*1 *2|
|LDC_D7|--|GPIO14|GPIO8|*1 *2|
|5V|--|5V|5V|*3|
|3.3V|--|3.3V|3.3V|*3|
|GND|--|GND|GND||

(*1)   
You can change any GPIO using menuconfig.   

![config-app3](https://user-images.githubusercontent.com/6020549/106685441-be669780-660b-11eb-8e8e-790348e65921.jpg)

(*2)   
When using REGISTER I/O parallel, GPIO from D0 to D7 is 1 to 31.(GPIO0 is boot mode pin)   

(*3)   
With a regulator (mostly AMS1117) is on the back, the supply voltage is 5V.   
With a regulator is NOT on the back, the supply voltage is 3.3V.   

![Back](https://user-images.githubusercontent.com/6020549/104248029-1f3afe00-54ac-11eb-913d-0a832fb569b2.JPG)

__Note__   
My R61509V has a regulator on the back.   
Normally, a TFT with a regulator works at 5V, but my R61509V doesn't work unless I supply both 5V and 3.3V.   

__Note__   
ESP32 development board cannot supply too much current.   
It is more stable when supplied from an external power source.   

![All](https://user-images.githubusercontent.com/6020549/104249117-1e0ad080-54ae-11eb-8c25-46a2d8fa5fed.JPG)

![Circuit](https://user-images.githubusercontent.com/6020549/107828267-dbe5ef00-6dcb-11eb-87e1-31f93fb571d3.jpg)

# Graphic support
![Graphic1](https://user-images.githubusercontent.com/6020549/104248260-848eef00-54ac-11eb-9ab8-4b74a2a7713f.JPG)
![Graphic2](https://user-images.githubusercontent.com/6020549/104248263-85c01c00-54ac-11eb-9d19-6886827cd29d.JPG)
![Graphic3](https://user-images.githubusercontent.com/6020549/104248265-86f14900-54ac-11eb-937b-b2bd5ed4ce69.JPG)
![Graphic4](https://user-images.githubusercontent.com/6020549/104248266-8789df80-54ac-11eb-89e6-bad34a3c035d.JPG)
![Graphic5](https://user-images.githubusercontent.com/6020549/104248267-88227600-54ac-11eb-903f-3c64f4e7cb60.JPG)
![Graphic6](https://user-images.githubusercontent.com/6020549/104248268-88bb0c80-54ac-11eb-827b-d886e0491664.JPG)
![Graphic7](https://user-images.githubusercontent.com/6020549/104248273-8953a300-54ac-11eb-8ed1-ffc38e8d6a7c.JPG)
![Graphic8](https://user-images.githubusercontent.com/6020549/104248276-89ec3980-54ac-11eb-8b0d-0bb90e33473e.JPG)

# Fonts support
![Font1](https://user-images.githubusercontent.com/6020549/104248202-6628f380-54ac-11eb-893d-08fdbfd5ee93.JPG)
It's possible to text rotation and invert.   
![Font2](https://user-images.githubusercontent.com/6020549/104248205-675a2080-54ac-11eb-8268-c7b585061205.JPG)
![Font3](https://user-images.githubusercontent.com/6020549/104248206-67f2b700-54ac-11eb-9b94-2cc37f0360e3.JPG)
![Font4](https://user-images.githubusercontent.com/6020549/104248208-688b4d80-54ac-11eb-8b88-d87b5b084e71.JPG)
It's possible to indicate more than one font at the same time.   
![Font5](https://user-images.githubusercontent.com/6020549/104248209-6923e400-54ac-11eb-8812-a438ca1be724.JPG)

# Image support
BMP file   
![Image1](https://user-images.githubusercontent.com/6020549/104248104-3974dc00-54ac-11eb-9b97-56a062f13e5b.JPG)
JPEG file(ESP32 only)   
![Image2](https://user-images.githubusercontent.com/6020549/104248108-3aa60900-54ac-11eb-8545-3c8971a344a9.JPG)
PNG file(ESP32 only)    
![Image3](https://user-images.githubusercontent.com/6020549/104248110-3b3e9f80-54ac-11eb-9487-a0379f40fd35.JPG)

# Using Wemos D1 ESP32   
I bought this for $4.   
![WeMos-R32-11](https://user-images.githubusercontent.com/6020549/107591015-09585e80-6c4d-11eb-9198-d882408d0d4a.JPG)

I have changed some pin assignments.   
Attached the TFT shield.   
The TFT shield worked fine.   
![WeMos-R32-12](https://user-images.githubusercontent.com/6020549/107591018-0c534f00-6c4d-11eb-81d1-22f141ea1044.JPG)

It's very smart.   
![WeMos-R32-13](https://user-images.githubusercontent.com/6020549/201453096-40b41987-30bf-4703-af59-3652b9bdbea3.JPG)

![WeMos-R32-1](https://user-images.githubusercontent.com/6020549/145746743-9c4a0e7c-5315-496f-8f66-129115044091.jpg)

![WeMos-R32-2](https://user-images.githubusercontent.com/6020549/107591035-12e1c680-6c4d-11eb-9c67-5967c7b8a59f.jpg)

![WeMos-R32-3](https://user-images.githubusercontent.com/6020549/107723948-ff565e80-6d25-11eb-89cc-74fcd2fa11f3.jpg)

![WeMos-R32-4](https://user-images.githubusercontent.com/6020549/107725257-2e220400-6d29-11eb-808a-a440336780d6.jpg)

__If you use OPEN-SMART TFT-Shield Products, you have to change Custom GPIO.__   

![WeMos-R32-OpenSmart](https://user-images.githubusercontent.com/6020549/145744630-bce9eac3-3039-4cf6-952f-28c8ec277ec2.jpg)

# JPEG Decoder   
The ESP-IDF component includes Tiny JPEG Decompressor.   
The document of Tiny JPEG Decompressor is [here](http://elm-chan.org/fsw/tjpgd/00index.html).   
This can reduce the image to 1/2 1/4 1/8.   

# PNG Decoder   
The ESP-IDF component includes part of the miniz library, such as mz_crc32.   
But it doesn't support all of the miniz.   
The document of miniz library is [here](https://github.com/richgel999/miniz).   

And I ported the pngle library from [here](https://github.com/kikuchan/pngle).   
This can reduce the image to any size.   

# Font File   
You can add your original font file.   
The format of the font file is the FONTX format.   
Your font file is put in font directory.   
Your font file is uploaded to SPIFFS partition using meke flash.   

Please refer [this](http://elm-chan.org/docs/dosv/fontx_e.html) page about FONTX format.   

```
FontxFile yourFont[2];
InitFontx(yourFont,"/spiffs/your_font_file_name","");
uint8_t ascii[10];
strcpy((char *)ascii, "MyFont");
lcdDrawString(dev, yourFont, x, y, ascii, color);
```

---

# 4-line resistance touch screen   
Some TFT has 4-line resistance touch screen.   
The 4-line resistor touch screen uses 4 pins.   
- X(+):Digital Output
- X(-):Digital Output/Analog Input
- Y(+):Digital Output/Analog Input
- Y(-):Digital Output


When using GPIO Parallel Interface or REGISTER Parallel Interface, you can enable 4-line resistance touch screen using menuconfig.   

ESP-IDF cannot simultaneous both digital output and analog input using a single gpio.   
Two GPIOs are required for simultaneous digital output and analog input.   
(X-) and ESP32 are connected with two wires.   
(Y+) and ESP32 are connected with two wires.   

```
(X-) ----+---- Gpio for Digital Output(Using GPIO number)
         +---- Gpio for Analog Input(Using ADC1 Channel number)

(Y+) ----+---- Gpio for Digital Output(Using GPIO number)
         +---- Gpio for Analog Input(Using ADC1 Channel number)
```

## Configuration for touch screen

![config-touch](https://user-images.githubusercontent.com/6020549/201503687-973577d5-9375-4985-aacc-b46bccbf3b58.jpg)

- ADC Channel   
When reading analog values, ESP-IDF can use ADC1 and ADC2.   
This project use ADC1 to read analog value.   
ESP32 has 8 channels: GPIO32 - GPIO39.   
ESP32S2/S3 has 10 channels: GPIO01 - GPIO10.   
Refer to the ESP32 data sheet for the relationship between ADC channels and GPIOs.   
When using ADC1_6(ADC1 Channel#6) and ADC1_7(ADC1 Channel#7), the following wiring is additionally required.   
|TFT|ESP32|ESP32S2|
|:-:|:-:|:-:|
|LCD-WR(Y+)|GPIO34|GPIO07|
|LCD-RS(X-)|GPIO35|GPIO08|

- GPIO number    
It uses four GPIOs, but the GPIOs differ depending on the TFT model.   



### OPEN-SMART 16Pin-Parallel Products   
|X(+)|X(-)|Y(+)|Y(-)|
|:-:|:-:|:-:|:-:|
|LCD_D6|LCD_RS|LCD_WR|LCD_D7|

![Touch0](https://user-images.githubusercontent.com/6020549/145660530-1aeb060d-93f7-4dc8-b6ea-fd6250cc1c44.JPG)


### OPEN-SMART TFT-Shield Products   
There is no marking about 4-line resistance touch screen.   
But 4-line resistance touch screen available.   

|X(+)|X(-)|Y(+)|Y(-)|
|:-:|:-:|:-:|:-:|
|LCD_D6|LCD_RS|LCD_WR|LCD_D7|

![Touch-OpenSmart-Sheild-1](https://user-images.githubusercontent.com/6020549/145698700-a1fb42a7-27d0-438b-aaa1-07e15c87eda7.JPG)

When using ADC1_6(ADC1 Channel#6) and ADC1_7(ADC1 Channel#7), the following wiring is additionally required.   
|TFT||ESP32|ESP32S2|
|:-:|:-:|:-:|:-:|
|LCD_WR(Y+)||ADC1_6(GPIO34)|ADC1_6(GPIO07)|
|LCD_RS(X-)||ADC1_7(GPIO35)|ADC1_7(GPIO08)|

__4-line resistance touch screen cannot be used with the Wemos D1 ESP32.__   
__Because LCD_WR (GPIO4) is ADC2_0.__

### ELEGOO TFT-Shield Products   
There is no marking about 4-line resistance touch screen.   
But 4-line resistance touch screen available.   

|X(+)|X(-)|Y(+)|Y(-)|
|:-:|:-:|:-:|:-:|
|LCD_D0|LCD_RS|LCD_CS|LCD_D1|

![Touch-Elegoo-1](https://user-images.githubusercontent.com/6020549/145734702-a7719de0-3d7e-4f70-aada-e6f1cdd0ca20.JPG)

When using ADC1_6(ADC1 Channel#6) and ADC1_7(ADC1 Channel#7), the following wiring is additionally required.   
|TFT||ESP32|ESP32S2|
|:-:|:-:|:-:|:-:|
|LCD_CS(Y+)||ADC1_6(GPIO34)|ADC1_6(GPIO07)|
|LCD_RS(X-)||ADC1_7(GPIO35)|ADC1_7(GPIO08)|

__No additional wiring is required when using the Wemos D1 ESP32.__   
__LCD_CS is assigned to ADC1_6 and LCD_RS is assigned to ADC1_7.__   

### ILI9341 2.4inch TFT-Shield with Yellow Pin-header   
There is no marking about 4-line resistance touch screen.   
But 4-line resistance touch screen available.   

|X(+)|X(-)|Y(+)|Y(-)|
|:-:|:-:|:-:|:-:|
|LCD_D0|LCD_RS|LCD_CS|LCD_D1|

![Touch-ILI9341-1](https://user-images.githubusercontent.com/6020549/145734786-a717d42d-0ad8-441a-846c-e486e07c0839.JPG)

When using ADC1_6(ADC1 Channel#6) and ADC1_7(ADC1 Channel#7), the following wiring is additionally required.   
|TFT||ESP32|ESP32S2|
|:-:|:-:|:-:|:-:|
|LCD_CS(Y+)||ADC1_6(GPIO34)|ADC1_6(GPIO07)|
|LCD_RS(X-)||ADC1_7(GPIO35)|ADC1_7(GPIO08)|

__No additional wiring is required when using the Wemos D1 ESP32.__   
__LCD_CS is assigned to ADC1_6 and LCD_RS is assigned to ADC1_7.__   

### ILI9486 3.5inch TFT-Shield   
There is no marking about 4-line resistance touch screen.   
But 4-line resistance touch screen available.   

|X(+)|X(-)|Y(+)|Y(-)|
|:-:|:-:|:-:|:-:|
|LCD_D6|LCD_RS|LCD_WR|LCD_D7|

![Touch-ILI9486-1](https://user-images.githubusercontent.com/6020549/146495127-f5013a91-fe23-467b-ae02-5dd3e58ccf45.JPG)

When using ADC1_6(ADC1 Channel#6) and ADC1_7(ADC1 Channel#7), the following wiring is additionally required.   
|TFT||ESP32|ESP32S2|
|:-:|:-:|:-:|:-:|
|LCD_WR(Y+)||ADC1_6(GPIO34)|ADC1_6(GPIO07)|
|LCD_RS(X-)||ADC1_7(GPIO35)|ADC1_7(GPIO08)|

### Other TFT   
I don't know which pin is X(+), X(-), Y(+), Y(-).   
If you find, please tell me.

## Calibrating the touch screen   
Keep touching the corner point.   
![TouchCalib-1](https://user-images.githubusercontent.com/6020549/145699311-fce1049d-bb9b-460a-9724-bab62cdbf674.JPG)
![TouchCalib-2](https://user-images.githubusercontent.com/6020549/145699316-98171b90-70c7-4359-b72e-ed2f1cd4bf13.JPG)

## Draw with touch   
If there is no touch for 20 seconds, it will end.   
![Touch-OpenSmat-16pin](https://user-images.githubusercontent.com/6020549/145699328-c065278a-39b9-4a11-b2f3-6ca5d54c6e3f.JPG)
![Touch-OpenSmart-Shield-2](https://user-images.githubusercontent.com/6020549/145698848-77901c4a-bc64-4c14-8bec-7a7c63d073c5.JPG)
![Touch-Elegoo-2](https://user-images.githubusercontent.com/6020549/145734726-68d2272b-1452-47a9-b934-647f7a741ad1.JPG)
![Touch-ILI9341-2](https://user-images.githubusercontent.com/6020549/145734734-ad6fa02b-9ba7-48e3-83a6-224e3637c18a.JPG)
![Touch-ILI9486-2](https://user-images.githubusercontent.com/6020549/146495160-94c9c2de-9ca3-4124-afb9-5109568ab60c.JPG)

## Select with touch   
If there is no touch for 20 seconds, it will end.   
I borrowed the icon from [here](https://www.flaticon.com/packs/social-media-343).   
![Touch-Icon-1](https://user-images.githubusercontent.com/6020549/201503323-3e872abe-fce5-4ba9-a154-5232f7ae1b52.JPG)
![Touch-Icon-2](https://user-images.githubusercontent.com/6020549/201503325-3cf35bcf-87cc-47b3-a44e-fa0275198814.JPG)

## How dose it works   
https://www.sparkfun.com/datasheets/LCD/HOW%20DOES%20IT%20WORK.pdf


# Application layer

![LibraryLayer](https://user-images.githubusercontent.com/6020549/118599243-30916f80-b7ea-11eb-8766-5dd8fecd66aa.jpg)

# Performance comparison using ILI9488(320x480)

|Test|GPIO parallel|REGISTER I/O parallel|I2S parallel|
|:-:|:-:|:-:|:-:|
|AddressTest|1300|430|70|
|FillTest|5320|2810|1680|
|ColorBarTest|1290|460|110|
|ArrowTest|1360|460|210|
|LineTest|3190|1280|3650|
|CircleTest|3030|1230|3310|
|RoundRectTest|3060|1220|3400|
|RectAngleTest|4110|1890|11480|
|TriangleTest|4700|2110|13290|
|DirectionTest|1440|530|340|
|HorizontalTest|1750|640|740|
|VerticalTest|1750|640|730|
|FillRectTest|2340|890|280|
|ColorTest|2630|970|400|
|BMPTest|3350|2190|1370|
|JPEGTest|3870|2990|2630|
|PNGTest|4310|3450|3050|

# Performance comparison using ILI9341(240x320)
SPI used [this](https://github.com/nopnop2002/esp-idf-ili9340).   

|Test|SPI|GPIO parallel|REGISTER I/O parallel|I2S parallel|
|:-:|:-:|:-:|:-:|:-:|
|FillTest|1620|2700|1920|1560|
|ColorBarTest|80|420|160|50|
|ArrowTest|250|460|170|140|
|LineTest|2690|1040|420|1530|
|CircleTest|2400|980|410|1370|
|RoundRectTest|2400|980|400|1390|
|RectAngleTest|5960|2010|940|6720|
|TriangleTest|6550|2120|990|7630|
|DirectionTest|420|520|200|240|
|HorizontalTest|990|720|300|580|
|VerticalTest|990|710|300|580|
|FillRectTest|160|730|300|120|
|ColorTest|240|850|330|190|
|BMPTest|1600|1930|1320|960|
|JPEGTest|2540|2940|2650|2530|
|PNGTest|2830|3210|2940|2810|



# Reference about I2S driver
https://github.com/espressif/esp-iot-solution/tree/master/components/bus

- for esp32   
 i2s_lcd_esp32_driver.c   
- for esp32s2   
 i2s_lcd_esp32s2_driver.c   
- Common header   
 i2s_lcd_driver.h   

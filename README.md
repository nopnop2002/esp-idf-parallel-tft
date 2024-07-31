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

These are OPEN-SMART TFT-Shield Products.
![OPEN-SMART-Shield](https://github.com/nopnop2002/esp-idf-parallel-tft/assets/6020549/e48327d2-4470-426b-88d5-92607208586f)


# Software requirements
ESP-IDF V5.0 or later.   
ESP-IDF V4.4 release branch reached EOL in July 2024.   

__Note for ESP-IDF V5.x__   
ESP-IDF V5.x gives this warning, but work.   
```
#warning "legacy adc driver is deprecated, please migrate to use esp_adc/adc_oneshot.h and esp_adc/adc_continuous.h for oneshot mode and continuous mode drivers respectively"
```


# Installation

```
git clone https://github.com/nopnop2002/esp-idf-parallel-tft
cd esp-idf-parallel-tft/
idf.py set-target {esp32/esp32s2/esp32s3}
idf.py menuconfig
idf.py flash
```

__Note for ESP32-S2__   
The tjpgd library is not present in ESP32-S2 ROM.   
With ESP-IDF Ver5, the JPEG decode library is now provided from the IDF Component Registry, and the JPEG decode library is now available for ESP32-S2.   
However, since the SRAM is small, a large image cannot be displayed.   
With ESP-IDF Ver4.4, you cannot be displayed JPEG files because the IDF Component Registry cannot be used.   
Due to the small SRAM capacity, there is a possibility that an error may occur when displaying PNG files.
You can avoid this error by enabling PSRAM.

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
 I2S parallel is suitable for image-intensive applications.   
 REGISTER I/O parallel is suitable for character-intensive applications.   
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
You can specify multiple fonts at the same time.   
![Font5](https://user-images.githubusercontent.com/6020549/104248209-6923e400-54ac-11eb-8812-a438ca1be724.JPG)

# Image support
BMP file   
![Image1](https://user-images.githubusercontent.com/6020549/104248104-3974dc00-54ac-11eb-9b97-56a062f13e5b.JPG)
JPEG file   
![Image2](https://user-images.githubusercontent.com/6020549/104248108-3aa60900-54ac-11eb-8545-3c8971a344a9.JPG)
PNG file   
![Image3](https://user-images.githubusercontent.com/6020549/104248110-3b3e9f80-54ac-11eb-9487-a0379f40fd35.JPG)

# Using Wemos D1 ESP32   
I bought this for $4.   
![WeMos-R32-11](https://user-images.githubusercontent.com/6020549/107591015-09585e80-6c4d-11eb-9198-d882408d0d4a.JPG)

To attach the TFT-Shield, you need to do changed some pin assignments like this.   
Because GPIO34/35/36/39 are READ ONLY.   
![WeMos-R32-1](https://user-images.githubusercontent.com/6020549/145746743-9c4a0e7c-5315-496f-8f66-129115044091.jpg)

![WeMos-R32-12](https://user-images.githubusercontent.com/6020549/107591018-0c534f00-6c4d-11eb-81d1-22f141ea1044.JPG)

With this change, GPIO will look like this:   
![WeMos-R32-2](https://user-images.githubusercontent.com/6020549/107591035-12e1c680-6c4d-11eb-9c67-5967c7b8a59f.jpg)

![WeMos-R32-3](https://user-images.githubusercontent.com/6020549/107723948-ff565e80-6d25-11eb-89cc-74fcd2fa11f3.jpg)

![WeMos-R32-4](https://user-images.githubusercontent.com/6020549/107725257-2e220400-6d29-11eb-808a-a440336780d6.jpg)

It's great because it doesn't require any wiring.   
![WeMos-R32-13](https://user-images.githubusercontent.com/6020549/201453096-40b41987-30bf-4703-af59-3652b9bdbea3.JPG)


__If you use OPEN-SMART TFT-Shield Products, you must use custom GPIO.__   
![WeMos-R32-OpenSmart](https://user-images.githubusercontent.com/6020549/145744630-bce9eac3-3039-4cf6-952f-28c8ec277ec2.jpg)



# Using TZT D1 ESP32-S3   
I bought this for $5.   
![tzt-d1-esp32s3](https://github.com/nopnop2002/esp-idf-parallel-tft/assets/6020549/ba6772f1-c8b7-4e7d-9d0a-0fb8fb8cbea5)

You can attach the TFT-Shield without changing anything.   
But I2S parallel doesn't work. I don't know why.   
![tzt-d1-esp32s3-Generic](https://github.com/nopnop2002/esp-idf-parallel-tft/assets/6020549/ea622d34-ffa9-4de0-941f-98460e10f416)
![tzt-d1-esp32s3-Open-Smart](https://github.com/nopnop2002/esp-idf-parallel-tft/assets/6020549/8316345e-cef6-4768-afbc-f42be5b4bf70)

If you use TZT D1 ESP32-S3, you must use custom GPIO.  
![config-esp32s3-generic](https://github.com/nopnop2002/esp-idf-parallel-tft/assets/6020549/607bd6c5-537a-48c1-a4aa-76030bdad451)


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

# Font File Editor(FONTX Editor)   
[There](http://elm-chan.org/fsw/fontxedit.zip) is a font file editor.   
This can be done on Windows 10.   
Developer page is [here](http://elm-chan.org/fsw_e.html).   

![fontx-editor-1](https://github.com/user-attachments/assets/76a8c96f-74c3-4583-a4f1-5664f0e81f3a)

This library uses the following as default fonts:   
- font/ILGH16XB.FNT // 8x16Dot Gothic
- font/ILGH24XB.FNT // 12x24Dot Gothic
- font/ILGH32XB.FNT // 16x32Dot Gothic
- font/ILMH16XB.FNT // 8x16Dot Mincyo
- font/ILMH24XB.FNT // 12x24Dot Mincyo
- font/ILMH32XB.FNT // 16x32Dot Mincyo

From 0x00 to 0x7f, the characters image of Alphanumeric are stored.   
From 0x80 to 0xff, the characters image of Japanese are stored.   
Changing this file will change the font.

# How to build your own font file   
step1)   
download fontxedit.exe.   

step2)   
download BDF font file from Internet.   
I downloaded from [here](https://github.com/fcambus/spleen).   
fontxedit.exe can __ONLY__ import Monospaced bitmap fonts file.   
Monospaced bitmap fonts can also be downloaded [here](https://github.com/Tecate/bitmap-fonts).

step3)   
import the BDF font file into your fontxedit.exe.   
this tool can convert from BDF to FONTX.   
![fontx-editor-2](https://github.com/user-attachments/assets/3353bf23-01f0-455d-8c9c-b56d55b4dc9c)

step4)   
adjust font size.   
![fontx-editor-3](https://github.com/user-attachments/assets/0a99fb0b-1725-472e-8310-ca57362ae6d1)

step5)   
check font pattern.   
when you have made any changes, press the apply button.   
![fontx-editor-4](https://github.com/user-attachments/assets/44b8ed95-0c3e-4507-87fa-b94c3c2349de)

step6)   
save as .fnt file from your fontedit.exe.   
![fontx-editor-5](https://github.com/user-attachments/assets/db5b62a8-3a61-49aa-8505-b906067f1111)

step7)   
upload your font file to $HOME/esp-idf-ili9340/font directory.   

step8)   
add font to use   
```
FontxFile fx32L[2];
InitFontx(fx32L,"/spiffs/LATIN32B.FNT",""); // 16x32Dot LATIN
```
Font file that From 0x00 to 0x7f, the characters image of Standard ASCII are stored.   
![AsciiCode-1](https://github.com/user-attachments/assets/9311b258-fecd-460f-a349-599b2a6c1fbc)

Font file that From 0x80 to 0xff, the characters image of Japanese are stored.   
![AsciiCode-2](https://github.com/user-attachments/assets/dc6c5733-9ad4-4e24-b65d-9b1905f99a45)

Font file that From 0x80 to 0xff, the characters image of Latin are stored.   
![AsciiCode-3](https://github.com/user-attachments/assets/32778605-48d2-4dd4-b268-985adc2bef02)

# Convert from TTF font to FONTX font  
step1)   
Download WFONTX64.exe from [here](https://github.com/nemuisan/WFONTX64/releases).
Developer page is [here](https://github.com/nemuisan/WFONTX64).   

step2)   
Select ttf font.   
![WFINTX64-1](https://github.com/user-attachments/assets/0fc874b5-4ffa-4cba-b22f-9430b406ba04)

step3)   
Enter Height, Width, FontX2 name.   
Specify half of Height for Width.   
Specify your favorite font name in the FontX2 name field using up to 8 characters.   
![WFINTX64-2](https://github.com/user-attachments/assets/6cdc69af-891e-4f43-8c65-8db0a12e10d7)

step4)   
Specify the file name to save.   
![WFINTX64-3](https://github.com/user-attachments/assets/3588bca1-e03f-42ca-bbe0-d8f9627b20c9)

step5)   
Specify the font style as required.   
![WFINTX64-4](https://github.com/user-attachments/assets/9246bd4f-615c-4676-a451-a8c2a39b83e5)

step6)   
Press the RUN button to convert TTF fonts to FONTX format.   
![WFINTX64-4](https://github.com/user-attachments/assets/68530333-6375-4796-b0ca-38b3bd1ffc98)

![tft_font](https://github.com/user-attachments/assets/7149863b-cea0-4a11-9364-27af31c6f51d)

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

![config-touch-1](https://github.com/nopnop2002/esp-idf-parallel-tft/assets/6020549/5efd74fe-3b3e-4cbe-b04a-266cee5a44ee)

- ADC Channel   
When reading analog values, ESP-IDF can use ADC1 and ADC2.   
This project use ADC1 to read analog value.   
ESP32 has 8 channels: GPIO32 - GPIO39.   
ESP32S2/S3 has 10 channels: GPIO01 - GPIO10.   
Refer to the ESP32 data sheet for the relationship between ADC channels and GPIOs.   
When using ADC1_6(ADC1 Channel#6) and ADC1_7(ADC1 Channel#7), the following wiring is additionally required.   

|TFT|ADC1|ESP32|ESP32S2/S3|
|:-:|:-:|:-:|:-:|
|LCD-WR(Y+)|Channel#6|GPIO34|GPIO07|
|LCD-RS(X-)|Channel#7|GPIO35|GPIO08|

- GPIO number    
It uses four GPIOs, but the GPIOs differ depending on the TFT model.   


![config-touch-2](https://github.com/nopnop2002/esp-idf-parallel-tft/assets/6020549/c306cc6b-26e7-46e8-ac8b-b31918576d11)

- Save calibration data to NVS   
Write calibration data to NVS.   
Read calibration data from NVS when starting the firmware and use it.   
If you use the same TFT, you don't need to calibrate again.   
To clear the calibration data recorded in NVS, execute the following command.   
```
idf.py erase_flash
```


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
|TFT||ESP32|ESP32S2/S3|
|:-:|:-:|:-:|:-:|
|LCD_WR(Y+)||ADC1_6(GPIO34)|ADC1_6(GPIO07)|
|LCD_RS(X-)||ADC1_7(GPIO35)|ADC1_7(GPIO08)|

__4-line resistance touch screen cannot be used with the Wemos D1 ESP32.__   
__Because the combination of GPIO and ADC does not match.__

### ELEGOO TFT-Shield Products   
There is no marking about 4-line resistance touch screen.   
But 4-line resistance touch screen available.   

|X(+)|X(-)|Y(+)|Y(-)|
|:-:|:-:|:-:|:-:|
|LCD_D0|LCD_RS|LCD_CS|LCD_D1|

![Touch-Elegoo-1](https://user-images.githubusercontent.com/6020549/145734702-a7719de0-3d7e-4f70-aada-e6f1cdd0ca20.JPG)

When using ADC1_6(ADC1 Channel#6) and ADC1_7(ADC1 Channel#7), the following wiring is additionally required.   
|TFT||ESP32|ESP32S2/S3|
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
|TFT||ESP32|ESP32S2/S3|
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
|TFT||ESP32|ESP32S2/S3|
|:-:|:-:|:-:|:-:|
|LCD_WR(Y+)||ADC1_6(GPIO34)|ADC1_6(GPIO07)|
|LCD_RS(X-)||ADC1_7(GPIO35)|ADC1_7(GPIO08)|

__4-line resistance touch screen cannot be used with the Wemos D1 ESP32.__   
__Because the combination of GPIO and ADC does not match.__

### Other TFT   
I don't know which pin is X(+), X(-), Y(+), Y(-).   
If you find, please tell me.

## Calibrating the touch screen   
Keep touching the corner point.   
![TouchCalib-1](https://user-images.githubusercontent.com/6020549/145699311-fce1049d-bb9b-460a-9724-bab62cdbf674.JPG)
![TouchCalib-2](https://user-images.githubusercontent.com/6020549/145699316-98171b90-70c7-4359-b72e-ed2f1cd4bf13.JPG)

## Draw with touch   
If there is no touch for 10 seconds, it will end.   
![Touch-OpenSmat-16pin](https://user-images.githubusercontent.com/6020549/145699328-c065278a-39b9-4a11-b2f3-6ca5d54c6e3f.JPG)
![Touch-OpenSmart-Shield-2](https://user-images.githubusercontent.com/6020549/145698848-77901c4a-bc64-4c14-8bec-7a7c63d073c5.JPG)
![Touch-Elegoo-2](https://user-images.githubusercontent.com/6020549/145734726-68d2272b-1452-47a9-b934-647f7a741ad1.JPG)
![Touch-ILI9341-2](https://user-images.githubusercontent.com/6020549/145734734-ad6fa02b-9ba7-48e3-83a6-224e3637c18a.JPG)
![Touch-ILI9486-2](https://user-images.githubusercontent.com/6020549/146495160-94c9c2de-9ca3-4124-afb9-5109568ab60c.JPG)

## Button with touch   
You can only enter up to 15 characters.   
If there is no touch for 10 seconds, it will end.   
![Touch-Keyboard](https://github.com/nopnop2002/esp-idf-parallel-tft/assets/6020549/76bab0dd-4262-40fe-a248-64a29602eada)

## Move with touch   
If there is no touch for 10 seconds, it will end.   
![Touch-Move-1](https://github.com/nopnop2002/esp-idf-parallel-tft/assets/6020549/10eac755-d770-43c8-80ec-68ea59fcb49a)
![Touch-Move-2](https://github.com/nopnop2002/esp-idf-parallel-tft/assets/6020549/617abf8d-2cb6-46f2-9b80-2d9680aacac0)

## Menu with Touch   
If there is no touch for 10 seconds, it will end.   
![Touch-Menu-1](https://github.com/nopnop2002/esp-idf-parallel-tft/assets/6020549/0852357a-fb20-441f-be65-288c9ec1e8e7)
![Touch-Menu-2](https://github.com/nopnop2002/esp-idf-parallel-tft/assets/6020549/50cc507d-95fa-4713-ac38-f944e3943873)

## Select image with touch   
If there is no touch for 10 seconds, it will end.   
I borrowed the icon from [here](https://www.flaticon.com/packs/social-media-343).   
![Touch-Icon-1](https://github.com/nopnop2002/esp-idf-parallel-tft/assets/6020549/f3b6de6c-f63c-4b4f-9323-e5fad03d0720)
![Touch-Icon-2](https://github.com/nopnop2002/esp-idf-parallel-tft/assets/6020549/29a4afdd-8ca3-4ce4-aa92-bf8edb84314d)

## How dose it works   
https://www.sparkfun.com/datasheets/LCD/HOW%20DOES%20IT%20WORK.pdf


# Application layer

![LibraryLayer](https://user-images.githubusercontent.com/6020549/118599243-30916f80-b7ea-11eb-8766-5dd8fecd66aa.jpg)

# Performance comparison using ILI9488(320x480)   
I2S parallel is not always faster.   
Compiling with the O2 option doesn't help much.   
|Test|GPIO parallel|REGISTER I/O parallel|I2S parallel|I2S parallel(O2)|
|:-:|:-:|:-:|:-:|:-:|
|AddressTest|820|280|40|30|
|FillTest|3900|2370|1620|1590|
|ColorBarTest|810|290|60|40|
|ArrowTest|860|300|160|140|
|LineTest|2060|860|3150|2900|
|CircleTest|1950|830|2840|2610|
|RoundRectTest|1970|820|2930|2690|
|RectAngleTest|2810|1340|5180|4880|
|TriangleTest|2960|1530|6090|5640|
|DirectionTest|900|330|250|230|
|HorizontalTest|1110|430|610|550|
|VerticalTest|1110|430|610|550|
|FillRectTest|1300|590|200|140|
|ColorTest|1660|650|220|180|
|BMPTest|7280|6500|6130|5790|
|JPEGTest|1210|490|170|140|
|PNGTest|1480|750|440|1010|

![graph](https://github.com/nopnop2002/esp-idf-parallel-tft/assets/6020549/1f7ae5e8-f2e8-4e4f-8d90-7f2d2cf52ec5)

# Performance comparison using ILI9341(240x320)
SPI used [this](https://github.com/nopnop2002/esp-idf-ili9340).   

|Test|SPI|GPIO parallel|REGISTER I/O parallel|I2S parallel|
|:-:|:-:|:-:|:-:|:-:|
|FillTest|1620|2700|1920|1560|
|ColorBarTest|80|410|150|30|
|ArrowTest|250|450|160|140|
|LineTest|2770|1030|430|1570|
|CircleTest|2470|970|410|1410|
|RoundRectTest|2510|970|400|1430|
|RectAngleTest|5990|2000|940|4050|
|TriangleTest|6950|2110|990|4740|
|DirectionTest|420|500|190|240|
|HorizontalTest|1030|710|290|590|
|VerticalTest|1010|710|290|590|
|FillRectTest|170|710|340|120|
|ColorTest|250|850|340|150|
|BMPTest|7470|6880|6360|6120|
|JPEGTest|2530|800|350|150|
|PNGTest|2800|1070|610|420|



# Reference about I2S driver
https://github.com/espressif/esp-iot-solution/tree/master/components/bus

- for esp32   
 i2s_lcd_esp32_driver.c   
- for esp32s2   
 i2s_lcd_esp32s2_driver.c   
- for esp32s3   
 8080_lcd_esp32s3.c   
- Common header   
 i2s_lcd_driver.h   

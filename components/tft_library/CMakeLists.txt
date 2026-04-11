set(component_srcs
	"fontx.c"
	"ili9225.c"
	"ili9320.c"
	"ili9325.c"
	"ili9327.c"
	"ili9341.c"
	"ili9342.c"
	"ili9481.c"
	"ili9486.c"
	"ili9488.c"
	"lcd_com.c"
	"lcd_lib.c"
	"lgdp4532.c"
	"r61509.c"
	"st7781.c"
	"s6d1121.c"
	"hx8347.c"
)

if(IDF_VERSION_MAJOR STREQUAL "5")
	list(APPEND component_srcs "i2s_lcd_esp32_driver.c")
	list(APPEND component_srcs "i2s_lcd_esp32s2_driver.c")
	list(APPEND component_srcs "8080_lcd_esp32s3.c")
endif()

if(IDF_VERSION_MAJOR STREQUAL "5")
idf_component_register(SRCS "${component_srcs}"
	PRIV_REQUIRES driver
	PRIV_REQUIRES esp_adc
	INCLUDE_DIRS ".")
endif()

if(IDF_VERSION_MAJOR STREQUAL "6")
idf_component_register(SRCS "${component_srcs}"
	PRIV_REQUIRES esp_driver_gpio
	PRIV_REQUIRES esp_adc
	INCLUDE_DIRS ".")
endif()

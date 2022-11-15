set(srcs "main.c"
	"decode_png.c"
	"pngle.c"
	)

if (IDF_VERSION_MAJOR STREQUAL "4")
	list(APPEND srcs "decode_jpeg_v4.c")
endif()
if (IDF_VERSION_MAJOR STREQUAL "5")
	list(APPEND srcs "decode_jpeg_v5.c")
endif()

idf_component_register(SRCS ${srcs} INCLUDE_DIRS ".")

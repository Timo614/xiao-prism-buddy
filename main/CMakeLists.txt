set(MODEL_DIR ./model)
file(GLOB_RECURSE MODEL_SOURCES ${MODEL_DIR}/*.c)

set(VIEW_DIR ./view)
file(GLOB_RECURSE VIEW_SOURCES ${VIEW_DIR}/*.c)

set(CONTROLLER_DIR ./controller)
file(GLOB_RECURSE CONTROLLER_SOURCES ${CONTROLLER_DIR}/*.c)

FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/main/*.*)

idf_component_register(
	SRCS "main.cpp" "lv_port.cpp" ${UI_SOURCES} ${MODEL_SOURCES} ${VIEW_SOURCES} ${CONTROLLER_SOURCES}
	INCLUDE_DIRS "." ${MODEL_DIR} ${VIEW_DIR} ${CONTROLLER_DIR}
	EMBED_TXTFILES timeapi_cert.pem open-meteo-com.pem
	REQUIRES mbedtls esp-tls esp_http_client lwip json LovyanGFX lvgl esp_event Grove_Gesture
)
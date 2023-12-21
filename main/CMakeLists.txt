set(MODEL_DIR ./model)
file(GLOB_RECURSE MODEL_SOURCES ${MODEL_DIR}/*.*)

set(VIEW_DIR ./view)
file(GLOB_RECURSE VIEW_SOURCES ${VIEW_DIR}/*.*)

set(CONTROLLER_DIR ./controller)
file(GLOB_RECURSE CONTROLLER_SOURCES ${CONTROLLER_DIR}/*.*)

set(UI_DIR ./ui)
file(GLOB_RECURSE UI_SOURCES ${UI_DIR}/*.*)

FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/main/*.*)

idf_component_register(
	SRCS "main.cpp" "lv_port.cpp" ${MODEL_SOURCES} ${VIEW_SOURCES} ${CONTROLLER_SOURCES} ${UI_SOURCES}
	INCLUDE_DIRS "." ${MODEL_DIR} ${VIEW_DIR} ${CONTROLLER_DIR} ${UI_DIR}
	EMBED_TXTFILES timeapi_cert.pem open-meteo-com.pem api-coingecko-com.pem
	REQUIRES mbedtls esp-tls esp_http_client lwip json LovyanGFX lvgl esp_event esp_wifi Grove_Gesture
)
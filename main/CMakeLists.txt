set(MODEL_DIR ./model)
file(GLOB_RECURSE MODEL_SOURCES ${MODEL_DIR}/*.cpp)

set(VIEW_DIR ./view)
file(GLOB_RECURSE VIEW_SOURCES ${VIEW_DIR}/*.cpp)

set(CONTROLLER_DIR ./controller)
file(GLOB_RECURSE CONTROLLER_SOURCES ${CONTROLLER_DIR}/*.cpp)

FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/main/*.*)

idf_component_register(
	SRCS "main.cpp" "lv_port.cpp" ${UI_SOURCES} ${MODEL_SOURCES} ${VIEW_SOURCES} ${CONTROLLER_SOURCES}
	INCLUDE_DIRS "." ${MODEL_DIR} ${VIEW_DIR} ${CONTROLLER_DIR}
	EMBED_TXTFILES timeapi_cert.pem
	REQUIRES LovyanGFX lvgl esp_event)
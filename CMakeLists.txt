cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

idf_build_set_property(COMPILE_OPTIONS "-DLV_MEM_SIZE=98304" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-I../main" APPEND)

project(xiao-prism-buddy)

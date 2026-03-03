cmake_minimum_required(VERSION 3.16)
project(sv_qtcommon LANGUAGES CXX)

file(GLOB_RECURSE SV_QTCOMMON_SOURCES "*.cpp" "*.h")

add_library(sv_qtcommon STATIC ${SV_QTCOMMON_SOURCES})

target_include_directories(sv_qtcommon PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(sv_qtcommon PRIVATE sv_common)
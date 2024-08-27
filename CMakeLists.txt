cmake_minimum_required(VERSION 3.10)
project(chess)

set(CMAKE_C_STANDARD 99)

set(SRC_DIR src)
set(BUILD_DIR build)
set(BIN_DIR bin)

file(GLOB SRC_FILES ${SRC_DIR}/*.c)
add_executable(chess ${SRC_FILES})

find_package(SDL2 REQUIRED)

include_directories(include ${SDL2_INCLUDE_DIRs})

target_link_libraries(chess /usr/local/Cellar/sdl2/2.30.5/lib/libSDL2.dylib)

add_custom_command(TARGET chess POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:chess>/assets)

set_target_properties(chess PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY "${BUILD_DIR}"
    LIBRARY_OUTPUT_DIRECTORY "${BUILD_DIR}"
    INSTALL_RPATH_USE_LINK_PATH TRUE
    BUILD_RPATH /usr/local/Cellar/sdl2/2.30.5/lib
    INSTALL_RPATH /usr/local/Cellar/sdl2/2.30.5/lib
)

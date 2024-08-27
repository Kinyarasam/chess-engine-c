cmake_minimum_required(VERSION 3.10)
project(chess)

set(CMAKE_C_STANDARD 99)
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra -pedantic -std=gnu89")

set(SRC_DIR src)
set(BUILD_DIR build)
set(BIN_DIR bin)

file(GLOB SRC_FILES ${SRC_DIR}/*.c)
add_executable(chess ${SRC_FILES})

find_package(SDL2 REQUIRED)

set(SDL2_TTF_INCLUDE_DIR "/usr/local/Cellar/sdl2_ttf/2.22.0/include/SDL2")
set(SDL2_TTF_LIBRARY "/usr/local/Cellar/sdl2_ttf/2.22.0/lib/libSDL2_ttf.dylib")

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL_TTF DEFAULT_MSG SDL2_TTF_LIBRARY SDL2_TTF_INCLUDE_DIR)

if(SDL_TTF_FOUND)
    set(SDL_TTF_LIBRARIES ${SDL2_TTF_LIBRARY})
    set(SDL_TTF_INCLUDE_DIRS ${SDL2_TTF_INCLUDE_DIR})
endif()

include_directories(include ${SDL2_INCLUDE_DIRS} ${SDL_TTF_INCLUDE_DIRS})

target_link_libraries(chess /usr/local/Cellar/sdl2/2.30.5/lib/libSDL2.dylib /usr/local/Cellar/sdl2_ttf/2.22.0/lib/libSDL2_ttf.dylib)

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
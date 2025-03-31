cmake_minimum_required(VERSION 3.10)
project(chess LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Project structure
set(SRC_DIR src)
set(INCLUDE_DIR include)
set(ASSETS_DIR assets)
set(BIN_DIR bin)

# Source files
file(GLOB SRC_FILES 
    ${SRC_DIR}/*.c
    ${SRC_DIR}/*.h
)

# Find SDL2 packages
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf)

# Create executable
add_executable(chess ${SRC_FILES})

# Include directories
target_include_directories(chess PRIVATE
    ${INCLUDE_DIR}
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(chess PRIVATE
    ${SDL2_LIBRARIES}
)

if(SDL2_TTF_FOUND)
    target_link_libraries(chess PRIVATE ${SDL2_TTF_LIBRARIES})
endif()

# Copy assets directory to build directory
add_custom_command(TARGET chess POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/${ASSETS_DIR} 
    $<TARGET_FILE_DIR:chess>/${ASSETS_DIR}
)

# Output directories
set_target_properties(chess PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Installation (optional)
install(TARGETS chess DESTINATION bin)
install(DIRECTORY ${ASSETS_DIR} DESTINATION .)

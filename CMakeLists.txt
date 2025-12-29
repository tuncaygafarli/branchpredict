cmake_minimum_required(VERSION 3.16)
project(CPUINSIGHT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(BUILD_TESTS "Build tests" OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4)  # REMOVED: /WX
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)  # REMOVED: -Werror
endif()

# --------------------------------------------------------------------
# SFML Setup - Multiple fallback strategies
# --------------------------------------------------------------------
set(SFML_FOUND FALSE)

# Strategy 1: Check if user has SFML in vendor/SFML-2.6.1 (pre-compiled)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vendor/SFML-2.6.1/include/SFML")
    message(STATUS "Found pre-compiled SFML in vendor folder")
    set(SFML_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/vendor/SFML-2.6.1")
    set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")
    
    # Determine library suffix based on build type
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(SFML_LIB_SUFFIX "-d")
    else()
        set(SFML_LIB_SUFFIX "")
    endif()
    
    set(SFML_GRAPHICS_LIB "${SFML_ROOT}/lib/sfml-graphics${SFML_LIB_SUFFIX}.lib")
    set(SFML_WINDOW_LIB "${SFML_ROOT}/lib/sfml-window${SFML_LIB_SUFFIX}.lib")
    set(SFML_SYSTEM_LIB "${SFML_ROOT}/lib/sfml-system${SFML_LIB_SUFFIX}.lib")
    
    # Check if libraries exist
    if(EXISTS "${SFML_GRAPHICS_LIB}" AND 
       EXISTS "${SFML_WINDOW_LIB}" AND 
       EXISTS "${SFML_SYSTEM_LIB}")
        set(SFML_FOUND TRUE)
        message(STATUS "Using pre-compiled SFML libraries")
    endif()
endif()

# Strategy 2: Use FetchContent to download and build SFML from source
if(NOT SFML_FOUND)
    message(STATUS "Downloading SFML 2.6.1 via FetchContent...")
    
    include(FetchContent)
    
    FetchContent_Declare(
        sfml
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        2.6.1
        GIT_SHALLOW    TRUE
    )
    
    # Set SFML options before making available
    set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)
    
    FetchContent_MakeAvailable(sfml)
    
    if(TARGET sfml-graphics)
        set(SFML_FOUND TRUE)
        message(STATUS "SFML downloaded and will be built from source")
    endif()
endif()

# Strategy 3: Try system installation
if(NOT SFML_FOUND)
    message(STATUS "Looking for system SFML...")
    find_package(SFML 2.6 QUIET COMPONENTS graphics window system)
    if(SFML_FOUND)
        message(STATUS "Found system SFML: ${SFML_INCLUDE_DIR}")
    endif()
endif()

if(NOT SFML_FOUND)
    message(FATAL_ERROR "
SFML 2.6.1 not found. Options:
1. Download SFML 2.6.1 from https://www.sfml-dev.org/download.php
   Extract to: vendor/SFML-2.6.1/
   
2. Install via vcpkg: vcpkg install sfml:x64-windows
   
3. Let CMake download it automatically (recommended)
   - Delete build folder and reconfigure
")
endif()

# --------------------------------------------------------------------
# Main Executable
# --------------------------------------------------------------------
add_executable(${PROJECT_NAME}
    main.cpp
)

# Add all source files from subdirectories
file(GLOB_RECURSE SRC_FILES 
    "gui/*.cpp"
    "parser/*.cpp" 
    "cpu/*.cpp"
    # Add more directories as needed
)

target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES})

# --------------------------------------------------------------------
# Include Directories
# --------------------------------------------------------------------
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}          # For aliases.h
    ${CMAKE_CURRENT_SOURCE_DIR}/gui
    ${CMAKE_CURRENT_SOURCE_DIR}/parser
    ${CMAKE_CURRENT_SOURCE_DIR}/cpu
    ${CMAKE_CURRENT_SOURCE_DIR}/fonts
)

# Add SFML includes based on how it was found
if(TARGET sfml-graphics)
    # SFML was built via FetchContent, includes are automatic
elseif(SFML_INCLUDE_DIR)
    target_include_directories(${PROJECT_NAME} PRIVATE ${SFML_INCLUDE_DIR})
endif()

# --------------------------------------------------------------------
# Link Libraries
# --------------------------------------------------------------------
if(TARGET sfml-graphics)
    # Link to FetchContent targets
    target_link_libraries(${PROJECT_NAME} PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
    )
elseif(SFML_GRAPHICS_LIB)
    # Link to pre-compiled libraries
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${SFML_GRAPHICS_LIB}
        ${SFML_WINDOW_LIB}
        ${SFML_SYSTEM_LIB}
    )
elseif(SFML_FOUND)
    # Link to system libraries
    target_link_libraries(${PROJECT_NAME} PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
    )
endif()

# Windows-specific dependencies
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        opengl32
        winmm
        gdi32
        #freetype  # Only if SFML was built with static freetype
    )
    
    # For static linking
    if(BUILD_SHARED_LIBS)
        message(STATUS "Using dynamic SFML libraries")
    else()
        message(STATUS "Using static SFML libraries")
        target_compile_definitions(${PROJECT_NAME} PRIVATE SFML_STATIC)
    endif()
endif()

# --------------------------------------------------------------------
# Copy Resources
# --------------------------------------------------------------------
# Copy fonts to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    # Copy fonts
    COMMAND ${CMAKE_COMMAND} -E make_directory
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/fonts"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/fonts"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/fonts"
    
    # Copy asm_tests
    COMMAND ${CMAKE_COMMAND} -E make_directory
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/asm_tests"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/asm_tests"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>/asm_tests"
    
    COMMENT "Copying resources to binary directory"
)

# Copy SFML DLLs if using dynamic linking on Windows
if(WIN32 AND BUILD_SHARED_LIBS AND EXISTS "${SFML_ROOT}/bin")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_ROOT}/bin/openal32.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_ROOT}/bin/sfml-graphics-2.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_ROOT}/bin/sfml-window-2.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_ROOT}/bin/sfml-system-2.dll"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        COMMENT "Copying SFML DLLs to binary directory"
    )
endif()

# --------------------------------------------------------------------
# Tests (Optional)
# --------------------------------------------------------------------
if(BUILD_TESTS AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/asm_tests)
    enable_testing()
    add_subdirectory(asm_tests)
endif()

# --------------------------------------------------------------------
# Installation (Optional)
# --------------------------------------------------------------------
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION bin
)

install(DIRECTORY fonts/
    DESTINATION share/${PROJECT_NAME}/fonts
)

# --------------------------------------------------------------------
# Print Configuration Summary
# --------------------------------------------------------------------
message(STATUS "")
message(STATUS "=========================================")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "SFML found: ${SFML_FOUND}")
if(SFML_ROOT)
    message(STATUS "SFML location: ${SFML_ROOT}")
endif()
message(STATUS "Output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "=========================================")
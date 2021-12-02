cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(amd-miner LANGUAGES CXX)

# Add miner version at compile time, priority is tag, short hash and finally unknown
find_package(Git)
if(Git_FOUND)
    # Find current tag name :
    execute_process(COMMAND
            "${GIT_EXECUTABLE}" describe --tags --abbrev=0 --exact-match
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            OUTPUT_VARIABLE miner-version
            ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
    # If no tag was found, set to hash abbreviation
    if(NOT miner-version)
        execute_process(COMMAND
                "${GIT_EXECUTABLE}" describe --match=NeVeRmAtCh --always --abbrev=7
                WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                OUTPUT_VARIABLE miner-version
                ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
        set(miner-version canary-${miner-version})
    else()
        set(miner-version release-${miner-version})
    endif()
else()
    set(miner-version unknown)
endif()

message(miner version : ${miner-version})

#Setup conan
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake")
endif()
include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_cmake_run(REQUIRES libuv/1.42.0
        opencl-headers/2021.06.30
        BASIC_SETUP)

# Find the proper OpenCL lib, bail if it can't be found
find_package( OpenCL REQUIRED )
if(Git_FOUND)
    include_directories( ${OPENCL_INCLUDE_DIR} )
else()
    message( FATAL_ERROR "Was unable to locate OpenCL, ensure it is available to build the miner." )
endif()

# Target definitions
add_executable(amd-miner src/main.cc)
set_property(TARGET amd-miner PROPERTY CXX_STANDARD 11)
target_compile_definitions(amd-miner PUBLIC MINER_VERSION="${miner-version}")
target_link_libraries(amd-miner PRIVATE ${CONAN_LIBS} OpenCL::OpenCL)
#Copy binary to bin directory
add_custom_command(TARGET amd-miner
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:amd-miner> ${CMAKE_SOURCE_DIR}/bin)
cmake_minimum_required(VERSION 3.14)
project(dstu8845 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|ARM64")
    include(FetchContent)
    FetchContent_Declare(
        sse2neon
        GIT_REPOSITORY https://github.com/DLTcollab/sse2neon.git
        GIT_TAG        master
    )
    FetchContent_MakeAvailable(sse2neon)
endif()

add_library(strumok_lib STATIC strumok.cpp)

target_include_directories(strumok_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|ARM64")
    target_include_directories(strumok_lib PUBLIC ${sse2neon_SOURCE_DIR})
    target_compile_options(strumok_lib PRIVATE -O3 -mcpu=native -ffast-math -flto -fomit-frame-pointer)
else()
    target_compile_options(strumok_lib PRIVATE -O3 -march=native -ffast-math -flto -fomit-frame-pointer)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_executable(strumok_tests test.cpp)
    target_link_libraries(strumok_tests PRIVATE strumok_lib)

    if(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|ARM64")
        target_compile_options(strumok_tests PRIVATE -O3 -mcpu=native -ffast-math -flto -fomit-frame-pointer)
    else()
        target_compile_options(strumok_tests PRIVATE -O3 -march=native -ffast-math -flto -fomit-frame-pointer)
    endif()
endif()

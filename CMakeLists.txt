cmake_minimum_required(VERSION 4.0)
set(CMAKE_COLOR_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_COMPILER "clang++")

project("Adventure_miner" LANGUAGES CXX VERSION 1.0.0 DESCRIPTION "A simple game" HOMEPAGE_URL "https://github.com/FTS427/adventure_miner")

set(CMAKE_CXX_STANDARD 23)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
    message(STATUS "No build type specified, defaulting to Debug")
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address,undefined")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-as-needed")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--start-group")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--end-group")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-undefined -Wl,--as-needed")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--strip-all")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--strip-debug")

add_compile_options(
    "$<$<CONFIG:DEBUG>:-g;-O0>"
    "$<$<CONFIG:RELEASE>:-O3;-DNDEBUG>"
)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

find_package(fmt REQUIRED)
find_package(tomlplusplus REQUIRED)

include_directories("src/include")
aux_source_directory("src" SRC)

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} fmt::fmt tomlplusplus::tomlplusplus)

add_definitions(-DPROJECT_NAME="${PROJECT_NAME}")
add_definitions(-DPROJECT_VERSION="${PROJECT_VERSION}")
add_definitions(-DPROJECT_REPO="${PROJECT_HOMEPAGE_URL}")
add_definitions(-DPROJECT_LICENSE="GPL-3.0")
add_definitions(-DPROJECT_DESCRIPTION="${PROJECT_DESCRIPTION}")
add_definitions(-DPROJECT_AUTHOR="Huang FTS427")

option(ENABLE_CCACHE "Use ccache if available" ON)
find_program(CCACHE_PROGRAM ccache)
if(ENABLE_CCACHE AND CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

cmake_minimum_required(VERSION 3.13)
project(matchx)

set(ENGINE matchx)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES src/*.cpp)
add_executable(${ENGINE} ${SOURCES})

target_include_directories(${ENGINE} PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Redis++
find_path(HIREDIS_HEADER hiredis)
target_include_directories(${ENGINE} PUBLIC ${HIREDIS_HEADER})

find_library(HIREDIS_LIB hiredis)
target_link_libraries(${ENGINE} ${HIREDIS_LIB})

find_path(REDIS_PLUS_PLUS_HEADER sw)
target_include_directories(${ENGINE} PUBLIC ${REDIS_PLUS_PLUS_HEADER})

find_library(REDIS_PLUS_PLUS_LIB redis++)
target_link_libraries(${ENGINE} ${REDIS_PLUS_PLUS_LIB})

# Installing PLOG
include(FetchContent)

FetchContent_Declare(
    plog
    GIT_REPOSITORY https://github.com/SergiusTheBest/plog
    GIT_TAG        1.1.11
    GIT_SHALLOW    true
)
FetchContent_MakeAvailable(plog)

target_link_libraries(${ENGINE} plog::plog)

# Installing YAML-CPP
# include(FetchContent)
# 
# FetchContent_Declare(
#   yaml-cpp
#   GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
#   GIT_TAG 0.8.0
# )
# FetchContent_MakeAvailable(yaml-cpp)
# 
# target_link_libraries(${ENGINE} yaml-cpp::yaml-cpp)

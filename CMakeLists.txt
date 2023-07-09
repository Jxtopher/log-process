cmake_minimum_required(VERSION 3.12)

###############################################################################
# Set of compilator parameters
###############################################################################
project(lp)
if (CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_program(GCOV_PROGRAM gcov)
if(GCOV_PROGRAM)
    message(STATUS "Found gcov: ${GCOV_PROGRAM}")
else()
    message(STATUS "Could NOT find gcov")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # using Clang
  set(FLAGS "-Wall -Wextra")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  # using GCC
  set(FLAGS "-pedantic -Wall -Wextra -Wcast-align -Wctor-dtor-privacy \
  -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op \
  -Wmissing-include-dirs -Wno-unused -Wshadow \
  -Woverloaded-virtual -Wredundant-decls -Wsign-promo \
  -Wstrict-null-sentinel -Wstrict-overflow=2 -Wundef -Werror \
  -Wswitch-default -Wsign-conversion -Wcast-qual -Wold-style-cast -Wmissing-declarations")  
  set(COV_FLAGS "-fprofile-arcs -ftest-coverage")
endif()
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

###############################################################################
# Show variables defintion
###############################################################################
message(STATUS "PROJECT_NAME=${PROJECT_NAME}")
message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}")
message(STATUS "COV_FLAGS=${COV_FLAGS}")

###############################################################################
# Load modules cmake
###############################################################################
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CTest)
include(cppcheck)       # apply cppcheck
include(clang-format)   # apply clang-format
include(version)        # Version name generator form git branch, hash

###############################################################################
# Get external libs
###############################################################################
find_package(Boost 1.71 COMPONENTS program_options REQUIRED)

###############################################################################
# APPS
###############################################################################
file(GLOB LIST_OF_FILES  apps/*.cpp)
foreach(PATH_FILE  ${LIST_OF_FILES})
    get_filename_component(NAME_FILE ${PATH_FILE} NAME_WE)
    add_executable(${PROJECT_NAME} ${PATH_FILE})

    target_include_directories(${PROJECT_NAME} PRIVATE 
        "${CMAKE_BINARY_DIR}/generated/"
        ${Boost_INCLUDE_DIRS}
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS ${FLAGS})
endforeach()

###############################################################################
# Tests
###############################################################################
find_package(GTest REQUIRED)

enable_testing()
file(GLOB_RECURSE ALL_SOURCE_FILES  "test/*.cpp")
foreach (SOURCE_FILE ${ALL_SOURCE_FILES})
    get_filename_component(NAME_FILE ${SOURCE_FILE} NAME_WE)
    add_executable(${NAME_FILE}.test ${SOURCE_FILE})
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_link_libraries(${NAME_FILE}.test LINK_PRIVATE
            ${GTEST_LIBRARIES}
        )
    else()
        target_link_libraries(${NAME_FILE}.test LINK_PRIVATE
            ${GTEST_LIBRARIES}
            gcov
        )
    endif()
    target_include_directories(${NAME_FILE}.test PRIVATE 
        "/usr/local/include/"
        ${GTEST_INCLUDE_DIRS}
    )
    set_target_properties(${NAME_FILE}.test PROPERTIES COMPILE_FLAGS "${FLAGS} ${COV_FLAGS}")
    add_test(NAME ${NAME_FILE} COMMAND ${NAME_FILE}.test WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
endforeach()
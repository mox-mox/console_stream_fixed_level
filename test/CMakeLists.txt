# Build script for the test program
cmake_minimum_required(VERSION 3.0)
project(console_stream)

#{{{Variables: SRCS INCDIRS LINKDIRS LIBS CMAKE_MODULE_PATH

file(GLOB SRCS *.cpp)
set(INCDIRS "${CMAKE_CURRENT_SOURCE_DIR}")
set(INCDIRS_SYS "")
set(LIBS "")
#}}}


add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/.." "${CMAKE_CURRENT_BINARY_DIR}/console_streams")




#{{{CXXFLAGS

# Set build type when generating the Makefile like: cmake -DCMAKE_BUILD_TYPE=Debug ..
set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS}         -std=c++11 -Wall -Wextra -Wpedantic -Werror -Wfatal-errors -Os -DNOTIFY_LEVEL=3")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -std=c++11 -Wall -Wextra -Wpedantic -Werror -Wfatal-errors -O0 -ggdb -DDEBUG -save-temps -DNOTIFY_LEVEL=3")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11 -Wall -Wextra -Wpedantic -Werror -Wfatal-errors -Os -DNDEBUG -DNOTIFY_LEVEL=0")
#}}}


add_executable(console_stream_test ${SRCS})
target_link_libraries(console_stream_test ${LIBS} )
target_include_directories(console_stream_test PUBLIC ${INCDIRS})
target_include_directories(console_stream_test SYSTEM PUBLIC ${INCDIRS_SYS})
target_compile_definitions(console_stream_test PUBLIC LOGGING_LEVEL=0 USE_COLOUR_STREAMS=1)

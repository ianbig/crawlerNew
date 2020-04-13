cmake_minimum_required(VERSION 3.6)
project(crawler_multi_thread)

set(CMAKE_CXX_STANDARD 17)
set(GCC_COVERAGE_COMPILE_FLAGS "-lcurl -I/usr/local/include -L/usr/local/lib  -lgumbo")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} " )

set(SOURCE_FILES main.cpp master.cpp master.h Parser.cpp Parser.h HashTable.cpp HashTable.h md5/src/md5.cpp md5/src/md5.h gumbo-parser/src/gumbo.h)
add_executable(crawler_multi_thread ${SOURCE_FILES})

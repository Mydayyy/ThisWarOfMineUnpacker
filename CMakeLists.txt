cmake_minimum_required(VERSION 3.7)
project(ThisWarOfMineUnpacker)

set(CMAKE_CXX_STANDARD 17)

find_package(ZLIB REQUIRED)
if (ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
endif()

set(SOURCE_FILES main.cpp GameFile.cpp GameFile.h Z.cpp Z.h murmur.h)
add_executable(ThisWarOfMineUnpacker ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${ZLIB_LIBRARIES})
target_link_libraries(${PROJECT_NAME} stdc++fs)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

PROJECT(GAME)
AUX_SOURCE_DIRECTORY(. SOURCES)
ADD_EXECUTABLE(game ${SOURCES})
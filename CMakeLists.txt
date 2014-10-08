CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
CMAKE_POLICY(SET CMP0015 NEW)

PROJECT(BlueMars)

LIST(APPEND CMAKE_CXX_FLAGS "-std=c++0x")

INCLUDE_DIRECTORIES(./bluemars)
SET(SOURCE_FILES 
bluemars/bluemars.cpp
bluemars/baselayer.cpp
bluemars/chunkdata.cpp
bluemars/flat.cpp
)

ADD_EXECUTABLE(BlueMars ${SOURCE_FILES})

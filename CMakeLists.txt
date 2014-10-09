CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
CMAKE_POLICY(SET CMP0015 NEW)

PROJECT(BlueMars)
SET( BlueMars_MAJOR_VERSION 0 )
SET( BlueMars_MINOR_VERSION 1 )
SET( BlueMars_REVISION_VERSION 0 )
LIST(APPEND CMAKE_CXX_FLAGS "-std=c++0x")

# Give the user the possibility to enable/disable modules
SET( BlueMars_BUILD_SHARED_LIBS true CACHE BOOL "Build shared library." )
SET( BlueMars_BUILD_2D_VISUALIZER false CACHE BOOL "Build 2D Visualizer" )
SET( BlueMars_BUILD_UNITTEST false CACHE BOOL "Build unit tests." )

# Find dependancies
## For 2D Viz
IF( BlueMars_BUILD_2D_VISUALIZER )
	FIND_PACKAGE( SFGUI REQUIRED )
ENDIF()
## For unittest
IF( BlueMars_BUILD_UNITTEST )
	ADD_SUBDIRECTORY( ./unittest/gtest )
	INCLUDE_DIRECTORIES( ./unittest/gtest/include )
ENDIF()

# Includes
INCLUDE_DIRECTORIES(./bluemars)
IF( BlueMars_BUILD_2D_VISUALIZER )
	INCLUDE_DIRECTORIES(./2DVisualizer)
ENDIF()
IF( BlueMars_BUILD_UNITTEST )
	INCLUDE_DIRECTORIES(./unittest)
ENDIF()

# Sources
SET( BLUEMARS_SOURCE_FILES 
bluemars/bluemars.h
bluemars/bluemars.cpp
bluemars/baselayer.h
bluemars/baselayer.cpp
bluemars/chunkdata.h
bluemars/chunkdata.cpp
bluemars/flat.h
bluemars/flat.cpp
)

IF( BlueMars_BUILD_2D_VISUALIZER )
	SET( 2DVISUALIZER_SOURCE_FILES
	)
ENDIF()

IF( BlueMars_BUILD_UNITTEST )
	SET( UNITTEST_SOURCE_FILES
	unittest/unittest.cpp
	)
ENDIF()

# Let's build
IF ( BlueMars_BUILD_SHARED_LIBS )
	ADD_LIBRARY(BlueMars SHARED ${BLUEMARS_SOURCE_FILES} )
ELSE()
	## ToDo - Implement possibility to build static
	MESSAGE( FATAL_ERROR "Building static libraries is not supported yet!" )
ENDIF()

IF ( BlueMars_BUILD_2D_VISUALIZER )
	ADD_EXECUTABLE( BlueMars2DVisualizer ${2DVISUALIZER_SOURCE_FILES} )
ENDIF()

IF ( BlueMars_BUILD_UNITTEST )
	ADD_EXECUTABLE( unittests ${UNITTEST_SOURCE_FILES} )
	TARGET_LINK_LIBRARIES( unittests gtest_main )
ENDIF()

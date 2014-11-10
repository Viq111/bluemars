CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
CMAKE_POLICY(SET CMP0015 NEW)

PROJECT(BlueMars)
SET( BlueMars_MAJOR_VERSION 0 )
SET( BlueMars_MINOR_VERSION 1 )
SET( BlueMars_REVISION_VERSION 0 )

IF ( CMAKE_COMPILER_IS_GNUCXX )
	SET( CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wextra -Wshadow -Wconversion -Wno-long-long -pedantic ${CMAKE_CXX_FLAGS}" )
ENDIF()

# Give the user the possibility to enable/disable modules
SET( BUILD_SHARED_LIBS true CACHE BOOL "Build shared library." )
SET( BlueMars_BUILD_2D_VISUALIZER false CACHE BOOL "Build 2D Visualizer" )
SET( BlueMars_BUILD_UNITTEST false CACHE BOOL "Build unit tests." )

# For Visual Studio need to change the flags when compiling static libs, need to change flags
# See http://www.cmake.org/Wiki/CMake_FAQ#How_can_I_build_my_MSVC_application_with_a_static_runtime.3F
IF ( WIN32 )
	IF ( BUILD_SHARED_LIBS )
		foreach(flag_var
		CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
		CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
			if(${flag_var} MATCHES "/MT")
				string(REGEX REPLACE "/MT" "/MD" ${flag_var} "${${flag_var}}")
			endif(${flag_var} MATCHES "/MT")
		endforeach(flag_var)
	ELSE()
		foreach(flag_var
		CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
		CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
			if(${flag_var} MATCHES "/MD")
				string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
			endif(${flag_var} MATCHES "/MD")
		endforeach(flag_var)
	ENDIF()
ENDIF()


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
bluemars/perlin.h
bluemars/perlin.cpp
bluemars/simpleAdditionLayer.h
bluemars/simpleAdditionLayer.cpp
bluemars/elevationlayer.h
bluemars/elevationlayer.cpp
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
IF ( BUILD_SHARED_LIBS )
	ADD_LIBRARY(BlueMars SHARED ${BLUEMARS_SOURCE_FILES} )
ELSE()
	ADD_LIBRARY(BlueMars ${BLUEMARS_SOURCE_FILES} )
ENDIF()

IF ( BlueMars_BUILD_2D_VISUALIZER )
	ADD_EXECUTABLE( BlueMars2DVisualizer ${2DVISUALIZER_SOURCE_FILES} )
ENDIF()

IF ( BlueMars_BUILD_UNITTEST )
	ADD_EXECUTABLE( unittests ${UNITTEST_SOURCE_FILES} )
	TARGET_LINK_LIBRARIES( unittests gtest_main)
	TARGET_LINK_LIBRARIES( unittests BlueMars )
ENDIF()

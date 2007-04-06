PROJECT(bibletime-tests CXX C)

cmake_minimum_required(VERSION 2.4.0)

find_package(Qt4 REQUIRED) # find and setup Qt4 for this project

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/build) 

INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/../src/ ${PROJECT_SOURCE_DIR})

SET(DEBUG true)

SET(osisparser_SOURCES
	backend/test_osisparser.cpp
)
qt4_automoc(${osisparser_SOURCES})
SET(osisparser_MOC_HEADERS)
ADD_EXECUTABLE(test-osisparser ${osisparser_SOURCES})

#
# Autoconfiguration section
#

#
#Find and include QT4
#
SET(QT_DONT_USE_QTGUI false)
IF (NOT QT4_FOUND)
	MESSAGE(FATAL_ERROR "Qt 4 was not found on your system.")
ENDIF (NOT QT4_FOUND)

INCLUDE_DIRECTORIES(
	${QT_INCLUDE_DIR}
	${QT_QTCORE_INCLUDE_DIR}
)

TARGET_LINK_LIBRARIES(test-osisparser
	${PROJECT_SOURCE_DIR}/../build/lib/libBackend.a
	QtCore QtTest
)


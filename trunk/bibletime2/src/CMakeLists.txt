PROJECT(bibletime CXX C)

cmake_minimum_required(VERSION 2.4.0)

find_package(Qt4 REQUIRED) # find and setup Qt4 for this project

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin) 
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SET(DEBUG true)

SET(bibletime_SOURCES
	main.cpp
)
SET(bibletime_MOC_HEADERS )

SET(libbackend_SOURCES
	backend/osisparser.cpp
)

#bibletime_MOC_SOURCES will contain a list of source files created by Qt's moc
#QT4_WRAP_CPP(bibletime_MOC_SOURCES ${bibletime_MOC_HEADERS})

#Add all source files to the binary
ADD_LIBRARY(Backend ${libbackend_SOURCES} )
ADD_EXECUTABLE(bibletime ${bibletime_SOURCES})

SET_TARGET_PROPERTIES(bibletime
	PROPERTIES 
		COMPILE_FLAGS "-Werror" #All Warnings are Errors
)

#
# Install locations
#
IF(DEBUG)
	SET(CMAKE_INSTALL_PREFIX install)
ELSE(DEBUG)
	SET(CMAKE_INSTALL_PREFIX /usr/local)
ENDIF(DEBUG)

INSTALL(TARGETS bibletime
	DESTINATION bin
)

#
# Autoconfiguration section
#

#
#Find and include QT4
#
SET(QT_DONT_USE_QTGUI true)
IF (NOT QT4_FOUND)
	MESSAGE(FATAL_ERROR "Qt 4 was not found on your system.")
ENDIF (NOT QT4_FOUND)
INCLUDE_DIRECTORIES(
	${QT_INCLUDE_DIR}
	${QT_QTCORE_INCLUDE_DIR}
)
TARGET_LINK_LIBRARIES(bibletime
	Backend
	QtCore
)


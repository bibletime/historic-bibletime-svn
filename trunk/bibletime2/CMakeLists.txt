PROJECT(bibletime CXX C)

SET(DEBUG true)


SET(bibletime_SOURCES
	src/backend/osisparser.cpp
	src/main.cpp
)

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
INCLUDE(FindQt4)
IF (NOT QT4_FOUND)
	MESSAGE(FATAL_ERROR "Qt 4 was not found on your system.")
ENDIF (NOT QT4_FOUND)
INCLUDE_DIRECTORIES(
	${QT_INCLUDE_DIR}
	${QT_QTCORE_INCLUDE_DIR}
)
TARGET_LINK_LIBRARIES(bibletime
	QtCore
)


PROJECT(Parsertest CXX C)

ADD_EXECUTABLE(parser parser.cpp)

##########################################################
#
#Find and include QT4
#
INCLUDE(FindQt4)
INCLUDE_DIRECTORIES(
	${QT_INCLUDE_DIR}
	${QT_QTCORE_INCLUDE_DIR}
)
IF (NOT QT4_FOUND)
	MESSAGE(FATAL_ERROR "Qt 4 was not found on your system.")
ENDIF (NOT QT4_FOUND)
##########################################################

TARGET_LINK_LIBRARIES(parser
	QtCore
)


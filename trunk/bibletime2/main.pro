TEMPLATE = app

CONFIG = debug
#CONFIG = release

#DEPENDPATH += .
#INCLUDEPATH += .


##########################################################
#
# BUILD SECTION
# Make sure build files are placed under build/
#
OBJECTS_DIR = build/obj
MOC_DIR 		= build/moc
UI_DIR			= build/uic
RCC_DIR			= build/rcc/ipe
TARGET			= build/bibletime2
##########################################################


##########################################################
#
# INSTALL SECTION
# Install to target/ by default, to /usr on release
#
install_base					= target
release:install_base	= /usr

install_binary.files	= build/bibletime2
install_binary.path 	= $$install_base/bin/

INSTALLS 							+= install_binary
##########################################################


##########################################################
#
# QT SECTION
#
QT 			+= xml sql
CONFIG 	+= qt stl exceptions
##########################################################



##########################################################
#
# SOURCECODE SECTION
# Include files according to subdir
#

# src/
FORMS 			+=
HEADERS 		+=
SOURCES 		+= src/main.cpp
RESOURCES 	+=

# src/frontend/
#FORMS 			+=
#HEADERS 		+=
#SOURCES 		+=
#RESOURCES 	+=
 
##########################################################




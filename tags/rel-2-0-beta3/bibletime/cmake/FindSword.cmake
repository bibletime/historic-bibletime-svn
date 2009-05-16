SET (REQUIRED_SWORD_VERSION 1.5.9)
SET (SIMPLE_RENDER_SWORD_VERSION 1.5.10)
SET (MULTIVERSE_SWORD_VERSION 1.5.11.1)
SET (SYSCONF_CHANGED_SWORD_VERSION 1.5.11.98)
#SET (INTERNET_WARNING_SWORD_VERSION 1.5.11.98) # same as SYSCONF_CHANGED_SWORD_VERSION, treated together

# This module looks for installed sword
#
# It will define the following values
# SWORD_INCLUDE_DIR
# SWORD_LIBRARY
# SWORD_LIBRARY_DIR
# SWORD_CFLAGS

SET(TRIAL_LIBRARY_PATHS
	$ENV{SWORD_HOME}/lib${LIB_SUFFIX}
	${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}
	/usr/local/lib${LIB_SUFFIX}
	/opt/local/lib${LIB_SUFFIX}
	/usr/lib${LIB_SUFFIX}
	/usr/lib64
	/usr/pkg/lib${LIB_SUFFIX}
	/sw/lib${LIB_SUFFIX}
	../sword-1.5.11/lib/vcppmake/vc8/ICUDebug
	../sword-1.5.11/lib/vcppmake/vc8/ICURelease
)
SET(TRIAL_INCLUDE_PATHS
	$ENV{SWORD_HOME}/include/sword
	$ENV{SWORD_HOME}/include
	${CMAKE_INSTALL_PREFIX}/include/sword
	${CMAKE_INSTALL_PREFIX}/include
	/usr/local/include/sword
	/usr/local/include
	/usr/include/sword
	/usr/include
	/sw/include/sword
	/sw/include
	/usr/pkg/include/sword
	/usr/pkg/include
	../sword-1.5.11/include
)

FIND_LIBRARY(SWORD_LIBRARY sword NAMES libsword PATHS ${TRIAL_LIBRARY_PATHS})
IF (SWORD_LIBRARY)
	MESSAGE(STATUS "Found Sword library: ${SWORD_LIBRARY}")
	SET(SWORD_LIBRARY "sword")
ELSE (SWORD_LIBRARY)
	MESSAGE(FATAL_ERROR "Could not find the Sword library.")
ENDIF (SWORD_LIBRARY)

FIND_PATH(SWORD_LIBRARY_DIR	NAMES libsword.a libsword.la libsword.so libsword.lib PATHS ${TRIAL_LIBRARY_PATHS})
IF (SWORD_LIBRARY_DIR)
	MESSAGE(STATUS "Found Sword library dir: ${SWORD_LIBRARY_DIR}")
ELSE (SWORD_LIBRARY_DIR)
	MESSAGE(FATAL_ERROR "Could not find the Sword library dir.")
ENDIF (SWORD_LIBRARY_DIR)

FIND_PATH(SWORD_INCLUDE_DIR	NAMES swmgr.h PATHS ${TRIAL_INCLUDE_PATHS})
IF (SWORD_INCLUDE_DIR)
	MESSAGE(STATUS "Found Sword include dir: ${SWORD_INCLUDE_DIR}")
ELSE (SWORD_INCLUDE_DIR)
	MESSAGE(FATAL_ERROR "Sword include dir could not be found.")
ENDIF (SWORD_INCLUDE_DIR)

#
# Check for minimum Sword version
#
MACRO(CHECK_SWORD_VERSION VERSION)
	TRY_RUN(SWVERSIONTEST_RUN_RESULT SWVERSIONTEST_COMPILE_RESULT
		${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sword_version_compare.cpp
		CMAKE_FLAGS
			"-DINCLUDE_DIRECTORIES:STRING=${SWORD_INCLUDE_DIR}"
			"-DLINK_DIRECTORIES:STRING=${SWORD_LIBRARY_DIR}"
			"-DLINK_LIBRARIES:STRING=${SWORD_LIBRARY}"
		COMPILE_OUTPUT_VARIABLE SWVERSIONTEST_COMPILE_OUTPUT
		RUN_OUTPUT_VARIABLE SWVERSIONTEST_RUN_OUTPUT
		ARGS ${VERSION}
	)
	IF(NOT SWVERSIONTEST_COMPILE_RESULT)
		MESSAGE(FATAL_ERROR "Sword version check program could NOT be compiled: ${SWVERSIONTEST_COMPILE_OUTPUT}")
	ENDIF(NOT SWVERSIONTEST_COMPILE_RESULT)
ENDMACRO(CHECK_SWORD_VERSION VERSION)

MESSAGE(STATUS "Checking for required Sword version ${REQUIRED_SWORD_VERSION}...")
CHECK_SWORD_VERSION(${REQUIRED_SWORD_VERSION})

IF(SWVERSIONTEST_RUN_RESULT EQUAL 0)
	MESSAGE(STATUS "Installed Sword version is ok. Check program said: ${SWVERSIONTEST_RUN_OUTPUT}")
ELSE(SWVERSIONTEST_RUN_RESULT EQUAL 0)
	MESSAGE(FATAL_ERROR "Installed Sword version is NOT ok! Check program said: ${SWVERSIONTEST_RUN_OUTPUT}")
ENDIF(SWVERSIONTEST_RUN_RESULT EQUAL 0)

#
# Check for API changes that require compiler definitions
#

# SIMPLE_RENDER
CHECK_SWORD_VERSION(${SIMPLE_RENDER_SWORD_VERSION})
IF(SWVERSIONTEST_RUN_RESULT EQUAL 0)
	SET (SWORD_CFLAGS "${SWORD_CFLAGS} -DSWORD_SIMPLERENDER")
	MESSAGE(STATUS "	Found simple render version of Sword.")
ENDIF(SWVERSIONTEST_RUN_RESULT EQUAL 0)

# MULTIVERSE
CHECK_SWORD_VERSION(${MULTIVERSE_SWORD_VERSION})
IF(SWVERSIONTEST_RUN_RESULT EQUAL 0)
	SET (SWORD_CFLAGS "${SWORD_CFLAGS} -DSWORD_MULTIVERSE")
	MESSAGE(STATUS "	Found multiverse version of Sword.")
ENDIF(SWVERSIONTEST_RUN_RESULT EQUAL 0)

# SYSCONF_CHANGED and INTERNET_WARNING
CHECK_SWORD_VERSION(${SYSCONF_CHANGED_SWORD_VERSION})
IF(SWVERSIONTEST_RUN_RESULT EQUAL 0)
	SET (SWORD_CFLAGS "${SWORD_CFLAGS} -DSWORD_SYSCONF_CHANGED -DSWORD_INTERNET_WARNING")
	MESSAGE(STATUS "	Found sysconf_changed and internet_warning version of Sword.")
ENDIF(SWVERSIONTEST_RUN_RESULT EQUAL 0)

MESSAGE(STATUS "SWORD_CFLAGS: ${SWORD_CFLAGS}")

#
# Sword linker flag detection
#
MACRO(CHECK_SWORD_LINK_LIBRARIES FLAGS)
	TRY_COMPILE(SWLINKER_CHECK_COMPILE_RESULT
	${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sword_linker_check.cpp
	CMAKE_FLAGS
		"-DINCLUDE_DIRECTORIES:STRING=${SWORD_INCLUDE_DIR}"
		"-DLINK_DIRECTORIES:STRING=${SWORD_LIBRARY_DIR};${CLUCENE_LIBRARY_DIR}"
		"-DLINK_LIBRARIES:STRING=${FLAGS}"
	OUTPUT_VARIABLE SWLINKER_CHECK_COMPILE_OUTPUT
)
ENDMACRO(CHECK_SWORD_LINK_LIBRARIES FLAGS)

#CURL_LIBRARIES and ICU_LIBRARIES are optional, empty if not found
CHECK_SWORD_LINK_LIBRARIES("${CLUCENE_LIBRARY};${SWORD_LIBRARY};${CURL_LIBRARIES};${ICU_LIBRARIES};${ICU_I18N_LIBRARIES}")

IF(SWLINKER_CHECK_COMPILE_RESULT)
	MESSAGE(STATUS "Sword linker check compiled ok.")
	# SWORD_LIBRARY can stay unchanged
ELSE(SWLINKER_CHECK_COMPILE_RESULT)
	MESSAGE(STATUS "Sword linker check could NOT be compiled. It seems that you need additional libraries for the linker.")
	MESSAGE(STATUS "Here is the detailed output of the compilation and linking process:")
	MESSAGE(FATAL_ERROR "${SWLINKER_CHECK_COMPILE_OUTPUT}")
ENDIF(SWLINKER_CHECK_COMPILE_RESULT)


MARK_AS_ADVANCED(
	SWORD_INCLUDE_DIR
	SWORD_LIBRARY
	SWORD_LIBRARY_DIR
	SWORD_CFLAGS
)

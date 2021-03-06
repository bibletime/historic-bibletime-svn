SET (REQUIRED_SWORD_VERSION 1.5.9)
SET (SIMPLE_RENDER_SWORD_VERSION 1.5.10)
SET (MULTIVERSE_SWORD_VERSION 1.5.11.1)
SET (SYSCONF_CHANGED_SWORD_VERSION 1.5.11.98)
SET (INTERNET_WARNING_SWORD_VERSION 1.5.11.98)

# This module looks for installed sword
#
# It will define the following values
# SWORD_INCLUDE_DIR
# SWORD_LIBS

IF (WIN32)

  SET(TRIAL_LIBRARY_PATHS
    $ENV{SWORD_HOME}/lib${LIB_SUFFIX}
    ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}
    /usr/lib${LIB_SUFFIX}
    /usr/local/lib${LIB_SUFFIX}
    /sw/lib${LIB_SUFFIX}
    ../sword-1.5.11/lib/vcppmake/vc8/ICUDebug
    ../sword-1.5.11/lib/vcppmake/vc8/ICURelease
  )
  SET(TRIAL_INCLUDE_PATHS
    $ENV{SWORD_HOME}/include/sword
    $ENV{SWORD_HOME}/include
    ${CMAKE_INSTALL_PREFIX}/include/sword
    ${CMAKE_INSTALL_PREFIX}/include
    /sw/include
    ../sword-1.5.11/include
  )

  FIND_LIBRARY(SWORD_LIBRARY sword
      NAMES libsword
      PATHS ${TRIAL_LIBRARY_PATHS})
  IF (SWORD_LIBRARY)
    MESSAGE(STATUS "Found Sword library: ${SWORD_LIBRARY}")
  ENDIF (SWORD_LIBRARY)
  FIND_PATH(SWORD_INCLUDE_DIR
      NAMES swmgr.h
      PATHS ${TRIAL_INCLUDE_PATHS})

  IF (SWORD_INCLUDE_DIR)
    MESSAGE(STATUS "Found Sword include dir: ${SWORD_INCLUDE_DIR}")
  ENDIF (SWORD_INCLUDE_DIR)

ELSE(WIN32)

	EXECUTE_PROCESS(COMMAND pkg-config --atleast-version=${REQUIRED_SWORD_VERSION} sword
		RESULT_VARIABLE SWORD_VERSION_OK
	)
	EXECUTE_PROCESS(COMMAND pkg-config --atleast-version=${SIMPLE_RENDER_SWORD_VERSION} sword
		RESULT_VARIABLE SIMPLE_RENDER_SWORD_OK
	)
	EXECUTE_PROCESS(COMMAND pkg-config --atleast-version=${MULTIVERSE_SWORD_VERSION} sword
		RESULT_VARIABLE MULTIVERSE_SWORD_OK
	)
	EXECUTE_PROCESS(COMMAND pkg-config --atleast-version=${SYSCONF_CHANGED_SWORD_VERSION} sword
		RESULT_VARIABLE SYSCONF_CHANGED_SWORD_OK
	)
	EXECUTE_PROCESS(COMMAND pkg-config --atleast-version=${INTERNET_WARNING_SWORD_VERSION} sword
		RESULT_VARIABLE INTERNET_WARNING_SWORD_OK
	)


	EXECUTE_PROCESS(COMMAND pkg-config --modversion sword
		OUTPUT_VARIABLE SWORD_VERSION
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	IF (SWORD_VERSION_OK EQUAL 0)

		MESSAGE(STATUS "Sword version ${SWORD_VERSION} is ok")

		EXECUTE_PROCESS(COMMAND pkg-config --variable=includedir sword
			OUTPUT_VARIABLE SWORD_INCLUDE_DIR
			OUTPUT_STRIP_TRAILING_WHITESPACE
		)
		SET(SWORD_INCLUDE_DIR "${SWORD_INCLUDE_DIR}/sword")
		MESSAGE(STATUS "Sword include directory is ${SWORD_INCLUDE_DIR}")

		EXECUTE_PROCESS(COMMAND pkg-config --libs sword
			OUTPUT_VARIABLE SWORD_LIBS
			OUTPUT_STRIP_TRAILING_WHITESPACE
		)
		MESSAGE(STATUS "Sword lib linker flags to be used: ${SWORD_LIBS}")

	ELSE (SWORD_VERSION_OK EQUAL 0)

		IF (SWORD_VERSION)
	 		MESSAGE(FATAL_ERROR "Sword version ${SWORD_VERSION} does not match the required ${REQUIRED_SWORD_VERSION}")
	 	ELSE (SWORD_VERSION)
	 		MESSAGE(FATAL_ERROR "Sword not found")
	 	ENDIF (SWORD_VERSION)

	ENDIF (SWORD_VERSION_OK EQUAL 0)

ENDIF (WIN32)


IF (MULTIVERSE_SWORD_OK EQUAL 0)
	SET (SWORD_CFLAGS "${SWORD_CFLAGS} -DSWORD_MULTIVERSE")
ENDIF (MULTIVERSE_SWORD_OK EQUAL 0)
IF (SIMPLE_RENDER_SWORD_OK EQUAL 0)
	SET (SWORD_CFLAGS "${SWORD_CFLAGS} -DSWORD_SIMPLERENDER")
ENDIF (SIMPLE_RENDER_SWORD_OK EQUAL 0)
IF (SYSCONF_CHANGED_SWORD_OK EQUAL 0)
	SET (SWORD_CFLAGS "${SWORD_CFLAGS} -DSWORD_SYSCONF_CHANGED")
ENDIF (SYSCONF_CHANGED_SWORD_OK EQUAL 0)
IF (INTERNET_WARNING_SWORD_OK EQUAL 0)
	SET (SWORD_CFLAGS "${SWORD_CFLAGS} -DSWORD_INTERNET_WARNING")
ENDIF (INTERNET_WARNING_SWORD_OK EQUAL 0)

MARK_AS_ADVANCED(
	SWORD_INCLUDE_DIR
	SWORD_LIBS
	SWORD_CFLAGS
)

MESSAGE(STATUS "SWORD_CFLAGS: ${SWORD_CFLAGS}")

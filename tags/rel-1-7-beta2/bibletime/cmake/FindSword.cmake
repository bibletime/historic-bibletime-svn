SET (REQUIRED_SWORD_VERSION 1.5.9)

# This module looks for installed sword
#
# It will define the following values
# SWORD_INCLUDE_DIR
# SWORD_LIBS

EXECUTE_PROCESS(COMMAND pkg-config --atleast-version=${REQUIRED_SWORD_VERSION} sword
	RESULT_VARIABLE SWORD_VERSION_OK
)
EXECUTE_PROCESS(COMMAND pkg-config --modversion sword
	OUTPUT_VARIABLE SWORD_VERSION
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

IF (SWORD_VERSION_OK EQUAL 0)

	EXECUTE_PROCESS(COMMAND pkg-config --variable=includedir sword
		OUTPUT_VARIABLE SWORD_INCLUDE_DIR
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	MESSAGE(STATUS "Sword version ${SWORD_VERSION} is ok")
	
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

SET(SWORD_INCLUDE_DIR "${SWORD_INCLUDE_DIR}/sword")
MESSAGE(STATUS "Sword include directory is ${SWORD_INCLUDE_DIR}")

MARK_AS_ADVANCED(
	SWORD_INCLUDE_DIR
	SWORD_LIBS
)

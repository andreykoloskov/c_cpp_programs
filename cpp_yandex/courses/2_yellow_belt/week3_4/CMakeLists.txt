include("${MAIN_INCLUDE}")

file(GLOB HEADER_NAME *.h *.hpp)
file(GLOB CPP_NAMES *.cpp)
file(GLOB DIRECTORIES true *)

foreach(CPP_NAME ${CPP_NAMES})
	include("${DEFINITION_FILES_INCLUDE}")
	include("${INNER_INCLUDE}")
endforeach()

foreach(DIRECTORY ${DIRECTORIES})
    if (IS_DIRECTORY ${DIRECTORY})
		include("${DEFINITION_DIRECTORIES_INCLUDE}")
		include("${INNER_INCLUDE}")
	endif()
endforeach()


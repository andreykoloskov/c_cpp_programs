include("${MAIN_INCLUDE}")

file(GLOB HEADER_NAME *.h *.hpp)
file(GLOB CPP_NAMES *.cpp)

foreach(CPP_NAME ${CPP_NAMES})
	include("${DEFINITION_FILES_INCLUDE}")
	include("${INNER_INCLUDE}")
endforeach()


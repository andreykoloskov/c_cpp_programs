include("${MAIN_INCLUDE}")

foreach(NUMBER RANGE 1 18)
	set(CURRENT_NAME "main${NUMBER}")
	set(CPP_NAME "${CURRENT_NAME}.cpp")	
	include("${INNER_INCLUDE}")
endforeach()

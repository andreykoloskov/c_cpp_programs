include("${MAIN_INCLUDE}")

foreach(NUMBER RANGE 1 9)
	set(CURRENT_NAME "main${NUMBER}")	
	include("${INNER_INCLUDE}")
endforeach()

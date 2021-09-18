include("${MAIN_INCLUDE}")

foreach(NUMBER RANGE 1 2)
	set(CURRENT_NAME "main${NUMBER}")
	set(CPP_NAME "${CURRENT_NAME}.cpp")	
	include("${INNER_INCLUDE}")
endforeach()

foreach (MAIN_NAME Person Person0 Person2 ReversibleString SortedStrings Incognizable)
	set(CURRENT_NAME "${MAIN_NAME}")
	set(CPP_NAME "${CURRENT_NAME}.cpp")	
	include("${INNER_INCLUDE}")
endforeach()

include("${MAIN_INCLUDE}")

foreach(NUMBER RANGE 1 5)
	set(CURRENT_NAME "File${NUMBER}")
	set(CPP_NAME "${CURRENT_NAME}.cpp")	
	include("${INNER_INCLUDE}")
endforeach()

foreach (MAIN_NAME Function Function2 LectureTitle Rational rational_eq_add_sub rational_interface rational_io rational_map rational_mult_div)
	set(CURRENT_NAME "${MAIN_NAME}")
	set(CPP_NAME "${CURRENT_NAME}.cpp")	
	include("${INNER_INCLUDE}")
endforeach()

include("${MAIN_INCLUDE}")

foreach (MAIN_NAME Calculator ensure_equal project_template Rational time_server)
	set(CURRENT_NAME "${MAIN_NAME}")
	set(CPP_NAME "${CURRENT_NAME}.cpp")	
	include("${INNER_INCLUDE}")
endforeach()

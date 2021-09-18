include("${MAIN_INCLUDE}")

foreach (MAIN_NAME correct main starter test_equation test_palindrome test_person test_rational)
	set(CURRENT_NAME "${MAIN_NAME}")
	set(CPP_NAME "${CURRENT_NAME}.cpp")	
	include("${INNER_INCLUDE}")
endforeach()

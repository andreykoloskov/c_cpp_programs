include("${MAIN_INCLUDE}")

foreach (MAIN_NAME GetRestrict Mass Quadro RegionBase TeamTask Temperature)
	set(CURRENT_NAME "${MAIN_NAME}")
	set(CPP_NAME "${CURRENT_NAME}.cpp")	
	include("${INNER_INCLUDE}")
endforeach()

include("../../../include.cmake")

foreach(NUMBER RANGE 1 9)
    add_executable (${PROJECT_PREFIX}main${NUMBER} "main${NUMBER}.cpp" ${INC_LOCAL})
	
	set_target_properties(${PROJECT_PREFIX}main${NUMBER} PROPERTIES FOLDER ${CURRENT_DIR})	
    set_target_properties(${PROJECT_PREFIX}main${NUMBER} PROPERTIES OUTPUT_NAME "main${NUMBER}")	
	set_target_properties(${PROJECT_PREFIX}main${NUMBER} PROPERTIES PROJECT_LABEL "main${NUMBER}")
	
    install (TARGETS ${PROJECT_PREFIX}main${NUMBER} DESTINATION bin/${CURRENT_DIR})
endforeach()

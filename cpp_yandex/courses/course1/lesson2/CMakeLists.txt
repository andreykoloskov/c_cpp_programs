include("../../../include.cmake")

foreach(NUMBER RANGE 1 18)
    add_executable (${PROJECT_PREFIX}main${NUMBER} "main${NUMBER}.cpp" ${INC_LOCAL})
	
	set_target_properties(${PROJECT_PREFIX}main${NUMBER} PROPERTIES FOLDER ${CURRENT_DIR})	
    set_target_properties(${PROJECT_PREFIX}main${NUMBER} PROPERTIES OUTPUT_NAME "main${NUMBER}")	
	set_target_properties(${PROJECT_PREFIX}main${NUMBER} PROPERTIES PROJECT_LABEL "main${NUMBER}")

    target_link_libraries(${PROJECT_PREFIX}main${NUMBER}
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIB_INIT})

    target_include_directories(${PROJECT_PREFIX}main${NUMBER}
        PRIVATE
        ${Boost_INCLUDE_DIR})
		
    install (TARGETS ${PROJECT_PREFIX}main${NUMBER} DESTINATION bin/${CURRENT_DIR})	
endforeach()

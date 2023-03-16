macro(__add_common_library target)
	if(NOT INTERFACES)
		set(INTERFACES ${CMAKE_CURRENT_SOURCE_DIR})
		if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include")
			list(APPEND INTERFACES ${CMAKE_CURRENT_SOURCE_DIR}/include)
		endif()
	endif()
	
	if(NOT SRCS)
		message(FATAL_ERROR "SRCS is empty.")
	endif()
	
	if(NOT INCS)
		set(INCS ${CMAKE_CURRENT_SOURCE_DIR})
	endif()
	
	if(NOT DEFS)
		set(DEFS)
	endif()
	if(NOT INTERFACE_DEFS)
		set(INTERFACE_DEFS)
	endif()
	
	string(TOUPPER ${target} UpperName)
			
	if(CC_GLOBAL_FORCE_STATIC OR ${UpperName}_STATIC)
		list(APPEND INTERFACE_DEFS USE_${UpperName}_STATIC)
		__add_real_target(${target} lib SOURCE ${SRCS} 
										LIB ${LIBS}
										INC ${INCS}
										DEF ${DEFS}
										INTERFACE ${INTERFACES}
										INTERFACE_DEF ${INTERFACE_DEFS}
										SOURCE_FOLDER
										${ARGN}
										)
	else()
		list(APPEND DEFS ${UpperName}_DLL)
		list(APPEND INTERFACE_DEFS USE_${UpperName}_DLL)
		__add_real_target(${target} dll SOURCE ${SRCS} 
										LIB ${LIBS}
										INC ${INCS}
										DEF ${DEFS}
										INTERFACE ${INTERFACES}
										INTERFACE_DEF ${INTERFACE_DEFS}
										SOURCE_FOLDER
										${ARGN}
										)
	endif()
endmacro()

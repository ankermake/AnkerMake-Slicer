if(Python3_Development_FOUND)
	add_definitions(-DBOOST_ALL_NO_LIB)
	add_definitions(-DBOOST_PYTHON_SOURCE)
	
	set(SRC dict.cpp
			errors.cpp
			exec.cpp
			import.cpp
			list.cpp
			long.cpp
			module.cpp
			object_operators.cpp
			object_protocol.cpp
			slice.cpp
			str.cpp
			tuple.cpp
			wrapper.cpp
			converter/arg_to_python_base.cpp
			converter/builtin_converters.cpp
			converter/from_python.cpp
			converter/registry.cpp
			converter/type_id.cpp
			object/class.cpp
			object/enum.cpp
			object/function.cpp
			object/function_doc_signature.cpp
			object/inheritance.cpp
			object/iterator.cpp
			object/life_support.cpp
			object/pickle_support.cpp
			object/stl_iterator.cpp
			)
	
	message(STATUS "boost python build ${PYTHON_INCLUDE_DIRS}/../libs/")
	link_directories("${PYTHON_INCLUDE_DIRS}/../libs/")
	
	__add_real_target(boost_python dll SOURCE ${SRC} ${HEADER}
										INC ${CMAKE_CURRENT_SOURCE_DIR}/../../ ${PYTHON_INCLUDE_DIRS}
										DEF BOOST_ALL_DYN_LINK
										INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../../)
									  
endif()
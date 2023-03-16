add_definitions(-DBOOST_ALL_NO_LIB)
add_definitions(-DBOOST_THREAD_BUILD_DLL)

set(SRC tss_null.cpp
		future.cpp
		)
		
if(WIN32)
	list(APPEND SRC win32/tss_dll.cpp
					win32/tss_pe.cpp
					win32/thread.cpp
					win32/thread_primitives.cpp
					)
else()
	list(APPEND SRC pthread/once.cpp
					pthread/thread.cpp
					)
endif()

__add_real_target(boost_thread dll SOURCE ${SRC}
									  INC ${CMAKE_CURRENT_SOURCE_DIR}/../../
									  DEF BOOST_ALL_DYN_LINK
									  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../../)
									  
if(INSTALL_BOOST)
	INSTALL(TARGETS boost_thread
				LIBRARY DESTINATION lib
				ARCHIVE DESTINATION .
				RUNTIME DESTINATION bin)
endif()
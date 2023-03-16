add_definitions(-DBOOST_ALL_NO_LIB)
add_definitions(-DBOOST_FILESYSTEM_SOURCE)
__add_boost_target(file_system)

if(INSTALL_BOOST)
	INSTALL(TARGETS boost_file_system
				LIBRARY DESTINATION lib
				ARCHIVE DESTINATION .
				RUNTIME DESTINATION bin)
endif()
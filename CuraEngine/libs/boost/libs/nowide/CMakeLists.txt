add_definitions(-DBOOST_ALL_NO_LIB)
add_definitions(-DBOOST_FILESYSTEM_SOURCE)
__add_boost_target(nowide)

if(INSTALL_BOOST)
	INSTALL(TARGETS boost_nowide
				LIBRARY DESTINATION lib
				ARCHIVE DESTINATION .
				RUNTIME DESTINATION bin)
endif()
add_definitions(-DBOOST_ALL_NO_LIB)
__add_boost_target(regex)

if(INSTALL_BOOST)
	INSTALL(TARGETS boost_regex
				LIBRARY DESTINATION lib
				ARCHIVE DESTINATION .
				RUNTIME DESTINATION bin)
endif()
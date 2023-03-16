__tree_add_current(SRCS)
__add_real_target(clipper3r lib SOURCE ${SRCS}
							   LIB ${LIBS}
							   INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
							   
if(CC_INSTALL_CLIPPERZ)
	__install_directory_specif(${CMAKE_CURRENT_SOURCE_DIR}/ clipper3r)
endif()
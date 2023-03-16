__tree_add_current(SRCS)
__add_real_target(polypartition lib SOURCE ${SRCS}
								  INC ${CMAKE_CURRENT_SOURCE_DIR}
								  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
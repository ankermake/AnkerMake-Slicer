__tree_add_current(SRCS)
__add_real_target(poly2tri lib SOURCE ${SRCS}
								  LIB ${LIBS}
								  INC ${CMAKE_CURRENT_SOURCE_DIR}/../ ${CMAKE_CURRENT_SOURCE_DIR}
								  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../)

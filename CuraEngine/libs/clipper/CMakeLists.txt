set(SRCS clipper.hpp clipper.cpp)
		 
__add_real_target(clipper lib SOURCE ${SRCS}
							  INC ${CMAKE_CURRENT_SOURCE_DIR}
							  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
							  )
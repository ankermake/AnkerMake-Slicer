set(SRCS clipper.hpp clipper.cpp)
if(${AKSS_CURA_ENGINE} MATCHES "cloud")
	add_definitions(-fPIC)
	message("CLOUD_SLICER")
endif()
__add_real_target(clipper lib SOURCE ${SRCS}
							  INC ${CMAKE_CURRENT_SOURCE_DIR}
							  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
							  )
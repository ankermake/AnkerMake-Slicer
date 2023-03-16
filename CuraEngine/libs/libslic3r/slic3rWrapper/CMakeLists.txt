set(SRCS slic3rapi/slic3rexport.h slic3rapi/slice3rapi.cpp slic3rapi/slice3rapi.h)


set(INCS ${BOOST_INCLUDE_DIRS} ${EIGEN_INCLUDE_DIRS})

__add_real_target(slic3rapi lib SOURCE ${SRCS} LIB libslic3r _CuraEngine clipper INC ${INCS} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/)
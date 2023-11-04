__files_group(${CMAKE_CURRENT_SOURCE_DIR} SRC)


set(CURA_LIB_INCS ${CMAKE_CURRENT_SOURCE_DIR} ${STB_INCLUDE_DIRS} ${RAPIDJSON_INCLUDE_DIRS} ${EIGEN_INCLUDE_DIRS} ${BOOST_INCLUDE_DIRS})
set(INCS ${CURA_LIB_INCS})
set(LIBS clipper)

set(DEFS _CRT_SECURE_NO_WARNINGS NOMINMAX)

if(${AKSS_CURA_ENGINE} MATCHES "cloud")
	add_definitions(-fPIC)
	message("CLOUD_SLICER")
endif()


__add_real_target(_CuraEngine lib SOURCE ${SRC} ${CURA_SRC}
								 LIB ${LIBS}
								 INC ${INCS}
								 DEF ${DEFS}
								 INTERFACE ${ENGINE_INCS}
								 OPENMP
								 )

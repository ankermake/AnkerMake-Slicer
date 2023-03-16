__enable_cxx17()
__assert_parameter(EIGEN_INCLUDE_DIRS)
__assert_parameter(BOOST_INCLUDE_DIRS)
__assert_parameter(RAPIDJSON_INCLUDE_DIRS)

__assert_target(boost_file_system)
__assert_target(boost_regex)
__assert_target(boost_thread)
__assert_target(qhullstatic_r)
__assert_target(qhullcpp)
__assert_target(nlopt)
__assert_target(libnest2d)
__assert_target(clipper3r)

set(EXTRA_INCLUDES ${EIGEN_INCLUDE_DIRS} ${BOOST_INCLUDE_DIRS} 
			${CEREAL_INCLUDE_DIRS} ${RAPIDJSON_INCLUDE_DIRS})
set(EXTRA_LIBS tbb  qhullstatic_r qhullcpp nlopt libnest2d clipper3r
		 boost_file_system boost_regex boost_thread)
set(EXTRA_DEFS BOOST_ALL_NO_LIB BOOST_ALL_DYN_LINK _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
	LIBNEST2D_OPTIMIZER_nlopt LIBNEST2D_GEOMETRIES_clipper)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(libslic3r)
add_subdirectory(slic3rWrapper)


								
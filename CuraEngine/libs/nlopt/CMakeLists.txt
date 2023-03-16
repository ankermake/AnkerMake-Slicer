list (APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option (NLOPT_CXX "enable cxx routines" ON)
option (BUILD_SHARED_LIBS "Build NLopt as a shared library" ON)

set (NLOPT_MAJOR_VERSION "2")
set (NLOPT_MINOR_VERSION "6")
set (NLOPT_BUGFIX_VERSION "2")
set (NLOPT_VERSION_STRING ${NLOPT_MAJOR_VERSION}.${NLOPT_MINOR_VERSION}.${NLOPT_BUGFIX_VERSION})

# This is the ABI version number, which differes from the API version above
# (it indicates ABI compatibility), but they are typically incremented together.
set(SO_MAJOR 0)
set(SO_MINOR 10)
set(SO_PATCH 0)

include (GNUInstallDirs)
if(POLICY CMP0042)
  # Set MACOSX_RPATH to ON
  cmake_policy(SET CMP0042 NEW)
endif()
if (POLICY CMP0086)
  # UseSWIG honors SWIG_MODULE_NAME via -module flag
  cmake_policy(SET CMP0086 NEW)
endif ()
if (POLICY CMP0071)
  cmake_policy(SET CMP0071 NEW)
endif()

include (CheckIncludeFiles)
include (CheckFunctionExists)
include (CheckTypeSize)
include (CheckCCompilerFlag)
include (CheckCXXSymbolExists)
include (CheckCXXCompilerFlag)
include (CheckLibraryExists)

#==============================================================================
# COMPILATION CHECKINGS and CONFIGURATION GENERATION
#==============================================================================
check_include_file (dlfcn.h HAVE_DLFCN_H)
check_include_file (getopt.h HAVE_GETOPT_H)
check_include_file (unistd.h HAVE_UNISTD_H)
check_include_file (string.h HAVE_STRING_H)
check_include_file (strings.h HAVE_STRINGS_H)
check_include_file (inttypes.h HAVE_INTTYPES_H)
check_include_file (memory.h HAVE_MEMORY_H)
check_include_file (stdlib.h HAVE_STDLIB_H)
check_include_file (stdint.h HAVE_STDINT_H)
check_include_file (time.h HAVE_TIME_H)
check_include_file (sys/types.h HAVE_SYS_TYPES_H)
check_include_file (sys/stat.h HAVE_SYS_STAT_H)
check_include_file (sys/time.h HAVE_SYS_TIME_H)
if (HAVE_TIME_H AND HAVE_SYS_TIME_H)
	set (TIME_WITH_SYS_TIME TRUE)
endif ()
check_function_exists (getpid HAVE_GETPID)
check_function_exists (syscall HAVE_GETTID_SYSCALL)
check_function_exists (isinf HAVE_ISINF)
check_function_exists (isnan HAVE_ISNAN)
check_function_exists (gettimeofday HAVE_GETTIMEOFDAY)
check_function_exists (qsort_r HAVE_QSORT_R)
check_function_exists (time HAVE_TIME)
check_function_exists (copysign HAVE_COPYSIGN)
check_type_size ("uint32_t" SIZEOF_UINT32_T)
set (HAVE_UINT32_T ${SIZEOF_UINT32_T})
check_type_size ("unsigned int" SIZEOF_UNSIGNED_INT)
check_type_size ("unsigned long" SIZEOF_UNSIGNED_LONG)

check_library_exists ("m" sqrt "" HAVE_LIBM)
if (HAVE_LIBM)
	set (M_LIBRARY m)
endif()

if (NOT DEFINED HAVE_FPCLASSIFY)
	message(STATUS "Looking for fpclassify")
	file (WRITE ${CMAKE_CURRENT_BINARY_DIR}/fpclassify.c "#include <math.h>\n")
	file (APPEND ${CMAKE_CURRENT_BINARY_DIR}/fpclassify.c "int main(void) {\n")
	file (APPEND ${CMAKE_CURRENT_BINARY_DIR}/fpclassify.c "if (!fpclassify(3.14159)) fpclassify(2.7183);\n")
	file (APPEND ${CMAKE_CURRENT_BINARY_DIR}/fpclassify.c "  return 0; }\n")
	try_compile (HAVE_FPCLASSIFY
		${CMAKE_CURRENT_BINARY_DIR}/build_fpclassify
		${CMAKE_CURRENT_BINARY_DIR}/fpclassify.c
		CMAKE_FLAGS -DLINK_LIBRARIES=m)
	message(STATUS "Looking for fpclassify - ${HAVE_FPCLASSIFY}")
endif ()

option (WITH_THREADLOCAL "check thread local keyword" ON)
if (WITH_THREADLOCAL AND NOT DEFINED THREADLOCAL)
	foreach (_THREADLOCAL_KEY "__thread" "__declspec(thread)")
		unset (HAVE_THREAD_LOCAL_STORAGE CACHE)
		check_c_source_compiles("
		${_THREADLOCAL_KEY} int tls;
		
		int main(void) {
			return 0;
		}" HAVE_THREAD_LOCAL_STORAGE)
		if (HAVE_THREAD_LOCAL_STORAGE)
			set (THREADLOCAL ${_THREADLOCAL_KEY} CACHE STRING "Thread local keyword")
		endif ()
	endforeach()
endif ()


if (NLOPT_CXX OR NLOPT_PYTHON OR NLOPT_GUILE OR NLOPT_OCTAVE)
	check_cxx_symbol_exists (__cplusplus ciso646 SYSTEM_HAS_CXX)
	if (SYSTEM_HAS_CXX)
		check_cxx_compiler_flag ("-std=c++11" SUPPORTS_STDCXX11)
		if (SUPPORTS_STDCXX11)
			set (CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
			if (NLOPT_CXX)
				set (NLOPT_CXX11 ON)
			endif ()
		endif ()
	else()
		message (FATAL_ERROR "The compiler doesn't support CXX.")
	endif ()
endif ()

#==============================================================================
# CREATE nlopt_config.h
#==============================================================================
configure_file (${CMAKE_CURRENT_SOURCE_DIR}/nlopt_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/nlopt_config.h IMMEDIATE)

# pkgconfig file
if (UNIX OR MINGW)
	configure_file (${CMAKE_CURRENT_SOURCE_DIR}/nlopt.pc.in ${CMAKE_CURRENT_BINARY_DIR}/nlopt.pc @ONLY)
endif ()

#==============================================================================
# nlopt LIBRARY TARGET (SHARED OR STATIC)
#==============================================================================
set (NLOPT_HEADERS src/api/nlopt.h ${CMAKE_CURRENT_BINARY_DIR}/nlopt.hpp)

add_custom_command (OUTPUT nlopt.hpp
  COMMAND ${CMAKE_COMMAND} -DAPI_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/src/api -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/generate-cpp.cmake
  MAIN_DEPENDENCY src/api/nlopt-in.hpp)
add_custom_target (generate-cpp ALL DEPENDS nlopt.hpp)
#
#add_custom_command (OUTPUT nlopt.f
#  COMMAND ${CMAKE_COMMAND} -DAPI_SOURCE_DIR=${PROJECT_SOURCE_DIR}/src/api -P ${PROJECT_SOURCE_DIR}/cmake/generate-fortran.cmake
#  MAIN_DEPENDENCY src/api/nlopt.h)
#add_custom_target (generate-fortran ALL DEPENDS nlopt.f)

set (NLOPT_SOURCES
	src/algs/direct/DIRect.c src/algs/direct/direct_wrap.c src/algs/direct/DIRserial.c src/algs/direct/DIRsubrout.c src/algs/direct/direct-internal.h src/algs/direct/direct.h
	src/algs/cdirect/cdirect.c src/algs/cdirect/hybrid.c src/algs/cdirect/cdirect.h
	src/algs/praxis/praxis.c src/algs/praxis/praxis.h
	src/algs/luksan/plis.c src/algs/luksan/plip.c src/algs/luksan/pnet.c src/algs/luksan/mssubs.c src/algs/luksan/pssubs.c src/algs/luksan/luksan.h
	src/algs/crs/crs.c src/algs/crs/crs.h
	src/algs/mlsl/mlsl.c src/algs/mlsl/mlsl.h
	src/algs/mma/mma.c src/algs/mma/mma.h src/algs/mma/ccsa_quadratic.c
	src/algs/cobyla/cobyla.c src/algs/cobyla/cobyla.h
	src/algs/newuoa/newuoa.c src/algs/newuoa/newuoa.h
	src/algs/neldermead/nldrmd.c src/algs/neldermead/neldermead.h src/algs/neldermead/sbplx.c
	src/algs/auglag/auglag.c src/algs/auglag/auglag.h
	src/algs/bobyqa/bobyqa.c src/algs/bobyqa/bobyqa.h
	src/algs/isres/isres.c src/algs/isres/isres.h
	src/algs/slsqp/slsqp.c src/algs/slsqp/slsqp.h
	src/algs/esch/esch.c src/algs/esch/esch.h
	src/api/general.c src/api/options.c src/api/optimize.c src/api/deprecated.c src/api/nlopt-internal.h src/api/nlopt.h src/api/f77api.c src/api/f77funcs.h src/api/f77funcs_.h ${CMAKE_CURRENT_BINARY_DIR}/nlopt.hpp
	src/util/mt19937ar.c src/util/sobolseq.c src/util/soboldata.h src/util/timer.c src/util/stop.c src/util/nlopt-util.h src/util/redblack.c src/util/redblack.h src/util/qsort_r.c src/util/rescale.c
)

if (NLOPT_CXX)
	list (APPEND NLOPT_SOURCES src/algs/stogo/global.cc src/algs/stogo/linalg.cc src/algs/stogo/local.cc src/algs/stogo/stogo.cc src/algs/stogo/tools.cc
		src/algs/stogo/global.h src/algs/stogo/linalg.h src/algs/stogo/local.h src/algs/stogo/stogo_config.h src/algs/stogo/stogo.h src/algs/stogo/tools.h)
endif ()
if (NLOPT_CXX11)
	list (APPEND NLOPT_SOURCES src/algs/ags/data_types.hpp src/algs/ags/evolvent.hpp src/algs/ags/evolvent.cc src/algs/ags/solver.hpp src/algs/ags/solver.cc
		src/algs/ags/local_optimizer.hpp src/algs/ags/local_optimizer.cc src/algs/ags/ags.h src/algs/ags/ags.cc)
endif ()

set (nlopt_lib nlopt)
           
if(BUILD_NLOPT_STATIC)
    __add_real_target(${nlopt_lib} lib SOURCE ${NLOPT_SOURCES} LIB ${M_LIBRARY} DEF NLOPT_DLL NLOPT_DLL_EXPORT BOOST_ALL_NO_LIB BOOST_ALL_DYN_LINK
                                   INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src/api ${CMAKE_CURRENT_BINARY_DIR})
else()
    __add_real_target(${nlopt_lib} dll SOURCE ${NLOPT_SOURCES} LIB ${M_LIBRARY} DEF NLOPT_DLL NLOPT_DLL_EXPORT BOOST_ALL_NO_LIB BOOST_ALL_DYN_LINK
                                   INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src/api ${CMAKE_CURRENT_BINARY_DIR})
endif()

add_dependencies(${nlopt_lib} generate-cpp)
#target_link_libraries (${nlopt_lib} ${M_LIBRARY})

if(INSTALL_NLOPT)
	INSTALL(TARGETS nlopt
			LIBRARY DESTINATION lib
			ARCHIVE DESTINATION .
			RUNTIME DESTINATION bin)
endif()
	
#==============================================================================
# INCLUDE DIRECTORIES
#==============================================================================
target_include_directories (${nlopt_lib} PRIVATE
  ${CMAKE_CURRENT_BINARY_DIR}/src/api
  ${CMAKE_CURRENT_BINARY_DIR}
  src/algs/stogo
  src/algs/ags
  src/util
  src/algs/direct
  src/algs/cdirect
  src/algs/praxis
  src/algs/luksan
  src/algs/crs
  src/algs/mlsl
  src/algs/mma
  src/algs/cobyla
  src/algs/newuoa
  src/algs/neldermead
  src/algs/auglag
  src/algs/bobyqa
  src/algs/isres
  src/algs/slsqp
  src/algs/esch
  src/api)
  
add_subdirectory (src/api EXCLUDE_FROM_ALL)

if(CC_INSTALL_NLOPT)
	__install_directory_specif(${CMAKE_CURRENT_SOURCE_DIR}/src/api/ nlopt)
	INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/nlopt.hpp DESTINATION include/nlopt/)
endif()

#target OpenMP::OpenMP_CXX

macro(__enable_openmp)
	if(CC_BC_WIN)
		find_package(OpenMP REQUIRED)
		if(OPENMP_FOUND)
			message("__enable_opemmp Find OpenMP.")
			if(ANDROID)
				message(WARNING "OpenMP_CXX_LIBRARIES:  ${OpenMP_CXX_LIBRARIES}")
				message(WARNING "OpenMP_CXX_FLAGS:  ${OpenMP_CXX_FLAGS}")
			endif()
			if(TARGET OpenMP::OpenMP_CXX)
				message(STATUS "OpenMP TARGET OpenMP::OpenMP_CXX Imported.")
			endif()
		endif()
	elseif(CC_BC_MAC)
				message("__enable_opemmp OPENMP_ROOT ${OPENMP_ROOT}.")
				set(openmp_LIBRARIES_DEBUG ${OPENMP_ROOT}/lib/libomp.a)
				set(openmp_LIBRARIES_RELEASE ${OPENMP_ROOT}/lib/libomp.a)
				
				set(openmp_LOC_DEBUG ${openmp_LIBRARIES_DEBUG})
				set(openmp_LOC_RELEASE ${openmp_LIBRARIES_RELEASE})
			
				set(openmp_INCLUDE_DIRS ${OPENMP_ROOT}/include/)
	elseif(CC_BC_LINUX)
		set(CMAKE_CXX_FLAGS -fopenmp)	
	endif()
endmacro()

macro(__use_openmp target)
	
endmacro()

#macro(__add_openmp_lib arg1)
#    find_library(OPENOMP_LIBRARIES
#             NAMES omp
#             PATHS "/usr/local/lib")
#    message(STATUS ${OPENOMP_LIBRARIES})
#	if(OPENOMP_LIBRARIES)
#		target_link_libraries(${arg1} PRIVATE ${OPENOMP_LIBRARIES})
#	endif()
#endmacro()

#macro(__disable_openmp)
#	string(REPLACE "-openmp" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
#	string(REPLACE "-openmp" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#
#	include(AdjustToolFlags)
#	AdjustToolFlags(CMAKE_C_FLAGS REMOVALS "/-openmp")
#	AdjustToolFlags(CMAKE_CXX_FLAGS REMOVALS "/-openmp")
#	AdjustToolFlags(CMAKE_EXE_LINKER_FLAGS REMOVALS "${OpenMP_EXE_LINKER_FLAGS}")
#endmacro()


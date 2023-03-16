cmake_minimum_required(VERSION 3.8.0)

project(AnkerEngine)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")
include(ConfigureAllModules)  

if(APPLE)
    set(OPENMP_ROOT "/opt/homebrew/Cellar/libomp/14.0.6/")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14")
    if (M1)
        set(CMAKE_OSX_ARCHITECTURES "x86_64")
		set(OPENMP_ROOT "/Users/anker/mac_x64_86/libomp/14.0.0/")
    endif()
endif()

#__enable_vld()
__enable_openmp()

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(libs/clipper)
set(RAPIDJSON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/libs/rapidjson)
set(EIGEN_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/libs/eigen)
set(STB_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/libs/stb)
set(BOOST_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/libs/boost)
set(BOOST_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/libs/boost)

add_subdirectory(src)


set(RAPIDJSON_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/libs/rapidjson)

#add_definitions(-DBOOST_ALL_NO_LIB)
#add_definitions(-DBOOST_FILESYSTEM_SOURCE)
#add_subdirectory(libs/boost/libs/nowide)
#add_subdirectory(libs/boost/libs/file_system)
#add_subdirectory(libs/boost/libs/regex)
#add_subdirectory(libs/boost/libs/thread)
#add_subdirectory(libs/boost/libs/system)
#
#add_subdirectory(libs/tbb)
#add_subdirectory(libs/clipperz)
#add_subdirectory(libs/nlopt)
#add_subdirectory(libs/qhull)
#add_subdirectory(libs/libnest2d)
#add_subdirectory(libs/libslic3r)

#__set_folder_targets(slice3r TARGET libnest2d nlopt libslic3r slic3rapi clipper3r generate-cpp qhullcpp qhullstatic_r tbb tbb_def_files)
#__set_folder_targets(boost TARGET boost_nowide boost_file_system boost_regex boost_thread boost_system)

set(LIBS _CuraEngine)
set(INCS ${CMAKE_CURRENT_SOURCE_DIR}/src)


__add_real_target(CuraEngine exe SOURCE main.cpp LIB ${LIBS} INC ${INCS} OPENMP)


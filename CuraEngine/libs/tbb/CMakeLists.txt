include(CheckCXXCompilerFlag)
include(CheckCXXSourceRuns)

if(POLICY CMP0058)
  cmake_policy(SET CMP0058 NEW)
endif()

if(POLICY CMP0068)
  cmake_policy(SET CMP0068 NEW)
endif()

if (POLICY CMP0078)
  # swig standard target names
  cmake_policy(SET CMP0078 NEW)
endif ()

if (POLICY CMP0086)
  # UseSWIG honors SWIG_MODULE_NAME via -module flag
  cmake_policy(SET CMP0086 NEW)
endif ()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

if(NOT TBB_INSTALL_RUNTIME_DIR)
  set(TBB_INSTALL_RUNTIME_DIR bin)
endif()
if(NOT TBB_INSTALL_LIBRARY_DIR)
  set(TBB_INSTALL_LIBRARY_DIR lib)
endif()
if(NOT TBB_INSTALL_ARCHIVE_DIR)
  set(TBB_INSTALL_ARCHIVE_DIR lib)
endif()
if(NOT TBB_INSTALL_INCLUDE_DIR)
  set(TBB_INSTALL_INCLUDE_DIR include)
endif()
if(NOT TBB_CMAKE_PACKAGE_INSTALL_DIR)
  set(TBB_CMAKE_PACKAGE_INSTALL_DIR lib/cmake/tbb)
endif()

include_directories(include src src/rml/include ${CMAKE_CURRENT_BINARY_DIR})

option(TBB_BUILD_SHARED          "Build TBB shared library" ON)
option(TBB_BUILD_STATIC          "Build TBB static library" ON)
option(TBB_BUILD_TBBMALLOC       "Build TBB malloc library" ON)
option(TBB_BUILD_TBBMALLOC_PROXY "Build TBB malloc proxy library" ON)
option(TBB_BUILD_TESTS           "Build TBB tests and enable testing infrastructure" ON)
option(TBB_NO_DATE               "Do not save the configure date in the version string" OFF)
option(TBB_BUILD_PYTHON          "Build TBB Python bindings" OFF)
option(TBB_SET_SOVERSION         "Set the SOVERSION (shared library build version suffix)?" OFF)

# When this repository is part of a larger build system of a parent project
# we may not want TBB to set up default installation targets
option(TBB_INSTALL_TARGETS       "Include build targets for 'make install'" OFF)

if(APPLE)
  set(CMAKE_MACOSX_RPATH ON)
endif()

file(GLOB tbb_src "${CMAKE_CURRENT_SOURCE_DIR}/src/tbb/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/old/*.cpp")
list(REMOVE_ITEM tbb_src ${CMAKE_CURRENT_SOURCE_DIR}/src/tbb/tbb_bind.cpp)
list(APPEND tbb_src ${CMAKE_CURRENT_SOURCE_DIR}/src/rml/client/rml_tbb.cpp)
file(GLOB to_remove "${CMAKE_CURRENT_SOURCE_DIR}/src/old/test*.cpp")
list(REMOVE_ITEM tbb_src ${to_remove})

set(tbbmalloc_static_src
  src/tbbmalloc/backend.cpp
  src/tbbmalloc/large_objects.cpp
  src/tbbmalloc/backref.cpp
  src/tbbmalloc/tbbmalloc.cpp
  src/tbbmalloc/frontend.cpp
  src/tbb/itt_notify.cpp)

set(tbbmalloc_src ${tbbmalloc_static_src})

set(tbbmalloc_proxy_src
  src/tbbmalloc/proxy.cpp
  src/tbbmalloc/tbb_function_replacement.cpp)

add_library (tbb_interface INTERFACE)
add_definitions(-DTBB_SUPPRESS_DEPRECATED_MESSAGES=1)

if (CMAKE_SYSTEM_PROCESSOR MATCHES "(i386|x86_64)")
  if (NOT APPLE AND NOT MINGW)
    target_compile_definitions(tbb_interface INTERFACE DO_ITT_NOTIFY)
  endif()
endif()

if (APPLE)
  # Disable annoying "has no symbols" warnings
  set(CMAKE_C_ARCHIVE_CREATE   "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
  set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
  set(CMAKE_C_ARCHIVE_FINISH   "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
  set(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
endif()

macro(CHECK_CXX_COMPILER_AND_LINKER_FLAGS _RESULT _CXX_FLAGS _LINKER_FLAGS)
  set(CMAKE_REQUIRED_FLAGS ${_CXX_FLAGS})
  set(CMAKE_REQUIRED_LIBRARIES ${_LINKER_FLAGS})
  set(CMAKE_REQUIRED_QUIET TRUE)
  check_cxx_source_runs("#include <iostream>\nint main(int argc, char **argv) { std::cout << \"test\"; return 0; }" ${_RESULT})
  set(CMAKE_REQUIRED_FLAGS "")
  set(CMAKE_REQUIRED_LIBRARIES "")
endmacro()

# Prefer libc++ in conjunction with Clang
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  if (CMAKE_CXX_FLAGS MATCHES "-stdlib=libc\\+\\+")
    message(STATUS "TBB: using libc++.")
  else()
    CHECK_CXX_COMPILER_AND_LINKER_FLAGS(HAS_LIBCPP "-stdlib=libc++" "-stdlib=libc++")
    if (HAS_LIBCPP)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -D_LIBCPP_VERSION")
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
      set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++")
      message(STATUS "TBB: using libc++.")
    else()
      message(STATUS "TBB: NOT using libc++.")
    endif()
  endif()
endif()

set (CMAKE_CXX_STANDARD 11)

if (UNIX)
  target_compile_definitions(tbb_interface INTERFACE USE_PTHREAD)

  check_cxx_compiler_flag ("-mrtm -Werror" SUPPORTS_MRTM)
  if (SUPPORTS_MRTM)
    target_compile_options(tbb_interface INTERFACE "-mrtm")
  endif ()

elseif(WIN32)
  target_compile_definitions(tbb_interface INTERFACE USE_WINTHREAD _WIN32_WINNT=0x0600)
  if (MSVC)
    enable_language(ASM_MASM)
    target_compile_options(tbb_interface INTERFACE /GS- /Zc:wchar_t /Zc:forScope)
    check_cxx_compiler_flag ("/volatile:iso" SUPPORTS_VOLATILE_FLAG)
    if (SUPPORTS_VOLATILE_FLAG)
      target_compile_options(tbb_interface INTERFACE /volatile:iso)
    endif ()
    target_compile_options(tbb_interface INTERFACE $<$<COMPILE_LANGUAGE:CXX>:/wd4267 /wd4800 /wd4146 /wd4244 /wd4577 /wd4018>)
    if (NOT CMAKE_SIZEOF_VOID_P)
       message(FATAL_ERROR "'CMAKE_SIZEOF_VOID_P' is undefined. Please delete your build directory and rerun CMake again!")
    endif()

    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
      list(APPEND tbb_src src/tbb/intel64-masm/atomic_support.asm
        src/tbb/intel64-masm/itsx.asm src/tbb/intel64-masm/intel64_misc.asm)
      set_source_files_properties(
        src/tbb/intel64-masm/atomic_support.asm
        src/tbb/intel64-masm/itsx.asm src/tbb/intel64-masm/intel64_misc.asm
        src/tbb/intel64-masm/atomic_support.asm
        PROPERTIES LANGUAGE ASM_MASM)
      list(APPEND tbbmalloc_src src/tbb/intel64-masm/atomic_support.asm)
      set(CMAKE_ASM_MASM_FLAGS "/DEM64T=1 ${CMAKE_ASM_MASM_FLAGS}")
    else()
      list(APPEND tbb_src src/tbb/ia32-masm/atomic_support.asm
        src/tbb/ia32-masm/itsx.asm src/tbb/ia32-masm/lock_byte.asm)
      set_source_files_properties(
        src/tbb/ia32-masm/atomic_support.asm
        src/tbb/ia32-masm/itsx.asm src/tbb/ia32-masm/lock_byte.asm
        PROPERTIES LANGUAGE ASM_MASM)
      # Enable SAFESEH feature for assembly (x86 builds only).
      set(CMAKE_ASM_MASM_FLAGS "/safeseh ${CMAKE_ASM_MASM_FLAGS}")
    endif()
  elseif (MINGW)
    target_compile_options(tbb_interface INTERFACE "-mthreads")
  endif ()
endif()

if (MSVC)
  set(ENABLE_RTTI "/EHsc /GR ")
  set(DISABLE_RTTI "/EHs- /GR- ")
elseif (UNIX)
  set(ENABLE_RTTI "-frtti -fexceptions ")
  set(DISABLE_RTTI "-fno-rtti -fno-exceptions ")
endif ()

##--------
#   - Added TBB_USE_GLIBCXX_VERSION macro to specify the version of GNU
#     libstdc++ when it cannot be properly recognized, e.g. when used
#     with Clang on Linux* OS. Inspired by a contribution from David A.
if (NOT TBB_USE_GLIBCXX_VERSION AND UNIX AND NOT APPLE)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # using Clang
    string(REPLACE "." "0" TBB_USE_GLIBCXX_VERSION ${CMAKE_CXX_COMPILER_VERSION})
  endif()
endif()

if (TBB_USE_GLIBCXX_VERSION)
  target_compile_definitions(tbb_interface INTERFACE TBB_USE_GLIBCXX_VERSION=${TBB_USE_GLIBCXX_VERSION})
endif()

##-------

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
   check_cxx_compiler_flag ("-flifetime-dse=1" SUPPORTS_FLIFETIME)
   if (SUPPORTS_FLIFETIME)
     target_compile_options(tbb_interface INTERFACE -flifetime-dse=1)
   endif()
endif()

# Linker export definitions
if (APPLE)
  set (ARCH_PREFIX "mac")
elseif(WIN32)
  set (ARCH_PREFIX "win")
else()
  set (ARCH_PREFIX "lin")
endif()

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(ARCH_PREFIX "${ARCH_PREFIX}64")
else()
  set(ARCH_PREFIX "${ARCH_PREFIX}32")
endif()

if (MINGW)
  set (ARCH_PREFIX "${ARCH_PREFIX}-gcc")
  # there's no win32-gcc-tbb-export.def, use lin32-tbb-export.def
  execute_process (COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/src/tbb/lin32-tbb-export.def ${CMAKE_CURRENT_SOURCE_DIR}/src/tbb/win32-gcc-tbb-export.def)
endif()

if (MSVC)
  add_custom_command(OUTPUT tbb.def
    COMMAND ${CMAKE_CXX_COMPILER} /TC /EP ${CMAKE_CURRENT_SOURCE_DIR}/src/tbb/${ARCH_PREFIX}-tbb-export.def  -I ${CMAKE_CURRENT_SOURCE_DIR}/include > tbb.def
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/src/tbb/${ARCH_PREFIX}-tbb-export.def
    COMMENT "Preprocessing tbb.def"
  )

  add_custom_command(OUTPUT tbbmalloc.def
    COMMAND ${CMAKE_CXX_COMPILER} /TC /EP ${CMAKE_CURRENT_SOURCE_DIR}/src/tbbmalloc/${ARCH_PREFIX}-tbbmalloc-export.def  -I ${CMAKE_CURRENT_SOURCE_DIR}/include >   tbbmalloc.def
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/src/tbbmalloc/${ARCH_PREFIX}-tbbmalloc-export.def
    COMMENT "Preprocessing tbbmalloc.def"
  )
else()
  add_custom_command(OUTPUT tbb.def
    COMMAND ${CMAKE_CXX_COMPILER} -xc++ -std=c++11 -E ${CMAKE_CURRENT_SOURCE_DIR}/src/tbb/${ARCH_PREFIX}-tbb-export.def  -I ${CMAKE_CURRENT_SOURCE_DIR}/include -o tbb.def
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/src/tbb/${ARCH_PREFIX}-tbb-export.def
    COMMENT "Preprocessing tbb.def"
  )

  add_custom_command(OUTPUT tbbmalloc.def
    COMMAND ${CMAKE_CXX_COMPILER} -xc++ -std=c++11 -E ${CMAKE_CURRENT_SOURCE_DIR}/src/tbbmalloc/${ARCH_PREFIX}-tbbmalloc-export.def  -I ${CMAKE_CURRENT_SOURCE_DIR}/include -o   tbbmalloc.def
    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/src/tbbmalloc/${ARCH_PREFIX}-tbbmalloc-export.def
    COMMENT "Preprocessing tbbmalloc.def"
  )
endif()

add_custom_target(tbb_def_files DEPENDS tbb.def tbbmalloc.def)


if (TBB_BUILD_SHARED)
  #add_library(tbb SHARED ${tbb_src})
  __add_real_target(tbb dll SOURCE ${tbb_src} LIB tbb_interface)

  #target_link_libraries(tbb PRIVATE tbb_interface)
  target_include_directories(tbb INTERFACE "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>" "$<INSTALL_INTERFACE:${TBB_INSTALL_INCLUDE_DIR}>")
  set_property(TARGET tbb APPEND_STRING PROPERTY COMPILE_FLAGS ${ENABLE_RTTI})
  if (TBB_SET_SOVERSION)
    set_property(TARGET tbb PROPERTY SOVERSION 2)
  endif ()

  target_compile_definitions(tbb
    PRIVATE -D__TBB_BUILD=1)

  if (MSVC)
    target_compile_definitions(tbb
      PUBLIC -D__TBB_NO_IMPLICIT_LINKAGE=1
      PRIVATE -D_CRT_SECURE_NO_WARNINGS)
  endif()

  add_dependencies(tbb tbb_def_files)

  if (APPLE)
    set_property(TARGET tbb APPEND PROPERTY LINK_FLAGS "-Wl,-exported_symbols_list,\"${CMAKE_CURRENT_BINARY_DIR}/tbb.def\"")
  elseif (MSVC)
    set_property(TARGET tbb APPEND PROPERTY LINK_FLAGS "/DEF:\"${CMAKE_CURRENT_BINARY_DIR}/tbb.def\"")
  else ()
    set_property(TARGET tbb APPEND PROPERTY LINK_FLAGS "-Wl,-version-script,\"${CMAKE_CURRENT_BINARY_DIR}/tbb.def\"")
  endif()

  if (TBB_INSTALL_TARGETS)
    install(TARGETS tbb EXPORT TBB
            LIBRARY DESTINATION ${TBB_INSTALL_LIBRARY_DIR}
            ARCHIVE DESTINATION ${TBB_INSTALL_ARCHIVE_DIR}
            RUNTIME DESTINATION ${TBB_INSTALL_RUNTIME_DIR})
    if (MSVC)
      install(FILES $<TARGET_PDB_FILE:tbb> DESTINATION ${TBB_INSTALL_RUNTIME_DIR} OPTIONAL)
    endif()
  endif()

  if (UNIX AND NOT APPLE)
    target_link_libraries(tbb PUBLIC pthread dl)
  endif()
endif()


if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # Quench a warning on GCC
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/tbb/governor.cpp COMPILE_FLAGS "-Wno-missing-field-initializers ")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  # Quench a warning on Clang
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/tbb/itt_notify.cpp COMPILE_FLAGS "-Wno-varargs ")
elseif(MSVC)
  # Quench a warning on MSVC
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/src/tbb/scheduler.cpp COMPILE_FLAGS "/wd4458 ")
endif()



if (TBB_INSTALL_TARGETS)
  install(DIRECTORY include/tbb DESTINATION ${TBB_INSTALL_INCLUDE_DIR})
  if (TBB_BUILD_SHARED)
    install(EXPORT TBB DESTINATION ${TBB_CMAKE_PACKAGE_INSTALL_DIR} NAMESPACE TBB:: FILE TBBConfig.cmake)
  endif()
endif()

# version file
if (TBB_INSTALL_TARGETS)
  set (_VERSION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/include/tbb/tbb_stddef.h)
  file (STRINGS ${_VERSION_FILE} _VERSION_MAJOR_STRING REGEX ".*define[ ]+TBB_VERSION_MAJOR[ ]+[0-9]+.*")
  file (STRINGS ${_VERSION_FILE} _VERSION_MINOR_STRING REGEX ".*define[ ]+TBB_VERSION_MINOR[ ]+[0-9]+.*")
  string (REGEX REPLACE ".*TBB_VERSION_MAJOR[ ]+([0-9]+)" "\\1" TBB_MAJOR_VERSION ${_VERSION_MAJOR_STRING})
  string (REGEX REPLACE ".*TBB_VERSION_MINOR[ ]+([0-9]+)" "\\1" TBB_MINOR_VERSION ${_VERSION_MINOR_STRING})
  set (TBB_VERSION_STRING "${TBB_MAJOR_VERSION}.${TBB_MINOR_VERSION}")
  include (CMakePackageConfigHelpers)
  write_basic_package_version_file (TBBConfigVersion.cmake VERSION "${TBB_VERSION_STRING}" COMPATIBILITY AnyNewerVersion)
  install (FILES ${CMAKE_CURRENT_BINARY_DIR}/TBBConfigVersion.cmake DESTINATION "${TBB_CMAKE_PACKAGE_INSTALL_DIR}")
endif()

# version_string.ver
if (UNIX AND NOT TBB_NO_DATE)
  execute_process (COMMAND date "+%a, %d %b %Y %H:%M:%S %z"
                   OUTPUT_VARIABLE _configure_date
                   OUTPUT_STRIP_TRAILING_WHITESPACE)
elseif (WIN32 AND NOT TBB_NO_DATE)
  execute_process (COMMAND cmd " /C date /T"
                   OUTPUT_VARIABLE _configure_date
                   OUTPUT_STRIP_TRAILING_WHITESPACE)
else ()
  set (_configure_date "Unknown")
endif()
set (TBB_CONFIG_DATE "${_configure_date}" CACHE STRING "First time that TBB was configured")
set (_configure_date "${TBB_CONFIG_DATE}")
include_directories (${CMAKE_BINARY_DIR})
configure_file (build/version_string.ver.in version_string.ver @ONLY)



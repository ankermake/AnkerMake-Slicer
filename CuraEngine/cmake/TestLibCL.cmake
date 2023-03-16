
macro(__test_lib_cl)

if(TEST_CL AND WIN32)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        if (CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(Dest_Platform "msvc_x64")
        else()
            set(Dest_Platform "msvc_x32")
        endif()
    endif()
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        if (CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(Dest_Platform "gcc_x64")
        else()
            set(Dest_Platform "gcc_x32")
        endif()
    endif()

    message(STATUS "\n\n\n\n========TEST_BY_CL========\n")
    message(STATUS "PROJECT_NAME               : " ${PROJECT_NAME})
    message(STATUS "CMAKE_COMMAND              : " ${CMAKE_COMMAND})
    message(STATUS "CMAKE_BUILD_TYPE           : " ${CMAKE_BUILD_TYPE})

    set(Dest_AnkerSlicerRoot    ${CMAKE_CURRENT_BINARY_DIR}/../../AnkerSlicer/Local_Feature)
    set(Dest_EngineBinDir_In3rd ${Dest_AnkerSlicerRoot}/3rd/SliceEngine/SliceEngine_${Dest_Platform})
    set(Dest_FdmPlugin_Lib      ${Dest_AnkerSlicerRoot}/src/ankerplugins/fdm_slicer/lib)

    message(STATUS "CMAKE_CURRENT_BINARY_DIR   : " ${CMAKE_CURRENT_BINARY_DIR})
    message(STATUS "Dest_AnkerSlicerRoot       : " ${Dest_AnkerSlicerRoot})
    message(STATUS "Dest_FdmPlugin_Lib         : " ${Dest_FdmPlugin_Lib})
    message(STATUS "Dest_EngineBinDir_In3rd    : " ${Dest_EngineBinDir_In3rd})


    set(File_App_h ${CMAKE_SOURCE_DIR}/src/Application.h)
    set(File_engine_lib ${CMAKE_CURRENT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
    set(File_engine_exe ${CMAKE_CURRENT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/CuraEngine.exe)

    message(STATUS "File_App_h:      " ${File_App_h})
    message(STATUS "File_engine_lib: " ${File_engine_lib})
    message(STATUS "File_engine_exe: " ${File_engine_exe})

    add_custom_command( TARGET CuraEngine  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${File_App_h}      ${Dest_FdmPlugin_Lib}
        COMMAND ${CMAKE_COMMAND} -E copy ${File_engine_exe} ${Dest_EngineBinDir_In3rd}
        COMMAND ${CMAKE_COMMAND} -E copy ${File_engine_exe} "C:/Users/Administrator/AppData/Local/AnkerMake/AnkerMake_64bit_fp/SliceEngine"
    )

    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_custom_command( TARGET CuraEngine  POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${File_engine_lib}/_CuraEngine.lib    ${Dest_FdmPlugin_Lib}
            COMMAND ${CMAKE_COMMAND} -E copy ${File_engine_lib}/clipper.lib        ${Dest_FdmPlugin_Lib}
        )
    endif()
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        add_custom_command( TARGET CuraEngine  POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${File_engine_lib}/lib_CuraEngine.a    ${Dest_FdmPlugin_Lib}
            COMMAND ${CMAKE_COMMAND} -E copy ${File_engine_lib}/libclipper.a        ${Dest_FdmPlugin_Lib}
        )
    endif()

endif ()
endmacro()

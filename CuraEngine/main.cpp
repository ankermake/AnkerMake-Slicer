//Copyright (c) 2018 Ultimaker B.V.
//CuraEngine is released under the terms of the AGPLv3 or higher.

#include <iostream> //To change the formatting of std::cerr.
#include <signal.h> //For floating point exceptions.
#if defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
    #include <sys/resource.h> //For setpriority.
#endif
#include "Application.h"
#include "utils/logoutput.h"
#include "utils/string.h" 
#include <fstream> 
#include <cstring>
#include <vector>
#include <locale>         // std::wstring_convert
#include <codecvt>        // std::codecvt_utf8
#include <stdlib.h>

#include "utils/DecodeUtils.hpp"

namespace cura
{

//Signal handler for a "floating point exception", which can also be integer division by zero errors.
void signal_FPE(int n)
{
    (void)n;
    logError("Arithmetic exception.\n");
    exit(1);
}

}//namespace cura
//


int main(int argc, char **argv)
{
#if defined(__linux__) || (defined(__APPLE__) && defined(__MACH__))
    //Lower the process priority on linux and mac. On windows this is done on process creation from the GUI.
    setpriority(PRIO_PROCESS, 0, 10);
#endif

#ifndef DEBUG
    //Register the exception handling for arithmetic exceptions, this prevents the "something went wrong" dialog on windows to pop up on a division by zero.
    signal(SIGFPE, cura::signal_FPE);
#endif
    std::cerr << std::boolalpha;
    //char xx[4][40] = { "engin" ,"extParam","-f","C:/workspace/task/slice3.bat"};

    //cura::logToTempFileInit();
    cura::Application::getInstance().run(argc, argv);
  
    //cura::logToTempFileEnd();
    return 0;
}

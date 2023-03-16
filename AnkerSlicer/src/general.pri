# this is the common include for anything compiled inside the meshlab pro
# contains general preprocesser, compiler and linker settings,
# paths for dependencies and so on

######## GENERAL SETTINGS ##########

# This is the main coord type inside meshlab
# it can be double or float according to user needs.
DEFINES += MESHLAB_SCALAR=float

# defining meshlab version
# defining meshlab version
#exists($$PWD/../ML_VERSION){
#	MESHLAB_VERSION = $$cat($$PWD/../ML_VERSION)
#	DEFINES += "MESHLAB_VERSION=$$MESHLAB_VERSION"
#}

MESHLAB_VERSION = V0.3.x_2022a
DEFINES += "MESHLAB_VERSION=$$MESHLAB_VERSION"
#macx:CONFIG += MAC_M1_BUILD_X86
#macx:DEFINES += FORCE_ARM

win32-msvc: QMAKE_CXXFLAGS += /wd4819 /wd4100 /wd4267 /wd4828
win32-g++{
CONFIG += warn_off
QMAKE_CXXFLAGS += -Wall
QMAKE_CXXFLAGS += -Wno-comment
}
# VCG directory
VCGDIR = $$MESHLAB_SOURCE_DIRECTORY/vcglib

include(find_system_libs.pri)

# MeshLab requires C++11  ->  14
CONFIG += c++14

#Debug and Release configs
!win32-msvc{
    CONFIG(release, debug|release):QMAKE_CXXFLAGS += -O3 -DNDEBUG
    CONFIG(debug, debug|release):QMAKE_CXXFLAGS += -O0 -g
}
#Eigen and glew dirs
CONFIG(system_eigen3): EIGENDIR = /usr/include/eigen3
!CONFIG(system_eigen3):EIGENDIR = $$VCGDIR/eigenlib
!CONFIG(system_glew):  GLEWDIR = $$MESHLAB_EXTERNAL_DIRECTORY/glew-2.1.0

## Warnings Settings ##
#warning on unused result from fscanf...
!win32-msvc {
    QMAKE_CXXFLAGS+=-Wno-unused-result
    QMAKE_CFLAGS+=-Wno-unused-result
}

######## WINDOWS SETTINGS ##########

# Flags for OpenMP
win32-msvc:QMAKE_CXXFLAGS+=/openmp #Just for cl, flag is /openmp instead of -fopenmp

# the following line is needed to avoid mismatch between
# the awful min/max macros of windows and the limits max
win32:DEFINES += NOMINMAX

# Set up library search paths
win32-msvc:QMAKE_LFLAGS+= -L$$MESHLAB_DISTRIB_DIRECTORY/lib/win32-msvc -L$$MESHLAB_DISTRIB_DIRECTORY/lib
win32-gcc:QMAKE_LFLAGS+= -L$$MESHLAB_DISTRIB_DIRECTORY/lib/win32-gcc -L$$MESHLAB_DISTRIB_DIRECTORY/lib


######## MACOS SETTINGS ##########

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.14

macx:QMAKE_CXXFLAGS += -Wno-inconsistent-missing-override


# Flags for OpenMP
macx:QMAKE_CXXFLAGS += -Xpreprocessor -fopenmp
macx:{
#    contains(QMAKE_HOST.arch, arm64){
#    message("---"$$QMAKE_HOST.arch)
#    QMAKE_LFLAGS += -L/opt/homebrew/Cellar/libomp/14.0.6/lib/ -lomp #brew install libomp
#    INCLUDEPATH += /opt/homebrew/Cellar/libomp/14.0.6/include/
#    }else{
#    message("***"$$QMAKE_HOST.arch)
#    #QMAKE_LFLAGS += -L/usr/local/opt/libomp/lib/ -lomp #brew install libomp
#    #INCLUDEPATH += /usr/local/include/
#    #/usr/local/Cellar/libomp/14.0.0/
#    #/opt/homebrew/Cellar/libomp/14.0.6/
#    QMAKE_LFLAGS += -L/usr/local/Cellar/libomp/14.0.0/lib/ -lomp #brew install libomp
#    INCLUDEPATH += /usr/local/Cellar/libomp/14.0.0/include/
#    }

#    QMAKE_LFLAGS += -L/opt/homebrew/Cellar/libomp/14.0.6/lib/ -lomp #brew install libomp
#    INCLUDEPATH += /opt/homebrew/Cellar/libomp/14.0.6/include/
    MAC_M1_BUILD_X86{
        message("build omp, MAC_M1_BUILD_X86 is set")
        LIBS += /Users/anker/mac_x64_86/libomp/14.0.0/lib/libomp.a #brew install libomp
        INCLUDEPATH += /Users/anker/mac_x64_86/libomp/14.0.0/include/
    }else{
        message("build omp, MAC_M1_BUILD_X86 is not set")
        QMAKE_LFLAGS += -L/usr/local/Cellar/libomp/14.0.0/lib/ -lomp #brew install libomp
        INCLUDEPATH += /usr/local/Cellar/libomp/14.0.0/include/
    }
}

#macx:QMAKE_LFLAGS += -L/usr/local/opt/libomp/lib/ -lomp #brew install libomp
#macx:INCLUDEPATH += /usr/local/include/

# Set up library search paths
#macx:QMAKE_LFLAGS+= -L$$MESHLAB_DISTRIB_DIRECTORY/lib/macx64 -L$$MESHLAB_DISTRIB_DIRECTORY/lib
macx:QMAKE_LFLAGS+= -L$$MESHLAB_DISTRIB_DIRECTORY/lib


######## LINUX SETTINGS ##########

# Flags for OpenMP
linux:QMAKE_CXXFLAGS+= -fopenmp

# Set up library search paths
linux:QMAKE_RPATHDIR += $$MESHLAB_DISTRIB_DIRECTORY/lib
linux:QMAKE_LFLAGS+= -L$$MESHLAB_DISTRIB_DIRECTORY/lib/linux -L$$MESHLAB_DISTRIB_DIRECTORY/lib
linux:QMAKE_LFLAGS+= -L$$MESHLAB_DISTRIB_DIRECTORY/lib/linux -L$$MESHLAB_DISTRIB_DIRECTORY/lib

CONFIG(debug  , debug|release){ LIBS += -L$$MESHLAB_DISTRIB_DIRECTORY/../common/debug  }
CONFIG(release, debug|release){ LIBS += -L$$MESHLAB_DISTRIB_DIRECTORY/../common/release}

win32-msvc:LIBS +=  -lopengl32	\
                    -lGLU32
#win32-g++:LIBS += -lmeshlab-common -lopengl32 -lGLU32
win32-g++:LIBS += -lopengl32	\
                    -lGLU32
#DEFINES += USE_EXTRA_UI



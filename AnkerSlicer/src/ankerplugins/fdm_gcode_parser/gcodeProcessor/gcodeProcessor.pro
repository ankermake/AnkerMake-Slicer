include (../../../shared.pri)

TARGET = fdm_gcode_parser

SUBDIRS += gcodeProcessor
gcodeProcessor.subdir = gcodeProcessor
include (../../shared.pri)

TARGET = gcodeProcessor

HEADERS += \
    GCodeReader.hpp \
	GCodeProcessor.hpp
SOURCES += \
    GCodeReader.cpp \
	GCodeProcessor.cpp

CONFIG += c++17





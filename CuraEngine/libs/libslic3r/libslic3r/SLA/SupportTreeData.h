#pragma once
#include <libslic3r/SLA/SupportTreeBuilder.hpp>

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

namespace Slic3r {
    namespace sla {
        using namespace RAPIDJSON_NAMESPACE;

        void writeHeadsData(PrettyWriter<rapidjson::StringBuffer>& writer, SupportTreeBuilder* supportTreePtr);
        void writePillarsData(PrettyWriter<rapidjson::StringBuffer>& writer, SupportTreeBuilder* supportTreePtr);
        void writeJunctionsData(PrettyWriter<rapidjson::StringBuffer>& writer, SupportTreeBuilder* supportTreePtr);
        void writeBridgesData(PrettyWriter<rapidjson::StringBuffer>& writer, SupportTreeBuilder* supportTreePtr);
        void writeCrossBridgesData(PrettyWriter<rapidjson::StringBuffer>& writer, SupportTreeBuilder* supportTreePtr);
        void writeDiffBridgesData(PrettyWriter<rapidjson::StringBuffer>& writer, SupportTreeBuilder* supportTreePtr);
        void writePedestalsData(PrettyWriter<rapidjson::StringBuffer>& writer, SupportTreeBuilder* supportTreePtr);
        void writeAnchorsData(PrettyWriter<rapidjson::StringBuffer>& writer, SupportTreeBuilder* supportTreePtr);
        void  writePadData(PrettyWriter<rapidjson::StringBuffer>& writer, SupportTreeBuilder* supportTreePtr);
        void writeSquarePedestalsData(PrettyWriter<rapidjson::StringBuffer>& writer, SupportTreeBuilder* supportTreePtr);

        //read
        int readHeadsData(std::string filename, const char* data, SupportTreeBuilder* supportTreePtr);
        int readPillarsData(std::string filename, const char* data, SupportTreeBuilder* supportTreePtr);
        int readJunctionsData(std::string filename, const char* data, SupportTreeBuilder* supportTreePtr);
        int readBridgesData(std::string filename, const char* data, SupportTreeBuilder* supportTreePtr);
        int readCrossBridgesData(std::string filename, const char* data, SupportTreeBuilder* supportTreePtr);
        int readDiffBridgesData(std::string filename, const char* data, SupportTreeBuilder* supportTreePtr);
        int readPedestalsData(std::string filename, const char* data, SupportTreeBuilder* supportTreePtr);
        int readAnchorsData(std::string filename, const char* data, SupportTreeBuilder* supportTreePtr);
        int readPadData(std::string filename, const char* data, SupportTreeBuilder* supportTreePtr);
        int readSquarePedestalsData(std::string filename, const char* data, SupportTreeBuilder* supportTreePtr);

    }//end sla
}//end Slic3r

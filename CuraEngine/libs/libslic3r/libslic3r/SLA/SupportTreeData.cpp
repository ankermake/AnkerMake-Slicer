#include <libslic3r/SLA/SupportTreeData.h>
#include <libslic3r/SLA/SupportTreeBuildsteps.hpp>
#include <libslic3r/SLA/SupportTreeMesher.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>
//#include  <direct.h>  
#include  <stdio.h> 

namespace Slic3r {
    namespace sla {
        using namespace RAPIDJSON_NAMESPACE;

void writeHeadsData(PrettyWriter<rapidjson::StringBuffer>& writer, SupportTreeBuilder* supportTreePtr)
{

    //writer.StartObject();
    {
        writer.Key("Heads");
        writer.StartArray();
        for (Head& obj : supportTreePtr->m_heads)
        {
            writer.StartArray();
            writer.StartObject();
            if (obj.is_valid())
            {
                //pos
                {
                    writer.Key("pos");
                    writer.StartArray();
                    writer.Double(obj.pos.x());
                    writer.Double(obj.pos.y());
                    writer.Double(obj.pos.z());
                    writer.EndArray();

                }
                //dir
                {
                    writer.Key("dir");
                    writer.StartArray();
                    writer.Double(obj.dir.x());
                    writer.Double(obj.dir.y());
                    writer.Double(obj.dir.z());
                    writer.EndArray();

                }
                writer.Key("r_back_mm"); writer.Double(obj.r_back_mm);
                writer.Key("r_pin_mm"); writer.Double(obj.r_pin_mm);
                writer.Key("penetration_mm"); writer.Double(obj.penetration_mm);
                writer.Key("width_mm"); writer.Double(obj.width_mm);
                writer.Key("r_toptouch_mm"); writer.Double(obj.r_toptouch_mm);
                writer.Key("toptouch_enable"); writer.Bool(obj.toptouch_enable);
                writer.Key("id"); writer.Int64(obj.id);
                //////////////////////////////
                {
                    writer.Key("linkL");
                    writer.StartArray();

                    for (SupportTreeNode2MeshLink& lk : obj.linkL)
                    {
                        writer.StartArray();
                        writer.Int((int)lk.type);
                        writer.Int64(lk.id);
                        writer.EndArray();
                    }
                    writer.EndArray();
                }
                //////////////////////////////
                {
                    writer.Key("linkR");
                    writer.StartArray();
                    for (SupportTreeNode2MeshLink& lk : obj.linkR)
                    {
                        writer.StartArray();
                        writer.Int((int)lk.type);
                        writer.Int64(lk.id);
                        writer.EndArray();
                    }
                    writer.EndArray();

                }
            }
            writer.EndObject();
            writer.EndArray();
        }
        writer.EndArray();

    }
    //writer.EndObject();


}
//write
void writePillarsData(PrettyWriter<rapidjson::StringBuffer>& writer, SupportTreeBuilder* supportTreePtr)
{

    //writer.StartObject();
    {
        writer.Key("Pillars");
        writer.StartArray();
        for (Pillar& obj : supportTreePtr->m_pillars)
        {
            writer.StartArray();
            writer.StartObject();
            if (obj.is_valid())
            {
                //endpt
                {
                    writer.Key("endpt");
                    writer.StartArray();
                    writer.Double(obj.endpt.x());
                    writer.Double(obj.endpt.y());
                    writer.Double(obj.endpt.z());
                    writer.EndArray();

                }
                writer.Key("height"); writer.Double(obj.height);
                writer.Key("r_mm"); writer.Double(obj.r);
                writer.Key("bridgePillar"); writer.Bool(obj.bridgePillar);
                writer.Key("PillarSelfType"); writer.Int(obj.selfType);
                writer.Key("bridges"); writer.Uint(obj.bridges);
                writer.Key("links"); writer.Uint(obj.links);
                writer.Key("id"); writer.Int64(obj.id);
                //////////////////////////////
                {
                    writer.Key("linkL");
                    writer.StartArray();

                    for (SupportTreeNode2MeshLink& lk : obj.linkL)
                    {
                        writer.StartArray();
                        writer.Int((int)lk.type);
                        writer.Int64(lk.id);
                        writer.EndArray();
                    }
                    writer.EndArray();
                }
                //////////////////////////////
                {
                    writer.Key("linkR");
                    writer.StartArray();
                    for (SupportTreeNode2MeshLink& lk : obj.linkR)
                    {
                        writer.StartArray();
                        writer.Int((int)lk.type);
                        writer.Int64(lk.id);
                        writer.EndArray();
                    }
                    writer.EndArray();

                }
            }
            writer.EndObject();
            writer.EndArray();
        }
        writer.EndArray();

    }
    //writer.EndObject();


}
void writeJunctionsData(PrettyWriter<rapidjson::StringBuffer>& writer, SupportTreeBuilder* supportTreePtr)
{

    //writer.StartObject();
    {
        writer.Key("Junctions");
        writer.StartArray();
        for (Junction& obj : supportTreePtr->m_junctions)
        {
            writer.StartArray();
            writer.StartObject();
            if (obj.is_valid())
            {
                //pos
                {
                    writer.Key("pos");
                    writer.StartArray();
                    writer.Double(obj.pos.x());
                    writer.Double(obj.pos.y());
                    writer.Double(obj.pos.z());
                    writer.EndArray();

                }
                writer.Key("r_mm"); writer.Double(obj.r);
                writer.Key("id"); writer.Int64(obj.id);
                //////////////////////////////
                {
                    writer.Key("linkL");
                    writer.StartArray();

                    for (SupportTreeNode2MeshLink& lk : obj.linkL)
                    {
                        writer.StartArray();
                        writer.Int((int)lk.type);
                        writer.Int64(lk.id);
                        writer.EndArray();
                    }
                    writer.EndArray();
                }
                //////////////////////////////
                {
                    writer.Key("linkR");
                    writer.StartArray();
                    for (SupportTreeNode2MeshLink& lk : obj.linkR)
                    {
                        writer.StartArray();
                        writer.Int((int)lk.type);
                        writer.Int64(lk.id);
                        writer.EndArray();
                    }
                    writer.EndArray();

                }
            }
            writer.EndObject();
            writer.EndArray();
        }
        writer.EndArray();

    }
    //writer.EndObject();

}
void writeBridgesData(PrettyWriter<rapidjson::StringBuffer>& writer, SupportTreeBuilder* supportTreePtr)
{

    //writer.StartObject();
    {
        writer.Key("Bridges");
        writer.StartArray();
        for (Bridge& obj : supportTreePtr->m_bridges)
        {
            writer.StartArray();
            writer.StartObject();
            if (obj.is_valid())
            {
                //startp
                {
                    writer.Key("startp");
                    writer.StartArray();
                    writer.Double(obj.startp.x());
                    writer.Double(obj.startp.y());
                    writer.Double(obj.startp.z());
                    writer.EndArray();

                }
                //endp
                {
                    writer.Key("endp");
                    writer.StartArray();
                    writer.Double(obj.endp.x());
                    writer.Double(obj.endp.y());
                    writer.Double(obj.endp.z());
                    writer.EndArray();

                }
                writer.Key("r_mm"); writer.Double(obj.r);
                //////////////////////////////
                writer.Key("id"); writer.Int64(obj.id);

                {
                    writer.Key("briStartplink");
                    writer.StartArray();

                    for (SupportTreeNode2MeshLink& lk : obj.briStartplink)
                    {
                        writer.StartArray();
                        writer.Int((int)lk.type);
                        writer.Int64(lk.id);
                        writer.EndArray();
                    }
                    writer.EndArray();
                }


                {
                    writer.Key("linkL");
                    writer.StartArray();

                    for (SupportTreeNode2MeshLink& lk : obj.linkL)
                    {
                        writer.StartArray();
                        writer.Int((int)lk.type);
                        writer.Int64(lk.id);
                        writer.EndArray();
                    }
                    writer.EndArray();
                }
                //////////////////////////////
                {
                    writer.Key("linkR");
                    writer.StartArray();
                    for (SupportTreeNode2MeshLink& lk : obj.linkR)
                    {
                        writer.StartArray();
                        writer.Int((int)lk.type);
                        writer.Int64(lk.id);
                        writer.EndArray();
                    }
                    writer.EndArray();

                }
            }
            writer.EndObject();
            writer.EndArray();
        }
        writer.EndArray();

    }
    //writer.EndObject();


}
void writeCrossBridgesData(PrettyWriter<rapidjson::StringBuffer>& writer, SupportTreeBuilder* supportTreePtr)
{

    //writer.StartObject();
    {
        writer.Key("CrossBridges");
        writer.StartArray();
        for (CrossBridge& obj : supportTreePtr->m_crossbridges)
        {
            writer.StartArray();
            writer.StartObject();
            if (obj.is_valid())
            {
                //startp
                {
                    writer.Key("startp");
                    writer.StartArray();
                    writer.Double(obj.startp.x());
                    writer.Double(obj.startp.y());
                    writer.Double(obj.startp.z());
                    writer.EndArray();

                }
                //endp
                {
                    writer.Key("endp");
                    writer.StartArray();
                    writer.Double(obj.endp.x());
                    writer.Double(obj.endp.y());
                    writer.Double(obj.endp.z());
                    writer.EndArray();

                }
                writer.Key("r_mm"); writer.Double(obj.r);
                //////////////////////////////
                writer.Key("id"); writer.Int64(obj.id);
                //briStartplink
                {
                    writer.Key("briStartplink");
                    writer.StartArray();

                    for (SupportTreeNode2MeshLink& lk : obj.briStartplink)
                    {
                        writer.StartArray();
                        writer.Int((int)lk.type);
                        writer.Int64(lk.id);
                        writer.EndArray();
                    }
                    writer.EndArray();
                }

                {
                    writer.Key("linkL");
                    writer.StartArray();

                    for (SupportTreeNode2MeshLink& lk : obj.linkL)
                    {
                        writer.StartArray();
                        writer.Int((int)lk.type);
                        writer.Int64(lk.id);
                        writer.EndArray();
                    }
                    writer.EndArray();
                }
                //////////////////////////////
                {
                    writer.Key("linkR");
                    writer.StartArray();
                    for (SupportTreeNode2MeshLink& lk : obj.linkR)
                    {
                        writer.StartArray();
                        writer.Int((int)lk.type);
                        writer.Int64(lk.id);
                        writer.EndArray();
                    }
                    writer.EndArray();

                }
            }
            writer.EndObject();
            writer.EndArray();
        }
        writer.EndArray();

    }
    //writer.EndObject();


}
void writeDiffBridgesData(PrettyWriter<rapidjson::StringBuffer>& writer, SupportTreeBuilder* supportTreePtr)
{

    //writer.StartObject();
    {
        writer.Key("DiffBridges");
        writer.StartArray();
        for (DiffBridge& obj : supportTreePtr->m_diffbridges)
        {
            writer.StartArray();
            writer.StartObject();
            if (obj.is_valid())
            {
                //startp
                {
                    writer.Key("startp");
                    writer.StartArray();
                    writer.Double(obj.startp.x());
                    writer.Double(obj.startp.y());
                    writer.Double(obj.startp.z());
                    writer.EndArray();

                }
                //endp
                {
                    writer.Key("endp");
                    writer.StartArray();
                    writer.Double(obj.endp.x());
                    writer.Double(obj.endp.y());
                    writer.Double(obj.endp.z());
                    writer.EndArray();

                }
                writer.Key("r_mm"); writer.Double(obj.r);
                writer.Key("end_r_mm"); writer.Double(obj.end_r);
                //////////////////////////////
                writer.Key("id"); writer.Int64(obj.id);
                //briStartplink
                {
                    writer.Key("briStartplink");
                    writer.StartArray();

                    for (SupportTreeNode2MeshLink& lk : obj.briStartplink)
                    {
                        writer.StartArray();
                        writer.Int((int)lk.type);
                        writer.Int64(lk.id);
                        writer.EndArray();
                    }
                    writer.EndArray();
                }
                //linkL
                {
                    writer.Key("linkL");
                    writer.StartArray();

                    for (SupportTreeNode2MeshLink& lk : obj.linkL)
                    {
                        writer.StartArray();
                        writer.Int((int)lk.type);
                        writer.Int64(lk.id);
                        writer.EndArray();
                    }
                    writer.EndArray();
                }
                //////////////////////////////
                {
                    writer.Key("linkR");
                    writer.StartArray();
                    for (SupportTreeNode2MeshLink& lk : obj.linkR)
                    {
                        writer.StartArray();
                        writer.Int((int)lk.type);
                        writer.Int64(lk.id);
                        writer.EndArray();
                    }
                    writer.EndArray();

                }
            }
            writer.EndObject();
            writer.EndArray();
        }
        writer.EndArray();

    }
    //writer.EndObject();
}
void writePedestalsData(PrettyWriter<rapidjson::StringBuffer>& writer, SupportTreeBuilder* supportTreePtr)
{

    //writer.StartObject();
    {
        writer.Key("Pedestals");
        writer.StartArray();
        for (Pedestal& obj : supportTreePtr->m_pedestals)
        {
            writer.StartArray();
            writer.StartObject();
            if (obj.is_valid())
            {
                //pos
                {
                    writer.Key("pos");
                    writer.StartArray();
                    writer.Double(obj.pos.x());
                    writer.Double(obj.pos.y());
                    writer.Double(obj.pos.z());
                    writer.EndArray();

                }
                writer.Key("height"); writer.Double(obj.height);
                writer.Key("r_bottom"); writer.Double(obj.r_bottom);
                writer.Key("r_top"); writer.Double(obj.r_top);
                //////////////////////////////
                writer.Key("id"); writer.Int64(obj.id);

                {
                    writer.Key("linkL");
                    writer.StartArray();

                    for (SupportTreeNode2MeshLink& lk : obj.linkL)
                    {
                        writer.StartArray();
                        writer.Int((int)lk.type);
                        writer.Int64(lk.id);
                        writer.EndArray();
                    }
                    writer.EndArray();
                }
                //////////////////////////////
                {
                    writer.Key("linkR");
                    writer.StartArray();
                    for (SupportTreeNode2MeshLink& lk : obj.linkR)
                    {
                        writer.StartArray();
                        writer.Int((int)lk.type);
                        writer.Int64(lk.id);
                        writer.EndArray();
                    }
                    writer.EndArray();

                }
            }
            writer.EndObject();
            writer.EndArray();
        }
        writer.EndArray();

    }
    //writer.EndObject();
}
void writeAnchorsData(PrettyWriter<rapidjson::StringBuffer>& writer, SupportTreeBuilder* supportTreePtr)
{
    //writer.StartObject();
    {
        writer.Key("Anchors");
        writer.StartArray();
        for (Anchor& obj : supportTreePtr->m_anchors)
        {
            writer.StartArray();
            writer.StartObject();
            if (obj.is_valid())
            {
                //pos
                {
                    writer.Key("pos");
                    writer.StartArray();
                    writer.Double(obj.pos.x());
                    writer.Double(obj.pos.y());
                    writer.Double(obj.pos.z());
                    writer.EndArray();

                }
                //dir
                {
                    writer.Key("dir");
                    writer.StartArray();
                    writer.Double(obj.dir.x());
                    writer.Double(obj.dir.y());
                    writer.Double(obj.dir.z());
                    writer.EndArray();

                }
                writer.Key("r_back_mm"); writer.Double(obj.r_back_mm);
                writer.Key("r_pin_mm"); writer.Double(obj.r_pin_mm);
                writer.Key("penetration_mm"); writer.Double(obj.penetration_mm);
                writer.Key("width_mm"); writer.Double(obj.width_mm);
                //writer.Key("r_toptouch_mm"); writer.Double(obj.r_toptouch_mm);
                //writer.Key("toptouch_enable"); writer.Double(obj.toptouch_enable);
                //////////////////////////////
                writer.Key("id"); writer.Int64(obj.id);

                {
                    writer.Key("linkL");
                    writer.StartArray();

                    for (SupportTreeNode2MeshLink& lk : obj.linkL)
                    {
                        writer.StartArray();
                        writer.Int((int)lk.type);
                        writer.Int64(lk.id);
                        writer.EndArray();
                    }
                    writer.EndArray();
                }
                //////////////////////////////
                {
                    writer.Key("linkR");
                    writer.StartArray();
                    for (SupportTreeNode2MeshLink& lk : obj.linkR)
                    {
                        writer.StartArray();
                        writer.Int((int)lk.type);
                        writer.Int64(lk.id);
                        writer.EndArray();
                    }
                    writer.EndArray();

                }
            }
            writer.EndObject();
            writer.EndArray();
        }
        writer.EndArray();

    }
    //writer.EndObject();

}

void writePadData(PrettyWriter<rapidjson::StringBuffer>& writer, SupportTreeBuilder* supportTreePtr)
{
    //writer.StartObject();
    {
        struct stl_file* stl = &supportTreePtr->m_pad.tmesh.stl;
        int counts = stl->stats.number_of_facets;
        writer.Key("PAD.facets.count"); writer.Int(counts);
        writer.Key("Pad");
        writer.StartArray();
        for (int i = 0; i < counts; i++)
        {
            writer.StartArray();
            writer.Double(stl->facet_start[i].normal(0));
            writer.Double(stl->facet_start[i].normal(1));
            writer.Double(stl->facet_start[i].normal(2));
            writer.Double(stl->facet_start[i].vertex[0](0));
            writer.Double(stl->facet_start[i].vertex[0](1));
            writer.Double(stl->facet_start[i].vertex[0](2));
            writer.Double(stl->facet_start[i].vertex[1](0));
            writer.Double(stl->facet_start[i].vertex[1](1));
            writer.Double(stl->facet_start[i].vertex[1](2));
            writer.Double(stl->facet_start[i].vertex[2](0));
            writer.Double(stl->facet_start[i].vertex[2](1));
            writer.Double(stl->facet_start[i].vertex[2](2));
            writer.EndArray();
        }    
        writer.EndArray();

    }
    //writer.EndObject();

}
void writeSquarePedestalsData(PrettyWriter<rapidjson::StringBuffer>& writer, SupportTreeBuilder* supportTreePtr)
{

    //writer.StartObject();
    {
        writer.Key("SquarePedestals");
        writer.StartArray();
        for (SquarePedestal& obj : supportTreePtr->m_squarePedestals)
        {
            writer.StartArray();
            writer.StartObject();
            if (obj.is_valid())
            {
                //pos
                {
                    writer.Key("pos");
                    writer.StartArray();
                    writer.Double(obj.pos.x());
                    writer.Double(obj.pos.y());
                    writer.Double(obj.pos.z());
                    writer.EndArray();

                }
                writer.Key("height"); writer.Double(obj.height);
                writer.Key("r_bottom"); writer.Double(obj.r_bottom);
                writer.Key("r_top"); writer.Double(obj.r_top);
                //////////////////////////////
                writer.Key("id"); writer.Int64(obj.id);

                {
                    writer.Key("linkL");
                    writer.StartArray();

                    for (SupportTreeNode2MeshLink& lk : obj.linkL)
                    {
                        writer.StartArray();
                        writer.Int((int)lk.type);
                        writer.Int64(lk.id);
                        writer.EndArray();
                    }
                    writer.EndArray();
                }
                //////////////////////////////
                {
                    writer.Key("linkR");
                    writer.StartArray();
                    for (SupportTreeNode2MeshLink& lk : obj.linkR)
                    {
                        writer.StartArray();
                        writer.Int((int)lk.type);
                        writer.Int64(lk.id);
                        writer.EndArray();
                    }
                    writer.EndArray();

                }
            }
            writer.EndObject();
            writer.EndArray();
        }
        writer.EndArray();

    }
    //writer.EndObject();
}

//read
int readHeadsData(std::string filename,const char* data, SupportTreeBuilder* supportTreePtr)
{
    rapidjson::Document dom;

    if (dom.Parse(data).HasParseError())
    {
        fprintf(stderr, "fail to parse json file %s\n", filename.c_str());
        return -1;
    }

    if (dom.HasMember("Heads") && dom["Heads"].IsArray())
    {
        const rapidjson::Value& arr = dom["Heads"];
        for (int i = 0; i < arr.Size(); i++)
        {
            const rapidjson::Value& tmp = arr[i];
            if (tmp.IsArray())
            {
                for (int j = 0; j < tmp.Size(); j++)
                {
                    if (tmp[j].IsObject())
                    {
                        Head hd;
                        const rapidjson::Value& obj = tmp[j];
                        //pos

                        if (obj.HasMember("pos") && obj["pos"].IsArray())
                        {
                            const rapidjson::Value& postmp = obj["pos"];

                            for (int j = 0; j < postmp.Size(); j++) {
                                if (postmp[j].IsDouble()) {
                                    hd.pos[j] = postmp[j].GetDouble();//xyz
                                }
                            }
                        }
                        //dir
                        if (obj.HasMember("dir") && obj["dir"].IsArray())
                        {
                            const rapidjson::Value& postmp = obj["dir"];

                            for (int j = 0; j < postmp.Size(); j++) {
                                if (postmp[j].IsDouble()) {
                                    hd.dir[j] = postmp[j].GetDouble();//xyz

                                }
                            }
                        }
                        if (obj.HasMember("r_back_mm") && obj["r_back_mm"].IsDouble())
                        {
                            hd.r_back_mm = obj["r_back_mm"].GetDouble();
                        }
                        if (obj.HasMember("r_pin_mm") && obj["r_pin_mm"].IsDouble())
                        {
                            hd.r_pin_mm = obj["r_pin_mm"].GetDouble();
                        }
                        if (obj.HasMember("penetration_mm") && obj["penetration_mm"].IsDouble())
                        {
                            hd.penetration_mm = obj["penetration_mm"].GetDouble();
                        }
                        if (obj.HasMember("width_mm") && obj["width_mm"].IsDouble())
                        {
                            hd.width_mm = obj["width_mm"].GetDouble();
                        }
                        if (obj.HasMember("r_toptouch_mm") && obj["r_toptouch_mm"].IsDouble())
                        {
                            hd.r_toptouch_mm = obj["r_toptouch_mm"].GetDouble();
                        }
                        if (obj.HasMember("toptouch_enable") && obj["toptouch_enable"].IsBool())
                        {
                            hd.toptouch_enable = obj["toptouch_enable"].GetBool();
                        }
                        if (obj.HasMember("id") && obj["id"].IsInt64())
                        {
                            hd.id = obj["id"].GetInt64();
                        }
                        if (obj.HasMember("linkL") && obj["linkL"].IsArray())
                        {
                            const rapidjson::Value& linktmp = obj["linkL"];
                            for (int j = 0; j < linktmp.Size(); j++)
                            {
                                if (linktmp[j].IsArray())
                                {
                                    SupportTreeNode2MeshLink meshlktem;
                                    const rapidjson::Value& linktmpsub = linktmp[j];
                                    assert(linktmpsub.Size() == 2);
                                    meshlktem.type = (MeshTypeElement)linktmpsub[0].GetInt();
                                    meshlktem.id = linktmpsub[1].GetInt64();
                                    hd.linkL.emplace_back(meshlktem);
                                    //for (int k = 0; k < linktmpsub.Size(); k++)
                                    //{
                                    //    if (linktmpsub[k].IsInt64())
                                    //        std::cout << linktmpsub[k].GetInt64() << std::endl;
                                    //    else if (linktmpsub[k].IsInt())
                                    //        std::cout << linktmpsub[k].GetInt() << std::endl;
                                    //}
                                }
                            }
                        }
                        if (obj.HasMember("linkR") && obj["linkR"].IsArray())
                        {
                            const rapidjson::Value& linktmp = obj["linkR"];

                            for (int j = 0; j < linktmp.Size(); j++)
                            {
                                if (linktmp[j].IsArray())
                                {
                                    SupportTreeNode2MeshLink meshlktem;
                                    const rapidjson::Value& linktmpsub = linktmp[j];
                                    assert(linktmpsub.Size() == 2);
                                    meshlktem.type = (MeshTypeElement)linktmpsub[0].GetInt();
                                    meshlktem.id = linktmpsub[1].GetInt64();
                                    hd.linkR.emplace_back(meshlktem);
                                    //for (int k = 0; k < linktmpsub.Size(); k++)
                                    //{
                                    //    if (linktmpsub[k].IsInt64())
                                    //        std::cout << linktmpsub[k].GetInt64() << std::endl;
                                    //    else if (linktmpsub[k].IsInt())
                                    //        std::cout << linktmpsub[k].GetInt() << std::endl;
                                    //}
                                }
                            }
                        }
                        if (hd.is_valid())
                        {
                            supportTreePtr->add_head(hd.id, hd);
                            if (supportTreePtr->m_supportTreeBuildstepsPtr != nullptr)
                            {
                                supportTreePtr->m_supportTreeBuildstepsPtr->m_addHeadsize =std::max(hd.id,supportTreePtr->m_supportTreeBuildstepsPtr->m_addHeadsize);
                            }

                        }
                    }
                }
            }

        }
    }

    return 0;
}
int readPillarsData(std::string filename, const char* data, SupportTreeBuilder* supportTreePtr)
{
    rapidjson::Document dom;

    if (dom.Parse(data).HasParseError())
    {
        fprintf(stderr, "fail to parse json file %s\n", filename.c_str());
        return -1;
    }

    if (dom.HasMember("Pillars") && dom["Pillars"].IsArray())
    {
        const rapidjson::Value& arr = dom["Pillars"];
        for (int i = 0; i < arr.Size(); i++)
        {
            const rapidjson::Value& tmp = arr[i];
            if (tmp.IsArray())
            {
                for (int j = 0; j < tmp.Size(); j++)
                {
                    if (tmp[j].IsObject())
                    {
                        Pillar pillarhd;
                        const rapidjson::Value& obj = tmp[j];
                        //pos
                        //std::cout << "endpt" << std::endl;

                        if (obj.HasMember("endpt") && obj["endpt"].IsArray())
                        {
                            const rapidjson::Value& postmp = obj["endpt"];

                            for (int j = 0; j < postmp.Size(); j++) {
                                if (postmp[j].IsDouble()) {
                                    pillarhd.endpt[j] = postmp[j].GetDouble();//xyz
                                }
                            }
                        }
                        ////////////////////////////////
                        if (obj.HasMember("height") && obj["height"].IsDouble())
                        {
                            pillarhd.height = obj["height"].GetDouble();
                        }
                        if (obj.HasMember("r_mm") && obj["r_mm"].IsDouble())
                        {
                            pillarhd.r = obj["r_mm"].GetDouble();
                        }
                        if (obj.HasMember("bridges") && obj["bridges"].IsUint())
                        {
                            pillarhd.bridges = obj["bridges"].GetUint();
                        }
                        if (obj.HasMember("links") && obj["links"].IsUint())
                        {
                            pillarhd.links = obj["links"].GetUint();
                        }
                        if (obj.HasMember("bridgePillar") && obj["bridgePillar"].IsBool())
                        {
                            pillarhd.bridgePillar = obj["bridgePillar"].GetBool();
                        }
                        if (obj.HasMember("PillarSelfType") && obj["PillarSelfType"].IsInt())
                        {
                            pillarhd.selfType = obj["PillarSelfType"].GetInt();
                        }
                        /////////////////////////////////////////////
                        if (obj.HasMember("id") && obj["id"].IsInt64())
                        {
                            pillarhd.id = obj["id"].GetInt64();
                        }
                        if (obj.HasMember("linkL") && obj["linkL"].IsArray())
                        {
                            const rapidjson::Value& linktmp = obj["linkL"];
                            for (int j = 0; j < linktmp.Size(); j++)
                            {
                                if (linktmp[j].IsArray())
                                {
                                    SupportTreeNode2MeshLink meshlktem;
                                    const rapidjson::Value& linktmpsub = linktmp[j];
                                    assert(linktmpsub.Size() == 2);
                                    meshlktem.type = (MeshTypeElement)linktmpsub[0].GetInt();
                                    meshlktem.id = linktmpsub[1].GetInt64();
                                    pillarhd.linkL.emplace_back(meshlktem);
                                    //for (int k = 0; k < linktmpsub.Size(); k++)
                                    //{
                                    //    if (linktmpsub[k].IsInt64())
                                    //        std::cout << linktmpsub[k].GetInt64() << std::endl;
                                    //    else if (linktmpsub[k].IsInt())
                                    //        std::cout << linktmpsub[k].GetInt() << std::endl;
                                    //}
                                }
                            }
                        }
                        if (obj.HasMember("linkR") && obj["linkR"].IsArray())
                        {
                            const rapidjson::Value& linktmp = obj["linkR"];

                            for (int j = 0; j < linktmp.Size(); j++)
                            {
                                if (linktmp[j].IsArray())
                                {
                                    SupportTreeNode2MeshLink meshlktem;
                                    const rapidjson::Value& linktmpsub = linktmp[j];
                                    assert(linktmpsub.Size() == 2);
                                    meshlktem.type = (MeshTypeElement)linktmpsub[0].GetInt();
                                    meshlktem.id = linktmpsub[1].GetInt64();
                                    pillarhd.linkR.emplace_back(meshlktem);
                                    //for (int k = 0; k < linktmpsub.Size(); k++)
                                    //{
                                    //    if (linktmpsub[k].IsInt64())
                                    //        std::cout << linktmpsub[k].GetInt64() << std::endl;
                                    //    else if (linktmpsub[k].IsInt())
                                    //        std::cout << linktmpsub[k].GetInt() << std::endl;
                                    //}
                                }
                            }
                        }
                        if (pillarhd.is_valid())
                        {
                            SupportTreeBuildsteps* algPtr = (SupportTreeBuildsteps*)supportTreePtr->m_supportTreeBuildstepsPtr;
                            algPtr->m_pillar_index.guarded_insert(pillarhd.endpoint(), pillarhd.id);
                            algPtr->m_pillar_index_start.guarded_insert(pillarhd.startpoint(), pillarhd.id);
                            long id = pillarhd.id;
                            if (id >= supportTreePtr->m_pillars.size())
                                supportTreePtr->m_pillars.resize(id + 1);
                            supportTreePtr->m_pillars[id] = pillarhd;

                        }

                    }
                }
            }

        }
    }

    return 0;
}
int readJunctionsData(std::string filename, const char* data, SupportTreeBuilder* supportTreePtr)
{
    rapidjson::Document dom;

    if (dom.Parse(data).HasParseError())
    {
        fprintf(stderr, "fail to parse json file %s\n", filename.c_str());
        return -1;
    }

    if (dom.HasMember("Junctions") && dom["Junctions"].IsArray())
    {
        const rapidjson::Value& arr = dom["Junctions"];
        for (int i = 0; i < arr.Size(); i++)
        {
            const rapidjson::Value& tmp = arr[i];
            if (tmp.IsArray())
            {
                for (int j = 0; j < tmp.Size(); j++)
                {
                    if (tmp[j].IsObject())
                    {
                        Junction junhd;
                        const rapidjson::Value& obj = tmp[j];
                        //pos

                        if (obj.HasMember("pos") && obj["pos"].IsArray())
                        {
                            const rapidjson::Value& postmp = obj["pos"];

                            for (int j = 0; j < postmp.Size(); j++) {
                                if (postmp[j].IsDouble()) {
                                    junhd.pos[j] = postmp[j].GetDouble();//xyz
                                }
                            }
                        }
                        ////////////////////////////////
                        if (obj.HasMember("r_mm") && obj["r_mm"].IsDouble())
                        {
                            junhd.r = obj["r_mm"].GetDouble();
                        }
                        /////////////////////////////////////////////
                        if (obj.HasMember("id") && obj["id"].IsInt64())
                        {
                            junhd.id = obj["id"].GetInt64();
                        }
                        if (obj.HasMember("linkL") && obj["linkL"].IsArray())
                        {
                            const rapidjson::Value& linktmp = obj["linkL"];
                            for (int j = 0; j < linktmp.Size(); j++)
                            {
                                if (linktmp[j].IsArray())
                                {
                                    SupportTreeNode2MeshLink meshlktem;
                                    const rapidjson::Value& linktmpsub = linktmp[j];
                                    assert(linktmpsub.Size() == 2);
                                    meshlktem.type = (MeshTypeElement)linktmpsub[0].GetInt();
                                    meshlktem.id = linktmpsub[1].GetInt64();
                                    junhd.linkL.emplace_back(meshlktem);
                                    //for (int k = 0; k < linktmpsub.Size(); k++)
                                    //{
                                    //    if (linktmpsub[k].IsInt64())
                                    //        std::cout << linktmpsub[k].GetInt64() << std::endl;
                                    //    else if (linktmpsub[k].IsInt())
                                    //        std::cout << linktmpsub[k].GetInt() << std::endl;
                                    //}
                                }
                            }
                        }
                        if (obj.HasMember("linkR") && obj["linkR"].IsArray())
                        {
                            const rapidjson::Value& linktmp = obj["linkR"];

                            for (int j = 0; j < linktmp.Size(); j++)
                            {
                                if (linktmp[j].IsArray())
                                {
                                    SupportTreeNode2MeshLink meshlktem;
                                    const rapidjson::Value& linktmpsub = linktmp[j];
                                    assert(linktmpsub.Size() == 2);
                                    meshlktem.type = (MeshTypeElement)linktmpsub[0].GetInt();
                                    meshlktem.id = linktmpsub[1].GetInt64();
                                    junhd.linkR.emplace_back(meshlktem);
                                    //for (int k = 0; k < linktmpsub.Size(); k++)
                                    //{
                                    //    if (linktmpsub[k].IsInt64())
                                    //        std::cout << linktmpsub[k].GetInt64() << std::endl;
                                    //    else if (linktmpsub[k].IsInt())
                                    //        std::cout << linktmpsub[k].GetInt() << std::endl;
                                    //}
                                }
                            }
                        }
                        if (junhd.is_valid())
                        {
                            long id = junhd.id;
                            if (id >= supportTreePtr->m_junctions.size())
                                supportTreePtr->m_junctions.resize(id + 1);
                            supportTreePtr->m_junctions[id] = junhd;
                        }
                    }
                }
            }

        }
    }
    return 0;
}
int readBridgesData(std::string filename, const char* data, SupportTreeBuilder* supportTreePtr)
{
    rapidjson::Document dom;

    if (dom.Parse(data).HasParseError())
    {
        fprintf(stderr, "fail to parse json file %s\n", filename.c_str());
        return -1;
    }

    if (dom.HasMember("Bridges") && dom["Bridges"].IsArray())
    {
        const rapidjson::Value& arr = dom["Bridges"];
        for (int i = 0; i < arr.Size(); i++)
        {
            const rapidjson::Value& tmp = arr[i];
            if (tmp.IsArray())
            {
                for (int j = 0; j < tmp.Size(); j++)
                {
                    if (tmp[j].IsObject())
                    {
                        Bridge brihd;
                        const rapidjson::Value& obj = tmp[j];

                        if (obj.HasMember("startp") && obj["startp"].IsArray())
                        {
                            const rapidjson::Value& postmp = obj["startp"];

                            for (int j = 0; j < postmp.Size(); j++) {
                                if (postmp[j].IsDouble()) {
                                    brihd.startp[j] = postmp[j].GetDouble();//xyz
                                }
                            }
                        }
                        if (obj.HasMember("endp") && obj["endp"].IsArray())
                        {
                            const rapidjson::Value& postmp = obj["endp"];

                            for (int j = 0; j < postmp.Size(); j++) {
                                if (postmp[j].IsDouble()) {
                                    brihd.endp[j] = postmp[j].GetDouble();//xyz
                                }
                            }
                        }
                        ////////////////////////////////
                        if (obj.HasMember("r_mm") && obj["r_mm"].IsDouble())
                        {
                            brihd.r = obj["r_mm"].GetDouble();
                        }
                        /////////////////////////////////////////////
                        if (obj.HasMember("id") && obj["id"].IsInt64())
                        {
                            brihd.id = obj["id"].GetInt64();
                        }
                        //briStartplink
                        if (obj.HasMember("briStartplink") && obj["briStartplink"].IsArray())
                        {
                            const rapidjson::Value& linktmp = obj["briStartplink"];
                            for (int j = 0; j < linktmp.Size(); j++)
                            {
                                if (linktmp[j].IsArray())
                                {
                                    SupportTreeNode2MeshLink meshlktem;
                                    const rapidjson::Value& linktmpsub = linktmp[j];
                                    assert(linktmpsub.Size() == 2);
                                    meshlktem.type = (MeshTypeElement)linktmpsub[0].GetInt();
                                    meshlktem.id = linktmpsub[1].GetInt64();
                                    brihd.briStartplink.emplace_back(meshlktem);
                                }
                            }
                        }
                        //linkL
                        if (obj.HasMember("linkL") && obj["linkL"].IsArray())
                        {
                            const rapidjson::Value& linktmp = obj["linkL"];
                            for (int j = 0; j < linktmp.Size(); j++)
                            {
                                if (linktmp[j].IsArray())
                                {
                                    SupportTreeNode2MeshLink meshlktem;
                                    const rapidjson::Value& linktmpsub = linktmp[j];
                                    assert(linktmpsub.Size() == 2);
                                    meshlktem.type = (MeshTypeElement)linktmpsub[0].GetInt();
                                    meshlktem.id = linktmpsub[1].GetInt64();
                                    brihd.linkL.emplace_back(meshlktem);
                                    //for (int k = 0; k < linktmpsub.Size(); k++)
                                    //{
                                    //    if (linktmpsub[k].IsInt64())
                                    //        std::cout << linktmpsub[k].GetInt64() << std::endl;
                                    //    else if (linktmpsub[k].IsInt())
                                    //        std::cout << linktmpsub[k].GetInt() << std::endl;
                                    //}
                                }
                            }
                        }
                        //linkR
                        if (obj.HasMember("linkR") && obj["linkR"].IsArray())
                        {
                            const rapidjson::Value& linktmp = obj["linkR"];

                            for (int j = 0; j < linktmp.Size(); j++)
                            {
                                if (linktmp[j].IsArray())
                                {
                                    SupportTreeNode2MeshLink meshlktem;
                                    const rapidjson::Value& linktmpsub = linktmp[j];
                                    assert(linktmpsub.Size() == 2);
                                    meshlktem.type = (MeshTypeElement)linktmpsub[0].GetInt();
                                    meshlktem.id = linktmpsub[1].GetInt64();
                                    brihd.linkR.emplace_back(meshlktem);
                                    //for (int k = 0; k < linktmpsub.Size(); k++)
                                    //{
                                    //    if (linktmpsub[k].IsInt64())
                                    //        std::cout << linktmpsub[k].GetInt64() << std::endl;
                                    //    else if (linktmpsub[k].IsInt())
                                    //        std::cout << linktmpsub[k].GetInt() << std::endl;
                                    //}
                                }
                            }
                        }
                        //if (brihd.is_valid())
                        if (brihd.is_valid())
                        {
                            long id = brihd.id;
                            if (id >= supportTreePtr->m_bridges.size())
                                supportTreePtr->m_bridges.resize(id + 1);
                            supportTreePtr->m_bridges[id] = brihd;
                        }

                    }
                }
            }

        }
    }

    return 0;
}
int readCrossBridgesData(std::string filename, const char* data, SupportTreeBuilder* supportTreePtr)
{
    rapidjson::Document dom;

    if (dom.Parse(data).HasParseError())
    {
        fprintf(stderr, "fail to parse json file %s\n", filename.c_str());
        return -1;
    }

    if (dom.HasMember("CrossBridges") && dom["CrossBridges"].IsArray())
    {
        const rapidjson::Value& arr = dom["CrossBridges"];
        for (int i = 0; i < arr.Size(); i++)
        {
            const rapidjson::Value& tmp = arr[i];
            if (tmp.IsArray())
            {
                for (int j = 0; j < tmp.Size(); j++)
                {
                    if (tmp[j].IsObject())
                    {
                        CrossBridge brihd;
                        const rapidjson::Value& obj = tmp[j];

                        if (obj.HasMember("startp") && obj["startp"].IsArray())
                        {
                            const rapidjson::Value& postmp = obj["startp"];

                            for (int j = 0; j < postmp.Size(); j++) {
                                if (postmp[j].IsDouble()) {
                                    brihd.startp[j] = postmp[j].GetDouble();//xyz
                                }
                            }
                        }
                        if (obj.HasMember("endp") && obj["endp"].IsArray())
                        {
                            const rapidjson::Value& postmp = obj["endp"];

                            for (int j = 0; j < postmp.Size(); j++) {
                                if (postmp[j].IsDouble()) {
                                    brihd.endp[j] = postmp[j].GetDouble();//xyz
                                }
                            }
                        }
                        ////////////////////////////////
                        if (obj.HasMember("r_mm") && obj["r_mm"].IsDouble())
                        {
                            brihd.r = obj["r_mm"].GetDouble();
                        }
                        /////////////////////////////////////////////
                        if (obj.HasMember("id") && obj["id"].IsInt64())
                        {
                            brihd.id = obj["id"].GetInt64();
                        }
                        //briStartplink
                        if (obj.HasMember("briStartplink") && obj["briStartplink"].IsArray())
                        {
                            const rapidjson::Value& linktmp = obj["briStartplink"];
                            for (int j = 0; j < linktmp.Size(); j++)
                            {
                                if (linktmp[j].IsArray())
                                {
                                    SupportTreeNode2MeshLink meshlktem;
                                    const rapidjson::Value& linktmpsub = linktmp[j];
                                    assert(linktmpsub.Size() == 2);
                                    meshlktem.type = (MeshTypeElement)linktmpsub[0].GetInt();
                                    meshlktem.id = linktmpsub[1].GetInt64();
                                    brihd.briStartplink.emplace_back(meshlktem);
                                }
                            }
                        }
                        //linkL
                        if (obj.HasMember("linkL") && obj["linkL"].IsArray())
                        {
                            const rapidjson::Value& linktmp = obj["linkL"];
                            for (int j = 0; j < linktmp.Size(); j++)
                            {
                                if (linktmp[j].IsArray())
                                {
                                    SupportTreeNode2MeshLink meshlktem;
                                    const rapidjson::Value& linktmpsub = linktmp[j];
                                    assert(linktmpsub.Size() == 2);
                                    meshlktem.type = (MeshTypeElement)linktmpsub[0].GetInt();
                                    meshlktem.id = linktmpsub[1].GetInt64();
                                    brihd.linkL.emplace_back(meshlktem);
                                    //for (int k = 0; k < linktmpsub.Size(); k++)
                                    //{
                                    //    if (linktmpsub[k].IsInt64())
                                    //        std::cout << linktmpsub[k].GetInt64() << std::endl;
                                    //    else if (linktmpsub[k].IsInt())
                                    //        std::cout << linktmpsub[k].GetInt() << std::endl;
                                    //}
                                }
                            }
                        }
                        if (obj.HasMember("linkR") && obj["linkR"].IsArray())
                        {
                            const rapidjson::Value& linktmp = obj["linkR"];

                            for (int j = 0; j < linktmp.Size(); j++)
                            {
                                if (linktmp[j].IsArray())
                                {
                                    SupportTreeNode2MeshLink meshlktem;
                                    const rapidjson::Value& linktmpsub = linktmp[j];
                                    assert(linktmpsub.Size() == 2);
                                    meshlktem.type = (MeshTypeElement)linktmpsub[0].GetInt();
                                    meshlktem.id = linktmpsub[1].GetInt64();
                                    brihd.linkR.emplace_back(meshlktem);
                                    //for (int k = 0; k < linktmpsub.Size(); k++)
                                    //{
                                    //    if (linktmpsub[k].IsInt64())
                                    //        std::cout << linktmpsub[k].GetInt64() << std::endl;
                                    //    else if (linktmpsub[k].IsInt())
                                    //        std::cout << linktmpsub[k].GetInt() << std::endl;
                                    //}
                                }
                            }
                        }
                        //if (brihd.is_valid())
                        if (brihd.is_valid())
                        {
                            long id = brihd.id;
                            if (id >= supportTreePtr->m_crossbridges.size())
                                supportTreePtr->m_crossbridges.resize(id + 1);
                            supportTreePtr->m_crossbridges[id] = brihd;
                        }

                    }
                }
            }

        }
    }

    return 0;
}
int readDiffBridgesData(std::string filename, const char* data, SupportTreeBuilder* supportTreePtr)
{
    rapidjson::Document dom;

    if (dom.Parse(data).HasParseError())
    {
        fprintf(stderr, "fail to parse json file %s\n", filename.c_str());
        return -1;
    }

    if (dom.HasMember("DiffBridges") && dom["DiffBridges"].IsArray())
    {
        const rapidjson::Value& arr = dom["DiffBridges"];
        for (int i = 0; i < arr.Size(); i++)
        {
            const rapidjson::Value& tmp = arr[i];
            if (tmp.IsArray())
            {
                for (int j = 0; j < tmp.Size(); j++)
                {
                    if (tmp[j].IsObject())
                    {
                        DiffBridge brihd;
                        const rapidjson::Value& obj = tmp[j];

                        if (obj.HasMember("startp") && obj["startp"].IsArray())
                        {
                            const rapidjson::Value& postmp = obj["startp"];

                            for (int j = 0; j < postmp.Size(); j++) {
                                if (postmp[j].IsDouble()) {
                                    brihd.startp[j] = postmp[j].GetDouble();//xyz
                                }
                            }
                        }
                        if (obj.HasMember("endp") && obj["endp"].IsArray())
                        {
                            const rapidjson::Value& postmp = obj["endp"];

                            for (int j = 0; j < postmp.Size(); j++) {
                                if (postmp[j].IsDouble()) {
                                    brihd.endp[j] = postmp[j].GetDouble();//xyz
                                }
                            }
                        }
                        ////////////////////////////////
                        if (obj.HasMember("r_mm") && obj["r_mm"].IsDouble())
                        {
                            brihd.r = obj["r_mm"].GetDouble();
                        }

                        if (obj.HasMember("end_r_mm") && obj["end_r_mm"].IsDouble())
                        {
                            brihd.end_r = obj["end_r_mm"].GetDouble();
                        }
                        /////////////////////////////////////////////
                        if (obj.HasMember("id") && obj["id"].IsInt64())
                        {
                            brihd.id = obj["id"].GetInt64();
                        }
                        //briStartplink
                        if (obj.HasMember("briStartplink") && obj["briStartplink"].IsArray())
                        {
                            const rapidjson::Value& linktmp = obj["briStartplink"];
                            for (int j = 0; j < linktmp.Size(); j++)
                            {
                                if (linktmp[j].IsArray())
                                {
                                    SupportTreeNode2MeshLink meshlktem;
                                    const rapidjson::Value& linktmpsub = linktmp[j];
                                    assert(linktmpsub.Size() == 2);
                                    meshlktem.type = (MeshTypeElement)linktmpsub[0].GetInt();
                                    meshlktem.id = linktmpsub[1].GetInt64();
                                    brihd.briStartplink.emplace_back(meshlktem);
                                }
                            }
                        }
                        //linkL
                        if (obj.HasMember("linkL") && obj["linkL"].IsArray())
                        {
                            const rapidjson::Value& linktmp = obj["linkL"];
                            for (int j = 0; j < linktmp.Size(); j++)
                            {
                                if (linktmp[j].IsArray())
                                {
                                    SupportTreeNode2MeshLink meshlktem;
                                    const rapidjson::Value& linktmpsub = linktmp[j];
                                    assert(linktmpsub.Size() == 2);
                                    meshlktem.type = (MeshTypeElement)linktmpsub[0].GetInt();
                                    meshlktem.id = linktmpsub[1].GetInt64();
                                    brihd.linkL.emplace_back(meshlktem);
                                    //for (int k = 0; k < linktmpsub.Size(); k++)
                                    //{
                                    //    if (linktmpsub[k].IsInt64())
                                    //        std::cout << linktmpsub[k].GetInt64() << std::endl;
                                    //    else if (linktmpsub[k].IsInt())
                                    //        std::cout << linktmpsub[k].GetInt() << std::endl;
                                    //}
                                }
                            }
                        }
                        if (obj.HasMember("linkR") && obj["linkR"].IsArray())
                        {
                            const rapidjson::Value& linktmp = obj["linkR"];

                            for (int j = 0; j < linktmp.Size(); j++)
                            {
                                if (linktmp[j].IsArray())
                                {
                                    SupportTreeNode2MeshLink meshlktem;
                                    const rapidjson::Value& linktmpsub = linktmp[j];
                                    assert(linktmpsub.Size() == 2);
                                    meshlktem.type = (MeshTypeElement)linktmpsub[0].GetInt();
                                    meshlktem.id = linktmpsub[1].GetInt64();
                                    brihd.linkR.emplace_back(meshlktem);
                                    //for (int k = 0; k < linktmpsub.Size(); k++)
                                    //{
                                    //    if (linktmpsub[k].IsInt64())
                                    //        std::cout << linktmpsub[k].GetInt64() << std::endl;
                                    //    else if (linktmpsub[k].IsInt())
                                    //        std::cout << linktmpsub[k].GetInt() << std::endl;
                                    //}
                                }
                            }
                        }
                        //if (brihd.is_valid())
                        if (brihd.is_valid())
                        {
                            long id = brihd.id;
                            if (id >= supportTreePtr->m_diffbridges.size())
                                supportTreePtr->m_diffbridges.resize(id + 1);
                            supportTreePtr->m_diffbridges[id] = brihd;
                        }

                    }
                }
            }

        }
    }

    return 0;
}
int readPedestalsData(std::string filename, const char* data, SupportTreeBuilder* supportTreePtr)
{
    rapidjson::Document dom;

    if (dom.Parse(data).HasParseError())
    {
        fprintf(stderr, "fail to parse json file %s\n", filename.c_str());
        return -1;
    }

    if (dom.HasMember("Pedestals") && dom["Pedestals"].IsArray())
    {
        const rapidjson::Value& arr = dom["Pedestals"];
        for (int i = 0; i < arr.Size(); i++)
        {
            const rapidjson::Value& tmp = arr[i];
            if (tmp.IsArray())
            {
                for (int j = 0; j < tmp.Size(); j++)
                {
                    if (tmp[j].IsObject())
                    {
                        Pedestal pedestalhd;
                        const rapidjson::Value& obj = tmp[j];
                        //pos

                        if (obj.HasMember("pos") && obj["pos"].IsArray())
                        {
                            const rapidjson::Value& postmp = obj["pos"];

                            for (int j = 0; j < postmp.Size(); j++) {
                                if (postmp[j].IsDouble()) {
                                    pedestalhd.pos[j] = postmp[j].GetDouble();//xyz
                                }
                            }
                        }
                        ////////////////////////////////
                        if (obj.HasMember("height") && obj["height"].IsDouble())
                        {
                            pedestalhd.height = obj["height"].GetDouble();
                        }
                        if (obj.HasMember("r_bottom") && obj["r_bottom"].IsDouble())
                        {
                            pedestalhd.r_bottom = obj["r_bottom"].GetDouble();
                        }
                        if (obj.HasMember("r_top") && obj["r_top"].IsDouble())
                        {
                            pedestalhd.r_top = obj["r_top"].GetDouble();
                        }
                        /////////////////////////////////////////////
                        if (obj.HasMember("id") && obj["id"].IsInt64())
                        {
                            pedestalhd.id = obj["id"].GetInt64();
                        }
                        if (obj.HasMember("linkL") && obj["linkL"].IsArray())
                        {
                            const rapidjson::Value& linktmp = obj["linkL"];
                            for (int j = 0; j < linktmp.Size(); j++)
                            {
                                if (linktmp[j].IsArray())
                                {
                                    SupportTreeNode2MeshLink meshlktem;
                                    const rapidjson::Value& linktmpsub = linktmp[j];
                                    assert(linktmpsub.Size() == 2);
                                    meshlktem.type = (MeshTypeElement)linktmpsub[0].GetInt();
                                    meshlktem.id = linktmpsub[1].GetInt64();
                                    pedestalhd.linkL.emplace_back(meshlktem);
                                    //for (int k = 0; k < linktmpsub.Size(); k++)
                                    //{
                                    //    if (linktmpsub[k].IsInt64())
                                    //        std::cout << linktmpsub[k].GetInt64() << std::endl;
                                    //    else if (linktmpsub[k].IsInt())
                                    //        std::cout << linktmpsub[k].GetInt() << std::endl;
                                    //}
                                }
                            }
                        }
                        if (obj.HasMember("linkR") && obj["linkR"].IsArray())
                        {
                            const rapidjson::Value& linktmp = obj["linkR"];

                            for (int j = 0; j < linktmp.Size(); j++)
                            {
                                if (linktmp[j].IsArray())
                                {
                                    SupportTreeNode2MeshLink meshlktem;
                                    const rapidjson::Value& linktmpsub = linktmp[j];
                                    assert(linktmpsub.Size() == 2);
                                    meshlktem.type = (MeshTypeElement)linktmpsub[0].GetInt();
                                    meshlktem.id = linktmpsub[1].GetInt64();
                                    pedestalhd.linkR.emplace_back(meshlktem);
                                    //for (int k = 0; k < linktmpsub.Size(); k++)
                                    //{
                                    //    if (linktmpsub[k].IsInt64())
                                    //        std::cout << linktmpsub[k].GetInt64() << std::endl;
                                    //    else if (linktmpsub[k].IsInt())
                                    //        std::cout << linktmpsub[k].GetInt() << std::endl;
                                    //}
                                }
                            }
                        }
                        //if (pedestalhd.is_valid())
                        if (pedestalhd.is_valid())
                        {
                            long id = pedestalhd.id;
                            if (id >= supportTreePtr->m_pedestals.size())
                                supportTreePtr->m_pedestals.resize(id + 1);
                            supportTreePtr->m_pedestals[id] = pedestalhd;
                        }

                    }
                }
            }

        }
    }
    return 0;
}
int readAnchorsData(std::string filename, const char* data, SupportTreeBuilder* supportTreePtr)
{
    rapidjson::Document dom;

    if (dom.Parse(data).HasParseError())
    {
        fprintf(stderr, "fail to parse json file %s\n", filename.c_str());
        return -1;
    }

    if (dom.HasMember("Anchors") && dom["Anchors"].IsArray())
    {
        const rapidjson::Value& arr = dom["Anchors"];
        for (int i = 0; i < arr.Size(); i++)
        {
            const rapidjson::Value& tmp = arr[i];
            if (tmp.IsArray())
            {
                for (int j = 0; j < tmp.Size(); j++)
                {
                    if (tmp[j].IsObject())
                    {
                        Anchor hd;
                        const rapidjson::Value& obj = tmp[j];
                        //pos
                        //std::cout << "pos" << std::endl;

                        if (obj.HasMember("pos") && obj["pos"].IsArray())
                        {
                            const rapidjson::Value& postmp = obj["pos"];

                            for (int j = 0; j < postmp.Size(); j++) {
                                if (postmp[j].IsDouble()) {
                                    hd.pos[j] = postmp[j].GetDouble();//xyz
                                }
                            }
                        }
                        //dir
                        if (obj.HasMember("dir") && obj["dir"].IsArray())
                        {
                            const rapidjson::Value& postmp = obj["dir"];

                            for (int j = 0; j < postmp.Size(); j++) {
                                if (postmp[j].IsDouble()) {
                                    hd.dir[j] = postmp[j].GetDouble();//xyz

                                }
                            }
                        }
                        if (obj.HasMember("r_back_mm") && obj["r_back_mm"].IsDouble())
                        {
                            hd.r_back_mm = obj["r_back_mm"].GetDouble();
                        }
                        if (obj.HasMember("r_pin_mm") && obj["r_pin_mm"].IsDouble())
                        {
                            hd.r_pin_mm = obj["r_pin_mm"].GetDouble();
                        }
                        if (obj.HasMember("penetration_mm") && obj["penetration_mm"].IsDouble())
                        {
                            hd.penetration_mm = obj["penetration_mm"].GetDouble();
                        }
                        if (obj.HasMember("width_mm") && obj["width_mm"].IsDouble())
                        {
                            hd.width_mm = obj["width_mm"].GetDouble();
                        }
                        /////////////////////////////////////////////////////
                        if (obj.HasMember("id") && obj["id"].IsInt64())
                        {
                            hd.id = obj["id"].GetInt64();
                        }
                        if (obj.HasMember("linkL") && obj["linkL"].IsArray())
                        {
                            const rapidjson::Value& linktmp = obj["linkL"];
                            for (int j = 0; j < linktmp.Size(); j++)
                            {
                                if (linktmp[j].IsArray())
                                {
                                    SupportTreeNode2MeshLink meshlktem;
                                    const rapidjson::Value& linktmpsub = linktmp[j];
                                    assert(linktmpsub.Size() == 2);
                                    meshlktem.type = (MeshTypeElement)linktmpsub[0].GetInt();
                                    meshlktem.id = linktmpsub[1].GetInt64();
                                    hd.linkL.emplace_back(meshlktem);
                                    //for (int k = 0; k < linktmpsub.Size(); k++)
                                    //{
                                    //    if (linktmpsub[k].IsInt64())
                                    //        std::cout << linktmpsub[k].GetInt64() << std::endl;
                                    //    else if (linktmpsub[k].IsInt())
                                    //        std::cout << linktmpsub[k].GetInt() << std::endl;
                                    //}
                                }
                            }
                        }
                        if (obj.HasMember("linkR") && obj["linkR"].IsArray())
                        {
                            const rapidjson::Value& linktmp = obj["linkR"];

                            for (int j = 0; j < linktmp.Size(); j++)
                            {
                                if (linktmp[j].IsArray())
                                {
                                    SupportTreeNode2MeshLink meshlktem;
                                    const rapidjson::Value& linktmpsub = linktmp[j];
                                    assert(linktmpsub.Size() == 2);
                                    meshlktem.type = (MeshTypeElement)linktmpsub[0].GetInt();
                                    meshlktem.id = linktmpsub[1].GetInt64();
                                    hd.linkR.emplace_back(meshlktem);
                                    //for (int k = 0; k < linktmpsub.Size(); k++)
                                    //{
                                    //    if (linktmpsub[k].IsInt64())
                                    //        std::cout << linktmpsub[k].GetInt64() << std::endl;
                                    //    else if (linktmpsub[k].IsInt())
                                    //        std::cout << linktmpsub[k].GetInt() << std::endl;
                                    //}
                                }
                            }
                        }
                        //if (hd.is_valid())
                        supportTreePtr->m_anchors.emplace_back(hd);
                        if (hd.is_valid())
                        {
                            long id = hd.id;
                            if (id >= supportTreePtr->m_anchors.size())
                                supportTreePtr->m_anchors.resize(id + 1);
                            supportTreePtr->m_anchors[id] = hd;
                        }

                    }
                }
            }

        }
    }

    return 0;
}
#if 0
int readPadData(std::string filename, const char* data, SupportTreeBuilder* supportTreePtr)
{
    struct stl_file* stl = &supportTreePtr->m_pad.tmesh.stl;
    std::stringstream ss;
    std::string label = "pad";
    ss << "solid " << label << "\n";

    for (uint32_t i = 0; i < stl->stats.number_of_facets; ++i) {
        ss << "facet normal " << stl->facet_start[i].normal(0) << stl->facet_start[i].normal(1) << stl->facet_start[i].normal(2) << "\n";
        ss << "outer loop\n";
        ss << "vertex " << stl->facet_start[i].vertex[0](0) << " " << stl->facet_start[i].vertex[0](1) << " " << stl->facet_start[i].vertex[0](2) << "\n";
        ss << "vertex " << stl->facet_start[i].vertex[1](0) << " " << stl->facet_start[i].vertex[1](1) << " " << stl->facet_start[i].vertex[1](2) << "\n";
        ss << "vertex " << stl->facet_start[i].vertex[2](0) << " " << stl->facet_start[i].vertex[2](1) << " " << stl->facet_start[i].vertex[2](2) << "\n";
        ss << "endfacet\n";
    }
    ss << "endsolid " << label << "\n";

    data.clear();
    data = ss.str();

    return 0;
}
#endif
int readPadData(std::string filename, const char* data, SupportTreeBuilder* supportTreePtr)
{
    //struct stl_file* stl = &supportTreePtr->m_pad.tmesh.stl;
    rapidjson::Document dom;

    if (dom.Parse(data).HasParseError())
    {
        fprintf(stderr, "fail to parse json file %s\n", filename.c_str());
        return -1;
    }
    //struct stl_file* stl = &supportTreePtr->m_pad.tmesh.stl;
    //int counts = stl->stats.number_of_facets;
    //writer.Key("PAD.facets.count"); writer.Int(counts);
    //writer.Key("Pad");
    //writer.StartArray();
    //for (int i = 0; i < counts; i++)
    //{
    //    writer.StartArray();
    //    writer.Double(stl->facet_start[i].normal(0));
    //    writer.Double(stl->facet_start[i].normal(1));
    //    writer.Double(stl->facet_start[i].normal(2));
    //    writer.Double(stl->facet_start[i].vertex[0](0));
    //    writer.Double(stl->facet_start[i].vertex[0](1));
    //    writer.Double(stl->facet_start[i].vertex[0](2));
    //    writer.Double(stl->facet_start[i].vertex[1](0));
    //    writer.Double(stl->facet_start[i].vertex[1](1));
    //    writer.Double(stl->facet_start[i].vertex[1](2));
    //    writer.Double(stl->facet_start[i].vertex[2](0));
    //    writer.Double(stl->facet_start[i].vertex[2](1));
    //    writer.Double(stl->facet_start[i].vertex[2](2));
    //    writer.EndArray();
    //}
    //writer.EndArray()

    Pointf3s points;
    std::vector<Vec3i> facets;

    int index = 0;

    if (dom.HasMember("Pad") && dom["Pad"].IsArray()) {
        const rapidjson::Value& arr = dom["Pad"];
        int arrSize = arr.Size();
        for (int i = 0; i < arrSize; ++i) {
            const rapidjson::Value& tmp = arr[i];

            if (12 == tmp.Size())
            {
                points.emplace_back(tmp[3].GetDouble(), tmp[4].GetDouble(), tmp[5].GetDouble());
                points.emplace_back(tmp[6].GetDouble(), tmp[7].GetDouble(), tmp[8].GetDouble());
                points.emplace_back(tmp[9].GetDouble(), tmp[10].GetDouble(), tmp[11].GetDouble());

                int f1 = index++;
                int f2 = index++;
                int f3 = index++;

                facets.emplace_back(f1, f2, f3);

                //stl_facet facet;
                //facet.normal(0) = tmp[0].GetDouble();
                //facet.normal(1) = tmp[1].GetDouble();
                //facet.normal(2) = tmp[2].GetDouble();
                //facet.vertex[0](0) = tmp[3].GetDouble();
                //facet.vertex[0](1) = tmp[4].GetDouble();
                //facet.vertex[0](2) = tmp[5].GetDouble();
                //facet.vertex[1](0) = tmp[6].GetDouble();
                //facet.vertex[1](1) = tmp[7].GetDouble();
                //facet.vertex[1](2) = tmp[8].GetDouble();
                //facet.vertex[2](0) = tmp[9].GetDouble();
                //facet.vertex[2](1) = tmp[10].GetDouble();
                //facet.vertex[2](2) = tmp[11].GetDouble();
                //stl->facet_start.push_back(facet);

                
            }
        }
        supportTreePtr->m_pad.tmesh = TriangleMesh(points, facets);
        supportTreePtr->m_pad.tmesh.require_shared_vertices();
    }
    return 0;
}
int readSquarePedestalsData(std::string filename, const char* data, SupportTreeBuilder* supportTreePtr)
{
    rapidjson::Document dom;

    if (dom.Parse(data).HasParseError())
    {
        fprintf(stderr, "fail to parse json file %s\n", filename.c_str());
        return -1;
    }

    if (dom.HasMember("SquarePedestals") && dom["SquarePedestals"].IsArray())
    {
        const rapidjson::Value& arr = dom["SquarePedestals"];
        for (int i = 0; i < arr.Size(); i++)
        {
            const rapidjson::Value& tmp = arr[i];
            if (tmp.IsArray())
            {
                for (int j = 0; j < tmp.Size(); j++)
                {
                    if (tmp[j].IsObject())
                    {
                        SquarePedestal pedestalhd;
                        const rapidjson::Value& obj = tmp[j];
                        //pos

                        if (obj.HasMember("pos") && obj["pos"].IsArray())
                        {
                            const rapidjson::Value& postmp = obj["pos"];

                            for (int j = 0; j < postmp.Size(); j++) {
                                if (postmp[j].IsDouble()) {
                                    pedestalhd.pos[j] = postmp[j].GetDouble();//xyz
                                }
                            }
                        }
                        ////////////////////////////////
                        if (obj.HasMember("height") && obj["height"].IsDouble())
                        {
                            pedestalhd.height = obj["height"].GetDouble();
                        }
                        if (obj.HasMember("r_bottom") && obj["r_bottom"].IsDouble())
                        {
                            pedestalhd.r_bottom = obj["r_bottom"].GetDouble();
                        }
                        if (obj.HasMember("r_top") && obj["r_top"].IsDouble())
                        {
                            pedestalhd.r_top = obj["r_top"].GetDouble();
                        }
                        /////////////////////////////////////////////
                        if (obj.HasMember("id") && obj["id"].IsInt64())
                        {
                            pedestalhd.id = obj["id"].GetInt64();
                        }
                        if (obj.HasMember("linkL") && obj["linkL"].IsArray())
                        {
                            const rapidjson::Value& linktmp = obj["linkL"];
                            for (int j = 0; j < linktmp.Size(); j++)
                            {
                                if (linktmp[j].IsArray())
                                {
                                    SupportTreeNode2MeshLink meshlktem;
                                    const rapidjson::Value& linktmpsub = linktmp[j];
                                    assert(linktmpsub.Size() == 2);
                                    meshlktem.type = (MeshTypeElement)linktmpsub[0].GetInt();
                                    meshlktem.id = linktmpsub[1].GetInt64();
                                    pedestalhd.linkL.emplace_back(meshlktem);
                                    //for (int k = 0; k < linktmpsub.Size(); k++)
                                    //{
                                    //    if (linktmpsub[k].IsInt64())
                                    //        std::cout << linktmpsub[k].GetInt64() << std::endl;
                                    //    else if (linktmpsub[k].IsInt())
                                    //        std::cout << linktmpsub[k].GetInt() << std::endl;
                                    //}
                                }
                            }
                        }
                        if (obj.HasMember("linkR") && obj["linkR"].IsArray())
                        {
                            const rapidjson::Value& linktmp = obj["linkR"];

                            for (int j = 0; j < linktmp.Size(); j++)
                            {
                                if (linktmp[j].IsArray())
                                {
                                    SupportTreeNode2MeshLink meshlktem;
                                    const rapidjson::Value& linktmpsub = linktmp[j];
                                    assert(linktmpsub.Size() == 2);
                                    meshlktem.type = (MeshTypeElement)linktmpsub[0].GetInt();
                                    meshlktem.id = linktmpsub[1].GetInt64();
                                    pedestalhd.linkR.emplace_back(meshlktem);
                                    //for (int k = 0; k < linktmpsub.Size(); k++)
                                    //{
                                    //    if (linktmpsub[k].IsInt64())
                                    //        std::cout << linktmpsub[k].GetInt64() << std::endl;
                                    //    else if (linktmpsub[k].IsInt())
                                    //        std::cout << linktmpsub[k].GetInt() << std::endl;
                                    //}
                                }
                            }
                        }
                        //if (pedestalhd.is_valid())
                        if (pedestalhd.is_valid())
                        {
                            long id = pedestalhd.id;
                            if (id >= supportTreePtr->m_squarePedestals.size())
                                supportTreePtr->m_squarePedestals.resize(id + 1);
                            supportTreePtr->m_squarePedestals[id] = pedestalhd;
                        }

                    }
                }
            }

        }
    }
    return 0;
}


}//end sla 
} //end Slic3r

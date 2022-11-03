#ifndef PROCESS_AI_PICTURE_H
#define PROCESS_AI_PICTURE_H
#include <iostream>
#include <fstream>
using namespace std;

union fileHead{
    char a[128] = {0};
    struct {
        unsigned int  magic;                //0x3DA13DA1
        unsigned int  crc32;                
        unsigned int  version_num;          
        unsigned int  total_picture;        
        unsigned int  total_layer;          
        unsigned int  file_size;            
        char relate_gcode_name[64];         
    };
};


union imageHead{
    char a[64] = {0};
    struct {
        float         percentage;        
        unsigned int  layer_num;         
        unsigned int  file_size;         
        float         roi_info[4];       
        char name[32];                   
    };
};

struct imgInfo
{
    char* data;
    size_t size;
};

class processAiPicture
{
    ofstream m_outFile;
    fileHead m_head;
public:
    processAiPicture(std::string fileName,std::string fullPath,unsigned int totalLayers, unsigned int total_picture);
    ~processAiPicture();
    void writeImage(imageHead iHead,imgInfo iInfo);

};

#endif // PROCESS_AI_PICTURE_H

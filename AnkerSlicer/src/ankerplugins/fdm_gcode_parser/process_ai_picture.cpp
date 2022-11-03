#include "process_ai_picture.h"


processAiPicture::processAiPicture(std::string fileName,std::string fullPath,unsigned int totalLayers, unsigned int total_picture = 100)
{
    this->m_head.magic = 0x3DA13DA1;
    std::copy(fileName.begin(),fileName.end(),this->m_head.relate_gcode_name); //TODO: is need check
    this->m_head.version_num = 1;//TODO: need version name
    this->m_head.total_layer = totalLayers;
    this->m_head.total_picture = total_picture;
    this->m_head.file_size = 128;
    m_outFile.open(fullPath, ios::out | ios::binary);
    m_outFile.write(this->m_head.a,sizeof(this->m_head));
    m_outFile.flush();
}

processAiPicture::~processAiPicture()
{
    
    this->m_outFile.seekp(0, ios::beg);
    this->m_outFile.write(this->m_head.a, sizeof(this->m_head));
    this->m_outFile.flush();
    this->m_outFile.close();
}

void processAiPicture::writeImage(imageHead iHead, imgInfo iInfo)
{
    m_outFile.write(iHead.a, sizeof(iHead));
    this->m_head.file_size += 64;
    m_outFile.flush();
    m_outFile.write(iInfo.data, iInfo.size);
    this->m_head.file_size += iInfo.size;
    m_outFile.flush();
}

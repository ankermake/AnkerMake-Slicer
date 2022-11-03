#include "read_mesh.h"
#include <fstream>
#include <QDebug>
#include <QIODevice>


bool ReadMesh::checkMeshHeader(const QString &filename)
{
    int index = filename.lastIndexOf(".");
    QString suffix = filename.mid(index, filename.length() - index);
    if(suffix.toLower().contains("stl"))
    {
       return checkStlHeader(filename);
    }
    else if(suffix.toLower().contains("obj"))
    {
        return checkObjHeader(filename);
    }
    return false;
}

bool ReadMesh::checkObjHeader(const QString &filename)
{
//    QFile file(filename);
//    if(file.open(QIODevice::ReadOnly))
//    {
//        int fileSize = file.size();

//        {
//            return false;
//        }
//        while(file.canReadLine())
//        {
//           QString lineStr = file.readLine();
//           QStringList dataList = lineStr.split(" ");
//           if(dataList[0] == "v" || dataList[0] == "vn")
//           {

//                {
//                    return true;
//                }
//                else
//                {
//                    return false;
//                }
//           }

//        }
//    }
    return true;
}

bool ReadMesh::checkStlHeader(const QString &filename)
{
    //bool ret = false;
    QFile file(filename);

    if(file.open(QIODevice::ReadOnly))
    {
        int fileSize = file.size();
        if(fileSize < 84) 
        {
            return false;
        }
        char ch[80];
        file.read(ch, 80);

        
        QString tmpStr = QString::fromUtf8(ch);
        if(tmpStr.toLower().contains(QString("solid"))) 
        {
            file.close();
            return true;
        }

        
        int triangles;
        file.read((char*)&triangles, sizeof(int)); 
        qDebug() << "triangles: " << triangles;
        if(triangles > 0)
        {
            int totalSize = triangles * 50 + 84; 
            file.close();
            if(fileSize == totalSize)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        file.close();
        return false;
    }
    return false;
}

/**
 *










*/

bool ReadMesh::checkBinaryStlFile(const QString &filename)
{
    char str[80] = {0};
    std::ifstream in(QString(filename.toLocal8Bit()).toStdString().c_str(), std::ifstream::in | std::ifstream::binary);
    if(!in)
    {
        return false;
    }
    in.read(str, 80); 

    //number of triangles
    int triangles;
    in.read((char*)&triangles, sizeof(int)); 

    if(triangles <= 0)
    {
        in.close();
        return false;
    }

    float coordXYZ[12] = { 0.0 };
    in.read((char *)coordXYZ, 12 * sizeof(float));
    qDebug() << "coordXYZ:" << coordXYZ[0] << coordXYZ[1] << coordXYZ[2];

    in.read((char *)coordXYZ, 2);
    in.close();
    return false;
}

bool ReadMesh::checkAsciiStlFile(const QString &filename)
{
    int i = 0, j = 0, cnt = 0, pCnt = 0;
    char a[100] = { 0 };
    char str[100] = { 0 };
    double x = 0.0, y = 0.0, z = 0.0;
    std::ifstream in(QString(filename.toLocal8Bit()).toStdString().c_str(), std::ifstream::in);
    if(!in)
    {
        return false;
    }

    in.getline(a, 100, '\n');
    qDebug() << "getline: " << a[0] << a[1] << a[2];
    in.close();
    return false;
}

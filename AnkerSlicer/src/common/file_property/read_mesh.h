#ifndef READ_MESH_H
#define READ_MESH_H

#include <QFile>
#include <iostream>
#include <QString>
class ReadMesh
{
public:
    static bool checkMeshHeader(const QString &filename);
    static bool checkObjHeader(const QString &filename);
    static bool checkStlHeader(const QString &filename); 
    static bool checkBinaryStlFile(const QString &filename);
    static bool checkAsciiStlFile(const QString &filename);

};


#endif


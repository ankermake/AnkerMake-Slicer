#ifndef READ_MESH_H
#define READ_MESH_H

#include <QFile>
#include <iostream>
#include <QString>
#if defined(QT_SHARED)
#ifdef COMMONLIB
#define COMMONLIB_EXPORT Q_DECL_EXPORT
#else
#define COMMONLIB_EXPORT Q_DECL_IMPORT
#endif
#else
#define COMMONLIB_EXPORT
#endif
class COMMONLIB_EXPORT ReadMesh
{
public:
    static bool checkMeshHeader(const QString &filename);
    static bool checkObjHeader(const QString &filename);
    static bool checkStlHeader(const QString &filename); 
    static bool checkBinaryStlFile(const QString &filename);
    static bool checkAsciiStlFile(const QString &filename);

};


#endif


#ifndef IMPORT_EXPORT_SUPPORT_H
#define IMPORT_EXPORT_SUPPORT_H

#include "../GeoAndShow/CHMeshShowObj.h"
#include "xml_stream.h"
#include <QString>

class Import_Export_Support
{
public:
    static bool importSupport(const QString& inPathName, CHMeshShowObjPtr modelPtr);
    static bool exportSupport(const QString& outPathName, CHMeshShowObjPtr modelPtr);
    static bool writeMeshToSupportFile(const CHMeshShowObjPtr& mesh, const QString& path, bool binary);
    static bool readSupportFileToMesh(CHMeshShowObjPtr& mesh, const QString& inPath);
    static bool readAscii(CHMeshShowObjPtr& mesh, const std::string& inPath);
    static bool readBinary(CHMeshShowObjPtr& mesh, const std::string& inPath);
private:

};


#endif // !IMPORT_EXPORT_SUPPORT_H

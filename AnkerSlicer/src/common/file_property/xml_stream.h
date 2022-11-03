/****************************************************************************
Revision 0.1  2022/01/07 9:38:03  Aden Hu
****************************************************************************/
#ifndef XML_STREAM_H
#define XML_STREAM_H

#include "file_property.h"
#include <QObject>
#include <QString>
#include <QMap>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDir>
#include <QDebug>
#include <QXmlStreamAttributes>
#include "../GeoAndShow/support/SupportMesh.h"

class XmlStream : QObject
{
    Q_OBJECT
public:
    enum XmlStreamType
    {
        XmlStreamType_App = 10001,
        XmlStreamType_Document = 10002,
        XmlStreamType_View = 10003,
        XmlStreamType_PluginPamas = 10004,
        XmlStreamType_ModelFolder_Transform = 10005,
        XmlStreamType_Support = 10006,
    };

    XmlStream(const QString& fileName, FileProperty* fileProp, const XmlStreamType& xmlType);
    ~XmlStream();

    static bool wirteSupportParamsXml(const CH3DPrintModelPtr& printModel, const QString& pathName);
    static bool readSupportParamsXml(CH3DPrintModelPtr& printModel, const QString& pathName);

    static bool writeSupportParamsXml(const CH3DPrintModelPtr& printModel, const QString& pathName);
    static bool readSupportParamsXml2(CH3DPrintModelPtr& printModel, const QString& pathName);

    bool readFile();
    bool writeFile();

    static void createSupportType(SupportMesh::EMeshType sType, SupportMeshPtr &supportPtr);
private:
    void writeAppXmlElement();
    bool readAppXmlElement();

    void writeDocumentXmlElement();
    bool readDocumentXmlElement();

    void writeViewXmlElement();
    bool readViewXmlElement();

    void writePluginPamasXmlElement();
    bool readPluginPamasXmlElement();

    void writeModelFolderXmlElement();
    bool readModelFolderXmlElement();

    void writeSupportParamsXmlElement();
    bool readSupportParamsXmlElement();

    static SupportMeshPtr createSupportMesh(SupportMesh::EMeshType meshType);

private:
    QString m_fileName;
    QXmlStreamReader m_reader;
    QXmlStreamWriter m_writer;
    XmlStreamType m_type;
    FileProperty* m_file_prop;

};


#endif // !XML_STREAM_H

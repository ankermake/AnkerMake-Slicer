#include "xml_stream.h"
#include "xml_stream.h"
#include "../common/GeoAndShow/CH3DPrintModel.h"
#include "../common/GeoAndShow/support/SupportCuboid.h"
#include "../common/GeoAndShow/support/SupportCone.h"
#include "../common/GeoAndShow/support/SupportAssemblyMeshes.h"
#include "../common/GeoAndShow/support/SupportCylinder.h"
#include "../common/GeoAndShow/support/SupportMesh.h"
#include "../common/GeoAndShow/support/SupportSphere.h"
#include "../common/GeoAndShow/support/SupportTetrahedron.h"
#include "../common/GeoAndShow/support/SupportTrapezoidBody.h"


#include "mesh_model_import_export.h"
#include <QFile>
#include <vector>

XmlStream::XmlStream(const QString& fileName, FileProperty* fileProp, const XmlStreamType& xmlType)
    : m_fileName(fileName), m_file_prop(fileProp), m_type(xmlType)
{

}

XmlStream::~XmlStream()
{
}

void XmlStream::createSupportType(SupportMesh::EMeshType sType, SupportMeshPtr& supportPtr)
{
    switch (sType)
    {
    case SupportMesh::EMeshType::AssemblyMeshes:
        supportPtr = SupportMeshPtr(new SupportAssemblyMeshes);
        break;
    case SupportMesh::EMeshType::Cuboid:
        supportPtr = SupportMeshPtr(new SupportCuboid);
        break;
    case SupportMesh::EMeshType::Cone:
        supportPtr = SupportMeshPtr(new SupportCone);
        break;
    case SupportMesh::EMeshType::Cylinder:
        supportPtr = SupportMeshPtr(new SupportCylinder);
        break;
    case SupportMesh::EMeshType::Sphere:
        supportPtr = SupportMeshPtr(new SupportSphere);
        break;
    case SupportMesh::EMeshType::Tetrahedron:
        supportPtr = SupportMeshPtr(new SupportTetrahedron);
        break;
    case SupportMesh::EMeshType::TrapezoidBody:
        supportPtr = SupportMeshPtr(new SupportTrapezoidBody);
        break;
    default:
        break;
    }
    if (supportPtr != nullptr)
    {
        supportPtr->setType(sType);
    }
}

bool XmlStream::writeSupportParamsXml(const CH3DPrintModelPtr& printModel, const QString& pathName)
{
    if (printModel == nullptr)
    {
        return false;
    }
    QString meshName = pathName;
    int index1 = meshName.lastIndexOf("/");
    meshName.chop(1);
    int index2 = meshName.lastIndexOf("/") + 1;
    meshName = pathName.mid(index2, index1 - index2);
    QString xmlName = pathName + meshName + QString(".xml");

    QFile file(xmlName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Error: Cannot Open File " << pathName;
        return false;
    }
    QXmlStreamWriter writer;
    writer.setDevice(&file);
    writer.setCodec("UTF-8");
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement(QString("Mesh"));
    writer.writeAttribute(QString("meshName"), meshName);
    writer.writeAttribute(QString("meshPath"), printModel->m_filePath);
    for (int i = 0; i < printModel->m_supportMeshes.size(); i++)
    {
        writer.writeStartElement(QString("Support"));
        writer.writeAttribute(QString("name"), printModel->m_supportMeshes[i]->getObjectName());
        writer.writeAttribute(QString("type"), QString::number((int)printModel->m_supportMeshes[i]->getType()));
        writer.writeAttribute(QString("visible"), printModel->m_supportMeshes[i]->getVisuable() ? QString("true") : QString("false"));
        writer.writeAttribute(QString("meshID"), QString::number(printModel->m_supportMeshes[i]->getId()));
        const float* matData = printModel->m_supportMeshes[i]->getTransform().data();
        QString matDataStr = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14 %15 %16").
            arg(matData[0]).arg(matData[4]).arg(matData[8]).arg(matData[12]).
            arg(matData[1]).arg(matData[5]).arg(matData[9]).arg(matData[13]).
            arg(matData[2]).arg(matData[6]).arg(matData[10]).arg(matData[14]).
            arg(matData[3]).arg(matData[7]).arg(matData[11]).arg(matData[15]);
        writer.writeAttribute(QString("matrix44"), matDataStr);
        auto supportAssemblys = std::dynamic_pointer_cast<SupportAssemblyMeshes>(printModel->m_supportMeshes[i]);
        if (supportAssemblys != nullptr)
        {
            for (int j = 0; j < supportAssemblys->m_baseShowObjs.size(); j++)
            {
                writer.writeStartElement(QString("SupportPart"));
                writer.writeAttribute(QString("name"), supportAssemblys->m_baseShowObjs[j]->getObjectName());
                writer.writeAttribute(QString("type"), QString::number((int)supportAssemblys->m_baseShowObjs[j]->getType()));
                writer.writeAttribute(QString("visible"), supportAssemblys->m_baseShowObjs[j]->getVisuable() ? QString("true") : QString("false"));
                writer.writeAttribute(QString("meshID"), QString::number(supportAssemblys->m_baseShowObjs[j]->getId()));
                int tmpr, tmpg, tmpb, tmpa;
                supportAssemblys->m_baseShowObjs[j]->getColor().getRgb(&tmpr, &tmpg, &tmpb, &tmpa);
                QString tmpColor = QString("%1 %2 %3 %4").arg(tmpr).arg(tmpg).arg(tmpb).arg(tmpa);
                writer.writeAttribute(QString("meshColor"), tmpColor);

                const float* data = supportAssemblys->m_baseShowObjs[j]->getTransform().data();
                QString tmpMatStr = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14 %15 %16").
                    arg(data[0]).arg(data[4]).arg(data[8]).arg(data[12]).
                    arg(data[1]).arg(data[5]).arg(data[9]).arg(data[13]).
                    arg(data[2]).arg(data[6]).arg(data[10]).arg(data[14]).
                    arg(data[3]).arg(data[7]).arg(data[11]).arg(data[15]);
                writer.writeAttribute(QString("matrix44"), tmpMatStr);
                writer.writeEndElement();
            }
        }
        writer.writeEndElement();
    }
    writer.writeEndElement();
    writer.writeEndDocument();
    return true;
}

bool XmlStream::readSupportParamsXml2(CH3DPrintModelPtr& printModel, const QString& pathName)
{
    if (printModel == nullptr)
    {
        return false;
    }
    QFile file(pathName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Can't Open The File: " << pathName;
        return false;
    }
    QXmlStreamReader reader;
    reader.setDevice(&file);
    printModel->deleteAllSupportMeshes();
    printModel->m_supportMeshes.clear();
    while (!reader.atEnd())
    {
        if (reader.isStartElement())
        {
            QXmlStreamAttributes attributes = reader.attributes();
            if (attributes.hasAttribute(QString("meshName")))
            {
                printModel->setObjectName(attributes.value(QString("meshName")).toString());
                printModel->m_meshPath = attributes.value(QString("meshPath")).toString();
            }

            if (reader.name().toString() == QString("Support"))
            {
                SupportAssemblyMeshesPtr tmpSupportAssemblyPtr = SupportAssemblyMeshesPtr(new SupportAssemblyMeshes);
                tmpSupportAssemblyPtr->setParent(printModel.get());
                if (attributes.hasAttribute(QString("type")))
                {
                    tmpSupportAssemblyPtr->setType((SupportMesh::EMeshType)attributes.value(QString("type")).toInt());
                    tmpSupportAssemblyPtr->setId(attributes.value(QString("meshID")).toInt());
                    tmpSupportAssemblyPtr->setObjectName(attributes.value(QString("name")).toString());
                    tmpSupportAssemblyPtr->setVisuable(attributes.value(QString("visible")).toString().toLower() == QString("true"));
                    QMatrix4x4 tmpMat;
                    QStringList matStrList = attributes.value(QString("matrix44")).toString().trimmed().split(QString(" "));
                    if (matStrList.size() == 16)
                    {
                        tmpMat.setRow(0, QVector4D(matStrList[0].toDouble(), matStrList[1].toDouble(), matStrList[2].toDouble(), matStrList[3].toDouble()));
                        tmpMat.setRow(1, QVector4D(matStrList[4].toDouble(), matStrList[5].toDouble(), matStrList[6].toDouble(), matStrList[7].toDouble()));
                        tmpMat.setRow(2, QVector4D(matStrList[8].toDouble(), matStrList[9].toDouble(), matStrList[10].toDouble(), matStrList[11].toDouble()));
                        tmpMat.setRow(3, QVector4D(matStrList[12].toDouble(), matStrList[13].toDouble(), matStrList[14].toDouble(), matStrList[15].toDouble()));
                    }

                    tmpSupportAssemblyPtr->setTransform(tmpMat);
                    
                    tmpSupportAssemblyPtr->setLocalTransform(printModel->getTransform().inverted() * tmpSupportAssemblyPtr->getTransform());
                    tmpSupportAssemblyPtr->initSupportParams();
                    printModel->addSupportMesh(tmpSupportAssemblyPtr);
                }

                while (!reader.atEnd())
                {
                    if (reader.isEndElement() && reader.name().toString() == QString("Support"))
                    {
                        break;
                    }
                    if (reader.isStartElement())
                    {
                        if (reader.name().toString() == QString("SupportPart"))
                        {
                            QXmlStreamAttributes partAttributes = reader.attributes();
                            if (partAttributes.hasAttribute(QString("name")))
                            {
                                SupportMeshPtr tmpSupportPtr = nullptr;
                                createSupportType((SupportMesh::EMeshType)partAttributes.value(QString("type")).toInt(), tmpSupportPtr);
                                if (tmpSupportPtr == nullptr)
                                {
                                    return false;
                                }
                                tmpSupportPtr->setObjectName(partAttributes.value(QString("name")).toString());
                                tmpSupportPtr->setVisuable(partAttributes.value(QString("visible")).toString().toLower() == QString("true"));
                                tmpSupportPtr->setId(partAttributes.value(QString("meshID")).toInt());
                                QStringList colorList = partAttributes.value(QString("meshColor")).toString().split(QString(" "));
                                if (colorList.size() == 4)
                                {
                                    tmpSupportPtr->setColor(QColor(colorList[0].toInt(), colorList[1].toInt(), colorList[2].toInt(), colorList[3].toInt()));
                                }
                                QMatrix4x4 tmpMatrix;
                                QStringList matList = partAttributes.value(QString("matrix44")).toString().trimmed().split(QString(" "));
                                if (matList.size() == 16)
                                {
                                    tmpMatrix.setRow(0, QVector4D(matList[0].toDouble(), matList[1].toDouble(), matList[2].toDouble(), matList[3].toDouble()));
                                    tmpMatrix.setRow(1, QVector4D(matList[4].toDouble(), matList[5].toDouble(), matList[6].toDouble(), matList[7].toDouble()));
                                    tmpMatrix.setRow(2, QVector4D(matList[8].toDouble(), matList[9].toDouble(), matList[10].toDouble(), matList[11].toDouble()));
                                    tmpMatrix.setRow(3, QVector4D(matList[12].toDouble(), matList[13].toDouble(), matList[14].toDouble(), matList[15].toDouble()));
                                }
                                tmpSupportPtr->setTransform(tmpMatrix);
                                tmpSupportPtr->m_initTransform = tmpMatrix;
                                tmpSupportPtr->setLocalTransform(tmpSupportAssemblyPtr->getTransform().inverted() * tmpMatrix);
                                tmpSupportAssemblyPtr->append(tmpSupportPtr);
                            }
                        }

                    }

                    reader.readNext();
                }

            }
        }
        reader.readNext();
    }

    return true;
}

bool XmlStream::wirteSupportParamsXml(const CH3DPrintModelPtr& printModel, const QString& pathName)
{
    if (printModel == nullptr)
    {
        return false;
    }
    QString meshName = pathName;
    int index1 = meshName.lastIndexOf("/");
    meshName.chop(1);
    int index2 = meshName.lastIndexOf("/") + 1;
    meshName = pathName.mid(index2, index1 - index2);
    QString xmlName = pathName + meshName + QString(".xml");

    QFile file(xmlName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Error: Cannot Open File." << pathName;
        return false;
    }
    QXmlStreamWriter writer;
    writer.setDevice(&file);
    writer.setCodec("UTF-8");
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement(QString("Mesh"));
    writer.writeAttribute(QString("meshName"), meshName);
    writer.writeAttribute(QString("meshPath"), printModel->m_filePath);
    for (int i = 0; i < printModel->m_supportMeshes.size(); i++)
    {
        writer.writeStartElement(QString("Support"));
        writer.writeAttribute(QString("name"), printModel->m_supportMeshes[i]->getObjectName());
        writer.writeAttribute(QString("visible"), printModel->m_supportMeshes[i]->getVisuable() ? QString("true") : QString("false"));
        writer.writeAttribute(QString("meshID"), QString::number(printModel->m_supportMeshes[i]->getId()));
        int tmpr, tmpg, tmpb, tmpa;
        printModel->m_supportMeshes[i]->getColor().getRgb(&tmpr, &tmpg, &tmpb, &tmpa);
        QString tmpColor = QString("%1 %2 %3 %4").arg(tmpr).arg(tmpg).arg(tmpb).arg(tmpa);
        writer.writeAttribute(QString("meshColor"), tmpColor);

        const float* data = printModel->m_supportMeshes[i]->getTransform().data();
        QString tmpMatStr = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14 %15 %16").
            arg(data[0]).arg(data[4]).arg(data[8]).arg(data[12]).
            arg(data[1]).arg(data[5]).arg(data[9]).arg(data[13]).
            arg(data[2]).arg(data[6]).arg(data[10]).arg(data[14]).
            arg(data[3]).arg(data[7]).arg(data[11]).arg(data[15]);
        writer.writeAttribute(QString("matrix44"), tmpMatStr);
        writer.writeEndElement();
    }
    writer.writeEndDocument();
    return true;
}

bool XmlStream::readSupportParamsXml(CH3DPrintModelPtr& printModel, const QString& pathName)
{
    if (printModel == nullptr)
    {
        return false;
    }

    QFile file(pathName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Can't open the support file: " << pathName;
        return false;
    }
    QXmlStreamReader reader;
    reader.setDevice(&file);
    printModel->deleteAllSupportMeshes();
    printModel->m_supportMeshes.clear();
    while (!reader.atEnd())
    {
        if (reader.isStartElement())
        {
            QXmlStreamAttributes attributes = reader.attributes();
            if (attributes.hasAttribute(QString("meshName")))
            {
                printModel->setObjectName(attributes.value(QString("meshName")).toString());
                printModel->m_meshPath = attributes.value(QString("meshPath")).toString();
            }

            if (attributes.hasAttribute(QString("name")))
            {
                //CHSupportCuboidPtr tmpSupportPtr = CHSupportCuboidPtr(new CHSupportCuboid);
                //CHMeshShowObjPtr tmpSupportPtr = CHMeshShowObjPtr(new CHMeshShowObj);

                //CHCuboidPtr   tmpSupportPtr = CHCuboidPtr(new CHCuboid);
                SupportCuboidPtr   tmpSupportPtr = SupportCuboidPtr(new SupportCuboid());
                //SupportMeshPtr tmpSupportPtr = createSupportMesh();
                //tmpSupportPtr->deserialize()

                tmpSupportPtr->setObjectName(attributes.value(QString("name")).toString());
                tmpSupportPtr->setVisuable(attributes.value(QString("visible")).toString().toLower() == QString("true"));
                tmpSupportPtr->setId(attributes.value(QString("meshID")).toInt());
                QStringList colorList = attributes.value(QString("meshColor")).toString().split(QString(" "));
                if (colorList.size() == 4)
                {
                    tmpSupportPtr->setColor(QColor(colorList[0].toInt(), colorList[1].toInt(), colorList[2].toInt(), colorList[3].toInt()));
                }
                QMatrix4x4 tmpMatrix;
                QStringList matList = attributes.value(QString("matrix44")).toString().trimmed().split(QString(" "));
                if (matList.size() == 16)
                {
                    tmpMatrix.setRow(0, QVector4D(matList[0].toDouble(), matList[1].toDouble(), matList[2].toDouble(), matList[3].toDouble()));
                    tmpMatrix.setRow(1, QVector4D(matList[4].toDouble(), matList[5].toDouble(), matList[6].toDouble(), matList[7].toDouble()));
                    tmpMatrix.setRow(2, QVector4D(matList[8].toDouble(), matList[9].toDouble(), matList[10].toDouble(), matList[11].toDouble()));
                    tmpMatrix.setRow(3, QVector4D(matList[12].toDouble(), matList[13].toDouble(), matList[14].toDouble(), matList[15].toDouble()));
                }

                tmpSupportPtr->setTransform(tmpMatrix);
                printModel->m_supportMeshes.append(tmpSupportPtr);
            }
        }
        reader.readNext();
    }
    return true;
}


SupportMeshPtr XmlStream::createSupportMesh(SupportMesh::EMeshType meshType)
{
    switch (meshType)
    {
    case SupportMesh::EMeshType::AssemblyMeshes:
    {
        return SupportAssemblyMeshesPtr(new SupportAssemblyMeshes());
    }
    case SupportMesh::EMeshType::Cuboid:
    {
        return SupportCuboidPtr(new SupportCuboid());
    }
    case SupportMesh::EMeshType::Cone:
    {
        return SupportConePtr(new SupportCone());
    }
    case SupportMesh::EMeshType::Cylinder:
    {
        return SupportCylinderPtr(new SupportCylinder());
    }
    case SupportMesh::EMeshType::Sphere:
    {
        return SupportSpherePtr(new SupportSphere());
    }
    case SupportMesh::EMeshType::Tetrahedron:
    {
        return SupportTetrahedronPtr(new SupportTetrahedron());
    }
    case SupportMesh::EMeshType::TrapezoidBody:
    {
        return SupportTrapezoidBodyPtr(new SupportTrapezoidBody());
    }
    }
}

void XmlStream::writeDocumentXmlElement()
{
    m_writer.writeStartElement(QString("Document"));
    m_writer.writeAttribute(QString("docName"), m_file_prop->m_docPtr->objectName());
    m_writer.writeAttribute(QString("docPath"), m_file_prop->m_docPtr->m_path);

    auto it = m_file_prop->m_docPtr->m_printObjs.begin();// m_file_prop->m_document_property.models.begin();
    while (it != m_file_prop->m_docPtr->m_printObjs.end())
    {
        m_writer.writeStartElement(QString("MeshProperty"));
        int lastDotIndex = (*it)->getObjectName().lastIndexOf(QString("."));
        QString tmpMeshName = (*it)->getObjectName().mid(0, lastDotIndex);
        m_writer.writeAttribute(QString("meshName"), tmpMeshName);
        m_writer.writeAttribute(QString("visible"), (*it)->getVisuable() ? QString("true") : QString("false"));
        m_writer.writeAttribute(QString("meshId"), QString::number((*it)->getId()));
        m_writer.writeAttribute(QString("meshPath"), tmpMeshName + QString("/"));
        m_writer.writeAttribute(QString("meshColor"), QString("%1 %2 %3 %4").
            arg((*it)->getColor().red()).arg((*it)->getColor().green()).arg((*it)->getColor().blue()).arg((*it)->getColor().alpha()));

        qDebug() << "Write Transform: " << (*it)->getTransform();
        const float* data = (*it)->getTransform().data();
        QString tmpMatStr = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14 %15 %16").
            arg(data[0]).arg(data[4]).arg(data[8]).arg(data[12]).
            arg(data[1]).arg(data[5]).arg(data[9]).arg(data[13]).
            arg(data[2]).arg(data[6]).arg(data[10]).arg(data[14]).
            arg(data[3]).arg(data[7]).arg(data[11]).arg(data[15]);
        m_writer.writeAttribute(QString("matrix44"), tmpMatStr);

        const float* iniData = (*it)->m_initTransform.data();
        QString tmpInitMatStr = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10 %11 %12 %13 %14 %15 %16").
                arg(iniData[0]).arg(iniData[4]).arg(iniData[8]).arg(iniData[12]).
                arg(iniData[1]).arg(iniData[5]).arg(iniData[9]).arg(iniData[13]).
                arg(iniData[2]).arg(iniData[6]).arg(iniData[10]).arg(iniData[14]).
                arg(iniData[3]).arg(iniData[7]).arg(iniData[11]).arg(iniData[15]);
            m_writer.writeAttribute(QString("iniMatrix44"), tmpInitMatStr);

        m_writer.writeAttribute(QString("showMode"), QString::number((int)(*it)->m_showMode));

        QString tmpRotCenter = QString("%1 %2 %3").arg((*it)->m_rotCenter[0]).arg((*it)->m_rotCenter[1]).arg((*it)->m_rotCenter[2]);
        m_writer.writeAttribute(QString("rotCenter"), tmpRotCenter);

        std::vector<float> tmpParams = (*it)->m_params;
        if (tmpParams.size() == 9)
        {
            QString tmpParamsStr = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9").
                arg(tmpParams[0]).arg(tmpParams[1]).arg(tmpParams[2]).
                arg(tmpParams[3]).arg(tmpParams[4]).arg(tmpParams[5]).
                arg(tmpParams[6]).arg(tmpParams[7]).arg(tmpParams[8]);
            m_writer.writeAttribute(QString("params"), tmpParamsStr);
        }

        std::vector<float> initParams = (*it)->m_initParams;
        if (initParams.size() == 9)
        {
            QString tmpIniParamsStr = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9").
                arg(initParams[0]).arg(initParams[1]).arg(initParams[2]).
                arg(initParams[3]).arg(initParams[4]).arg(initParams[5]).
                arg(initParams[6]).arg(initParams[7]).arg(initParams[8]);
            m_writer.writeAttribute(QString("iniParams"), tmpIniParamsStr);
        }

        QString allSupportsName = "";
        for (int i = 0; i < (*it)->m_supportMeshes.size(); i++)
        {
            allSupportsName += (*it)->m_supportMeshes[i]->getObjectName() + QString("_") + QString::number((*it)->m_supportMeshes[i]->getId()) + QString(" ");
        }
        m_writer.writeAttribute(QString("support"), allSupportsName);
        m_writer.writeEndElement();
        ++it;
    }

    m_writer.writeEndElement();
}
bool XmlStream::readDocumentXmlElement()
{
    if (m_file_prop->m_docPtr == nullptr)
    {
        return false;
    }
    m_file_prop->m_docPtr->clearDocMeshModels();

    m_file_prop->m_docPtr->m_path = "";
    m_file_prop->m_docPtr->setObjectName("");
    while (!m_reader.atEnd())
    {
        //????
        if (m_reader.isStartElement())
        {
            QXmlStreamAttributes attributes = m_reader.attributes();
            if (attributes.hasAttribute(QString("docName")))
            {
                m_file_prop->m_docPtr->setObjectName(attributes.value(QString("docName")).toString());
                m_file_prop->m_docPtr->m_path = attributes.value(QString("docPath")).toString();
            }


            if(attributes.hasAttribute(QString("meshName")))
            {
                CH3DPrintModelPtr meshObjPtr = CH3DPrintModelPtr(new CH3DPrintModel);
                meshObjPtr->setObjectName(attributes.value(QString("meshName")).toString());
                //qDebug() << "meshName: " << attributes.value(QString("meshName")).toString();
                meshObjPtr->setVisuable(attributes.value(QString("visible")).toString().toLower() == QString("true"));
                meshObjPtr->setId(attributes.value(QString("meshId")).toString().toInt());
                meshObjPtr->m_meshPath = attributes.value(QString("meshPath")).toString();
                QStringList colorList = attributes.value(QString("meshColor")).toString().split(QString(" "));
                meshObjPtr->setColor(QColor(colorList[0].toInt(), colorList[1].toInt(), colorList[2].toInt(), colorList[3].toInt()));
                QMatrix4x4 tmpMatrix;
                QStringList matList = attributes.value(QString("matrix44")).toString().trimmed().split(QString(" "));
                if (matList.size() == 16)
                {
                    tmpMatrix.setRow(0, QVector4D(matList[0].toDouble(), matList[1].toDouble(), matList[2].toDouble(), matList[3].toDouble()));
                    tmpMatrix.setRow(1, QVector4D(matList[4].toDouble(), matList[5].toDouble(), matList[6].toDouble(), matList[7].toDouble()));
                    tmpMatrix.setRow(2, QVector4D(matList[8].toDouble(), matList[9].toDouble(), matList[10].toDouble(), matList[11].toDouble()));
                    tmpMatrix.setRow(3, QVector4D(matList[12].toDouble(), matList[13].toDouble(), matList[14].toDouble(), matList[15].toDouble()));
                }
                meshObjPtr->setTransform(tmpMatrix);
                qDebug() << "Read Matrix: " << tmpMatrix;

                QMatrix4x4 tmpIniMatrix;
                QStringList intMatList = attributes.value(QString("iniMatrix44")).toString().trimmed().split(QString(" "));
                if (intMatList.size() == 16)
                {
                    tmpIniMatrix.setRow(0, QVector4D(intMatList[0].toDouble(), intMatList[1].toDouble(), intMatList[2].toDouble(), intMatList[3].toDouble()));
                    tmpIniMatrix.setRow(1, QVector4D(intMatList[4].toDouble(), intMatList[5].toDouble(), intMatList[6].toDouble(), intMatList[7].toDouble()));
                    tmpIniMatrix.setRow(2, QVector4D(intMatList[8].toDouble(), intMatList[9].toDouble(), intMatList[10].toDouble(), intMatList[11].toDouble()));
                    tmpIniMatrix.setRow(3, QVector4D(intMatList[12].toDouble(), intMatList[13].toDouble(), intMatList[14].toDouble(), intMatList[15].toDouble()));
                }
                meshObjPtr->m_initTransform = tmpIniMatrix;


                int tmp = attributes.value(QString("showMode")).toInt();
                meshObjPtr->m_showMode = ShowMode(tmp);

                QStringList rotCenterList = attributes.value(QString("rotCenter")).toString().trimmed().split(QString(" "));
                if (rotCenterList.size() == 3)
                {
                    meshObjPtr->m_rotCenter[0] = rotCenterList[0].toDouble();
                    meshObjPtr->m_rotCenter[1] = rotCenterList[1].toDouble();
                    meshObjPtr->m_rotCenter[2] = rotCenterList[2].toDouble();
                }

                QStringList paramsList = attributes.value(QString("params")).toString().trimmed().split(QString(" "));
                if (paramsList.size() == 9)
                {
                    meshObjPtr->m_params.clear();

                    for (int i = 0; i < 9; i++)
                    {
                        meshObjPtr->m_params.push_back(paramsList[i].toFloat());
                    }
                }

                QStringList iniParamsList = attributes.value(QString("iniParams")).toString().trimmed().split(QString(" "));
                if (iniParamsList.size() == 9)
                {
                    meshObjPtr->m_initParams.clear();

                    for (int i = 0; i < 9; i++)
                    {
                        meshObjPtr->m_initParams.push_back(iniParamsList[i].toFloat());
                    }
                }


                /*QStringList allSupportNamesList = attributes.value(QString("support")).toString().trimmed().split(QString(" "));
                if (allSupportNamesList.size() > 0)
                {
                    meshObjPtr->deleteAllSupportMeshes();
                    meshObjPtr->m_supportMeshes.clear();
                    for (int i = 0; i < allSupportNamesList.size(); i++)
                    {
                        CH3DPrintModelPtr supportModel = CH3DPrintModelPtr(new CH3DPrintModel);
                        int index = allSupportNamesList[i].lastIndexOf("_");
                        QString tmpName = allSupportNamesList[i].mid(0, index);
                        index++;
                        int tmpId = allSupportNamesList[i].mid(index, allSupportNamesList[i].length() - index).toInt();
                        supportModel->setObjectName(tmpName);
                        supportModel->setId(tmpId);
                        meshObjPtr->m_supportMeshes.push_back(supportModel);
                    }

                }  */
                m_file_prop->m_docPtr->appendObj(meshObjPtr);
                // m_file_prop->m_docPtr->m_printObjs.push_back(meshObjPtr);
            }

        } m_reader.readNext();
    }

    return true;
}

void XmlStream::writeViewXmlElement()
{
    m_writer.writeStartElement(QString("View"));
    m_writer.writeAttribute(QString("viewName"), QString(m_file_prop->m_view_property.viewName.c_str()));
    m_writer.writeStartElement(QString("SceneProperty"));
    m_writer.writeAttribute(QString("verticalAngle"), QString::number(m_file_prop->m_view_property.m_scene_property.m_verticalAngle, 10, 4));
    m_writer.writeAttribute(QString("senseRadius"), QString::number(m_file_prop->m_view_property.m_scene_property.senseRadius, 10, 4));
    QVector3D tmpVec = m_file_prop->m_view_property.m_scene_property.camera.m_eye;
    QString tmpVecStr = QString("%1 %2 %3").arg(tmpVec[0]).arg(tmpVec[1]).arg(tmpVec[2]);
    m_writer.writeAttribute(QString("eye"), QString(tmpVecStr.toStdString().c_str()));
    tmpVec = m_file_prop->m_view_property.m_scene_property.camera.m_up;
    tmpVecStr = QString("%1 %2 %3").arg(tmpVec[0]).arg(tmpVec[1]).arg(tmpVec[2]);
    m_writer.writeAttribute(QString("up"), QString(tmpVecStr.toStdString().c_str()));
    tmpVec = m_file_prop->m_view_property.m_scene_property.camera.m_front;
    tmpVecStr = QString("%1 %2 %3").arg(tmpVec[0]).arg(tmpVec[1]).arg(tmpVec[2]);
    m_writer.writeAttribute(QString("front"), QString(tmpVecStr.toStdString().c_str()));
    m_writer.writeEndElement();
    m_writer.writeEndElement();
}
bool XmlStream::readViewXmlElement()
{
    m_file_prop->m_view_property.clear();

    while (!m_reader.atEnd())
    {
        //????
        if (m_reader.isStartElement())
        {
            QXmlStreamAttributes attributes = m_reader.attributes();
            if (attributes.hasAttribute(QString("viewName")))
            {
                m_file_prop->m_view_property.viewName = attributes.value(QString("viewName")).toString().toStdString();
            }
            else if (attributes.hasAttribute(QString("verticalAngle")))
            {
                m_file_prop->m_view_property.m_scene_property.m_verticalAngle = attributes.value(QString("verticalAngle")).toDouble();
                m_file_prop->m_view_property.m_scene_property.senseRadius = attributes.value(QString("senseRadius")).toDouble();
                QStringList tmpVecList = attributes.value(QString("eye")).toString().trimmed().split(QString(" "));
                if (tmpVecList.size() != 3)
                    return false;
                m_file_prop->m_view_property.m_scene_property.camera.m_eye = QVector3D(tmpVecList[0].toDouble(), tmpVecList[1].toDouble(), tmpVecList[2].toDouble());
                tmpVecList = attributes.value(QString("up")).toString().trimmed().split(QString(" "));
                if (tmpVecList.size() != 3)
                    return false;
                m_file_prop->m_view_property.m_scene_property.camera.m_up = QVector3D(tmpVecList[0].toDouble(), tmpVecList[1].toDouble(), tmpVecList[2].toDouble());
                tmpVecList = attributes.value(QString("front")).toString().trimmed().split(QString(" "));
                if (tmpVecList.size() != 3)
                    return false;
                m_file_prop->m_view_property.m_scene_property.camera.m_front = QVector3D(tmpVecList[0].toDouble(), tmpVecList[1].toDouble(), tmpVecList[2].toDouble());
            }
        }

        m_reader.readNext();
    }
    return true;
}

void XmlStream::writePluginPamasXmlElement()
{

}

bool XmlStream::readPluginPamasXmlElement()
{
    return true;
}

void XmlStream::writeModelFolderXmlElement()
{

}

bool XmlStream::readModelFolderXmlElement()
{
    return true;
}

void XmlStream::writeSupportParamsXmlElement()
{
    m_writer.writeStartElement(QString("Support"));

    /*auto it = m_file_prop->m_docPtr->m_printObjs.begin();
    while (it != m_file_prop->m_docPtr->m_printObjs.end())
    {
    }*/




    m_writer.writeEndElement();
}

bool XmlStream::readSupportParamsXmlElement()
{
    return false;
}

bool XmlStream::readAppXmlElement()
{
    m_file_prop->m_appParams_property.clear();
    while (!m_reader.atEnd())
    {
        //????
        QXmlStreamAttributes attributes = m_reader.attributes();
        if (attributes.hasAttribute(QString("Version")))
        {
            m_file_prop->m_appParams_property.m_version = attributes.value(QString("Version")).toString().toStdString();
        }
        else if (attributes.hasAttribute(QString("Language")))
        {
            m_file_prop->m_appParams_property.m_language = (AppParamsProperty::LanguageType)attributes.value(QString("Language")).toString().toInt();
        }
        else if (attributes.hasAttribute(QString("RecentFile")))
        {
            m_file_prop->m_appParams_property.m_recentFileList.push_back(attributes.value(QString("RecentFile")).toString().toStdString());
        }
        m_reader.readNext();
    }

    return true;
}

void XmlStream::writeAppXmlElement()
{
    m_writer.writeStartElement(QString("Application"));
    m_writer.writeAttribute(QString("Version"), QString(m_file_prop->m_appParams_property.m_version.c_str()));

    m_writer.writeStartElement(QString("Property"));
    m_writer.writeAttribute(QString("Language"), QString::number(m_file_prop->m_appParams_property.m_language));
    m_writer.writeEndElement();

    auto it = m_file_prop->m_appParams_property.m_recentFileList.begin();
    while (it != m_file_prop->m_appParams_property.m_recentFileList.end())
    {
        m_writer.writeStartElement(QString("Property"));
        m_writer.writeAttribute(QString("RecentFile"), QString((*it).c_str()));
        m_writer.writeEndElement();
        ++it;
    }
    m_writer.writeEndElement();
}

bool XmlStream::readFile()
{
    bool result = false;
    if (m_file_prop == NULL)
    {
        qDebug() << "Get File Property Failed.";
        return result;
    }
    QFile file(m_fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Can't Open the File: " << m_fileName;
        return result;
    }
    m_reader.setDevice(&file);

    switch (m_type)
    {
    case XmlStream::XmlStreamType_App:
        result = readAppXmlElement();
        break;
    case XmlStream::XmlStreamType_Document:
        result = readDocumentXmlElement();
        break;
    case XmlStream::XmlStreamType_View:
        result = readViewXmlElement();
        break;
    case XmlStream::XmlStreamType_PluginPamas:
        result = readPluginPamasXmlElement();
        break;
    case XmlStream::XmlStreamType_ModelFolder_Transform:
        result = readModelFolderXmlElement();
        break;
    case XmlStream::XmlStreamType_Support:
        result = readSupportParamsXmlElement();
        break;
    default:
        break;
    }

    file.close();
    if (m_reader.hasError())
    {
        qDebug() << "Parse File " << m_fileName << " Failed.";
        result = false;
        return result;
    }
    else if (file.error() != QFile::NoError)
    {
        qDebug() << "Can't Read File "<< m_fileName;
        result = false;
        return result;
    }
    qDebug() << "File Parse Success!";

    return result;
}



bool XmlStream::writeFile()
{
    if (m_file_prop == NULL)
    {
        qDebug() << "Get File Property Failed.";
        return false;
    }
    QFile file(m_fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Error: Cannot Open File " << m_fileName;
        return false;
    }
    m_writer.setDevice(&file);
    m_writer.setCodec("UTF-8");
    m_writer.setAutoFormatting(true);
    m_writer.writeStartDocument();
    switch (m_type)
    {
    case XmlStream::XmlStreamType_App:
        writeAppXmlElement();
        break;
    case XmlStream::XmlStreamType_Document:
        writeDocumentXmlElement();
        break;
    case XmlStream::XmlStreamType_View:
        writeViewXmlElement();
        break;
    case XmlStream::XmlStreamType_PluginPamas:
        writePluginPamasXmlElement();
        break;
    case XmlStream::XmlStreamType_ModelFolder_Transform:
        writeModelFolderXmlElement();
        break;
    case XmlStream::XmlStreamType_Support:
        writeSupportParamsXmlElement();
        break;
    default:
        break;
    }
    m_writer.writeEndDocument();
    return true;
}

#include "CHDoc.h"
#include "CHBaseAlg.h"
#include "qregularexpression.h"
#include <QFile>
#include <QDir>
#include <QDebug>
#include "support/SupportAssemblyMeshes.h"
#include "support/SupportMesh.h"

CHDoc::CHDoc()
{
    float lenth = 1.0 / 4.0;
    m_showPickedAABB = CHAssemblyPtr(new CHAssembly);
    std::vector<CHLineSegment3DShowObjPtr> lines(24);
    for (int i = 0; i < 24; i++)
    {
        lines[i] = CHLineSegment3DShowObjPtr(new CHLineSegment3DShowObj);
    }
    lines[0]->create(QVector3D(0, 0, 0), QVector3D(lenth, 0, 0));
    lines[1]->create(QVector3D(0, 0, 0), QVector3D(0, lenth, 0));
    lines[2]->create(QVector3D(0, 0, 0), QVector3D(0, 0, lenth));
    lines[3]->create(QVector3D(1, 0, 0), QVector3D(1 - lenth, 0, 0));
    lines[4]->create(QVector3D(1, 0, 0), QVector3D(1, lenth, 0));
    lines[5]->create(QVector3D(1, 0, 0), QVector3D(1, 0, lenth));
    lines[6]->create(QVector3D(1, 0, 1), QVector3D(1 - lenth, 0, 1));
    lines[7]->create(QVector3D(1, 0, 1), QVector3D(1, lenth, 1));
    lines[8]->create(QVector3D(1, 0, 1), QVector3D(1, 0, 1 - lenth));
    lines[9]->create(QVector3D(0, 0, 1), QVector3D(lenth, 0, 1));
    lines[10]->create(QVector3D(0, 0, 1), QVector3D(0, lenth, 1));
    lines[11]->create(QVector3D(0, 0, 1), QVector3D(0, 0, 1 - lenth));

    lines[12]->create(QVector3D(0, 1, 0), QVector3D(lenth, 1, 0));
    lines[13]->create(QVector3D(0, 1, 0), QVector3D(0, 1 - lenth, 0));
    lines[14]->create(QVector3D(0, 1, 0), QVector3D(0, 1, lenth));
    lines[15]->create(QVector3D(1, 1, 0), QVector3D(1 - lenth, 1, 0));
    lines[16]->create(QVector3D(1, 1, 0), QVector3D(1, 1 - lenth, 0));
    lines[17]->create(QVector3D(1, 1, 0), QVector3D(1, 1, lenth));
    lines[18]->create(QVector3D(1, 1, 1), QVector3D(1 - lenth, 1, 1));
    lines[19]->create(QVector3D(1, 1, 1), QVector3D(1, 1 - lenth, 1));
    lines[20]->create(QVector3D(1, 1, 1), QVector3D(1, 1, 1 - lenth));
    lines[21]->create(QVector3D(0, 1, 1), QVector3D(lenth, 1, 1));
    lines[22]->create(QVector3D(0, 1, 1), QVector3D(0, 1 - lenth, 1));
    lines[23]->create(QVector3D(0, 1, 1), QVector3D(0, 1, 1 - lenth));
    m_showPickedAABB->m_baseShowObjs.resize(24);
    for (int i = 0; i < 24; i++)
    {
        m_showPickedAABB->m_baseShowObjs[i] = lines[i];
    }
    m_showPickedAABB->setColor(QColor(97, 211, 125));
    connect(this, SIGNAL(modelCheckSceneInChanged()), this, SLOT(modelCheckSceneIn()));
}

CHDoc::~CHDoc()
{

}
//
//bool CHDoc::writeVisibleMeshesToStlFile(const QString& path, bool binary)
//{
//    qDebug() << "enter writeVisibleMeshesToStlFile.size is ";
//    if (m_printObjs.size() == 0)
//    {
//        return false;
//    }
//
//    std::vector<CH3DPrintModelPtr> visuableObjs;


//    for (int i = 0; i < m_printObjs.size(); i++)
//    {
//        if (m_printObjs[i]->getVisuable())
//        {
//            visuableObjs.push_back(m_printObjs[i]);
//
//            meshes.push_back(m_printObjs[i]);
//            supportMeshes.insert(supportMeshes.end(), m_printObjs[i]->m_supportMeshes.begin(), m_printObjs[i]->m_supportMeshes.end());
//        }
//    }
//    if (visuableObjs.size() == 0)
//    {
//        return false;
//    }
//

//    CHBaseAlg::instance()->writeMeshesToStlFile(meshes, path, binary);
//

//    QFileInfo meshInfo(path);
//    //    auto sptDir = fInfo.absoluteDir().absoluteFilePath("support");
//    //    QDir supportDir(sptDir);
//    //    if (!supportDir.exists())
//    //    {
//    //        supportDir.mkpath(sptDir);
//    //    }
//    qDebug() << "supportMeshes.size is " << supportMeshes.size();
//    std::vector<CHMeshShowObjPtr> singleMeshes;
//    for (int i = 0; i < supportMeshes.size(); i++)
//    {

//        singleMeshes.clear();
//        SupportAssemblyMeshesPtr assemblySupport = std::dynamic_pointer_cast<SupportAssemblyMeshes>(supportMeshes[i]);
//        if (nullptr != assemblySupport)
//        {
//            for (int j = 0; j < assemblySupport->m_baseShowObjs.size(); j++)
//            {
//                singleMeshes.push_back(assemblySupport->m_baseShowObjs[j]);
//            }
//        }
//        else
//        {
//            singleMeshes.push_back(supportMeshes[i]);
//        }
//

//        for (int k = 0; k < singleMeshes.size(); k++)
//        {
//            auto sptMeshPath = meshInfo.absoluteDir().absoluteFilePath(QString("sptMesh_%1_%2.stl").arg(i).arg(k));
//            CHBaseAlg::instance()->writeMeshesToStlFile(singleMeshes[k], sptMeshPath, binary);
//        }
//    }
//
//   /* std::vector<CHMeshShowObjPtr> singleMesh;
//    for (int i = 0; i < supportMeshes.size(); i++)
//    {
//        singleMesh.clear();
//
//        singleMesh.push_back(supportMeshes[i]);
//        auto sptMeshPath = meshInfo.absoluteDir().absoluteFilePath(QString("sptMesh_%1.stl").arg(i));
//        CHBaseAlg::instance()->writeMeshesToStlFile(singleMesh, sptMeshPath, binary);
//    }*/
//    return true;
//}


bool CHDoc::writeVisibleMeshesToStlFile(const QString& path, bool binary)
{
    if (m_printObjs.size() == 0)
    {
        return false;
    }

    std::vector<CH3DPrintModelPtr> visuableObjs;
    std::vector<CHMeshShowObjPtr> meshes;
    std::vector<SupportMeshPtr> supportMeshes;
    for (int i = 0; i < m_printObjs.size(); i++)
    {
        if (m_printObjs[i]->getVisuable())
        {
            visuableObjs.push_back(m_printObjs[i]);

            meshes.push_back(m_printObjs[i]);
            supportMeshes.insert(supportMeshes.end(), m_printObjs[i]->m_supportMeshes.begin(), m_printObjs[i]->m_supportMeshes.end());
        }
    }
    if (visuableObjs.size() == 0)
    {
        return false;
    }

    
    CHBaseAlg::instance()->writeMeshesToStlFile(meshes, path, binary);

    
    QFileInfo meshInfo(path);

    qDebug() << "supportMeshes.size is " << supportMeshes.size();
    std::vector<CHMeshShowObjPtr> singleMeshes;
    for (int i = 0; i < supportMeshes.size(); i++)
    {
        SupportAssemblyMeshesPtr assemblySupport = std::dynamic_pointer_cast<SupportAssemblyMeshes>(supportMeshes[i]);
        if (nullptr != assemblySupport)
        {
            for (int j = 0; j < assemblySupport->m_baseShowObjs.size(); j++)
            {
                singleMeshes.push_back(assemblySupport->m_baseShowObjs[j]);
            }
        }
        else
        {
            singleMeshes.push_back(supportMeshes[i]);
        }
    }

    auto sptMeshPath = meshInfo.absoluteDir().absoluteFilePath(QString("sptMesh_assemblySptMesh.stl"));
    CHBaseAlg::instance()->writeMeshesToStlFile(singleMeshes, sptMeshPath, binary);
    
    return true;
}
CH3DPrintModelPtr CHDoc::addNewMesh(const QString& fileName, const QString& infoFileName)
{
    return CH3DPrintModelPtr();
}

bool CHDoc::writeAllMeshModelsToStlFile(const QString& path, bool binary)
{
    if (m_printObjs.size() == 0)
    {
        return false;
    }

    std::vector<CH3DPrintModelPtr> visuableObjs;
    std::vector<CHMeshShowObjPtr> meshes;
    for (int i = 0; i < m_printObjs.size(); i++)
    {
        if (m_printObjs[i]->getVisuable())
        {
            visuableObjs.push_back(m_printObjs[i]);

            meshes.push_back(m_printObjs[i]);
            meshes.insert(meshes.end(), m_printObjs[i]->m_supportMeshes.begin(), m_printObjs[i]->m_supportMeshes.end());
        }
    }
    if (visuableObjs.size() == 0)
    {
        return false;
    }

    
    return CHBaseAlg::instance()->writeMeshesToStlFile(meshes, path, binary);

    FILE* fp = 0;
    //fp = fopen(path.toStdString().c_str(), "wb");
    fp = fopen(QFile::encodeName(path).constData(), "wb"); 
    if (fp == 0)
    {
        return false;
    }

    if (binary)
    {
        // Write Header
        char header[128] = "VCG                                                                                                  ";

        fwrite(header, 80, 1, fp);
        // write number of facets
        int faceSum = 0;
        for (int i = 0; i < visuableObjs.size(); i++)
        {
            faceSum += visuableObjs[i]->m_oldMesh->fn;
        }
        fwrite(&faceSum, 1, sizeof(int), fp);

        for (int i = 0; i < visuableObjs.size(); i++)
        {
            m_printObjs[i]->m_oldMesh->Tr = Matrix44m(m_printObjs[i]->getTransform().transposed().data());
            Matrix44m norTran = vcg::Inverse(m_printObjs[i]->m_oldMesh->Tr).transpose();

            vcg::Point3f p;
            unsigned short attributes = 0;
            for (vcgTriMesh::FaceIterator fi = visuableObjs[i]->m_oldMesh->face.begin(); fi != visuableObjs[i]->m_oldMesh->face.end(); ++fi)
            {
                if (!(*fi).IsD())
                {
                    // For each triangle write the normal, the three coords and a short set to zero
                    p.Import(norTran * vcg::TriangleNormal(*fi).Normalize());
                    fwrite(p.V(), 3, sizeof(float), fp);

                    for (int k = 0; k < 3; ++k)
                    {
                        p.Import(visuableObjs[i]->m_oldMesh->Tr * ((*fi).V(k)->P()));
                        fwrite(p.V(), 3, sizeof(float), fp);
                    }

                    fwrite(&attributes, 1, sizeof(short), fp);
                }
            }
        }
    }
    else
    {
        fprintf(fp, "solid vcg\n");

        for (int i = 0; i < visuableObjs.size(); i++)
        {
            visuableObjs[i]->m_oldMesh->Tr = Matrix44m(visuableObjs[i]->getTransform().transposed().data());
            Matrix44m norTran = vcg::Inverse(visuableObjs[i]->m_oldMesh->Tr).transpose();

            vcg::Point3f p;
            vcgTriMesh::FaceIterator fi;
            for (fi = visuableObjs[i]->m_oldMesh->face.begin(); fi != visuableObjs[i]->m_oldMesh->face.end(); ++fi)
            {
                if (!(*fi).IsD())
                {
                    // For each triangle write the normal, the three coords and a short set to zero
                    p.Import(norTran * TriangleNormal(*fi).Normalize());
                    fprintf(fp, "  facet normal %13e %13e %13e\n", p[0], p[1], p[2]);
                    fprintf(fp, "    outer loop\n");
                    for (int k = 0; k < 3; ++k)
                    {
                        p.Import(visuableObjs[i]->m_oldMesh->Tr * ((*fi).V(k)->P()));
                        fprintf(fp, "      vertex  %13e %13e %13e\n", p[0], p[1], p[2]);
                    }
                    fprintf(fp, "    endloop\n");
                    fprintf(fp, "  endfacet\n");
                }
            }
        }

        fprintf(fp, "endsolid vcg\n");
    }

    bool result = true;
    if (ferror(fp))
    {
        result = false;
    }
    fclose(fp);
    return result;
}

void CHDoc::clearDocMeshModels()

{
    for (int i = 0; i < m_printObjs.size(); i++)
    {
        m_printObjs[i]->setDelete(true);
        m_printObjs[i]->updateToScene();
    }
    m_printObjs.clear();

    emit modelObjsStatusChanged(ModelStatusChangedType::ClearDoc);
    calcVisibleModelCount();
    
    modelCheckSceneIn();
}

void CHDoc::resetAllMeshModels()
{
//    for (int i = 0; i < m_printObjs.size(); i++)
//    {
//        m_printObjs[i]->resetTransform();
//    }
    resetObjs(m_printObjs);
    //emit modelObjsStatusChanged();
}

void CHDoc::deleteObjs(std::vector<CH3DPrintModelPtr> objs)
{
    for (int p = 0; p < objs.size(); p++)
    {
        for (int i = 0; i < m_printObjs.size(); i++)
        {
            if (objs[p] == m_printObjs[i])
            {
                if(m_printObjs[i]->m_supportMeshes.size() > 0)
                {
                   for(int k = 0; k < m_printObjs[i]->m_supportMeshes.size(); k++)
                   {
                       m_printObjs[i]->m_supportMeshes[k]->setDelete(true);
                       m_printObjs[i]->m_supportMeshes[k]->updateToScene();
                       SupportMesh* tmpSupport = nullptr;
                       m_printObjs[i]->m_supportMeshes[k].reset(tmpSupport);
                   }
                   m_printObjs[i]->m_supportMeshes.clear();
                }
                m_printObjs[i]->setDelete(true);
                m_printObjs[i]->updateToScene();
                CH3DPrintModel* tmpModel = nullptr;
                m_printObjs[i].reset(tmpModel);
                m_printObjs.erase(m_printObjs.begin() + i);
                break;
            }
        }
    }

    emit modelObjsStatusChanged(ModelStatusChangedType::DeleteMesh);
    emit modelCheckSceneInChanged();
    calcVisibleModelCount();
}

void CHDoc::deleteObj(CH3DPrintModelPtr obj)
{
    std::vector<CH3DPrintModelPtr> objs;
    objs.push_back(obj);
    deleteObjs(objs);
}

void CHDoc::addObjs(std::vector<CH3DPrintModelPtr> objs)

{
    
    for (int i = 0; i < objs.size(); i++)
    {
        CH3DPrintModelPtr ttt = objs[i];

        
        
       
        //ttt->setObjectName(originName);
        //int num = 0;
       
//        QString ext, pureName;

//        {
//            pureName = originName;
//        }
//        else
//        {

//            pureName = originName.left(location1);
//        }
        
        /*QRegularExpression reg(QString::fromLocal8Bit("\\(\\d+\\)$"));
        QRegularExpressionMatch match = reg.match(pureName);
        if (match.hasMatch())
        {
            pureName = pureName.left(match.capturedStart(0));
        }*/
        int num = 0;
        QString meshName = ttt->getObjectName();
        int index1 = meshName.indexOf("(");
        int index2 = meshName.indexOf(")");
        if(index1 > 0 && index2 > 0)
        {
           meshName = meshName.mid(0, index1);
        }
        for(int p = 0; p < m_printObjs.size(); p++)
        {
            if(m_printObjs[p]->getObjectName().contains(meshName))
            {
                num++;
                ttt->setObjectName(meshName + QString("(") + QString::number(num) + QString(")"));
            }
        }
//        while (true)
//        {
//            bool flag = true;
//            for (int p = 0; p < m_printObjs.size(); p++)
//            {
//                if (ttt->getObjectName() == m_printObjs[p]->getObjectName())
//                {
//                    num++;

//                    ttt->setObjectName(ttt->getObjectName() + QString("(") + QString::number(num) + QString(")"));
//                    flag = false;
//                    break;
//                }
//            }

//            if (flag)
//            {
//                break;
//            }
//        }
        m_printObjs.push_back(ttt);
    }

    emit modelObjsStatusChanged(ModelStatusChangedType::AddMesh);
    calcVisibleModelCount();
    
    modelCheckSceneIn();
}

void CHDoc::addObj(CH3DPrintModelPtr obj)
{
    std::vector<CH3DPrintModelPtr> objs;
    objs.push_back(obj);
    addObjs(objs);
}

void CHDoc::appendObj(CH3DPrintModelPtr obj)
{
    m_printObjs.push_back(obj);
    calcVisibleModelCount();
}

void CHDoc::setObjsVisuable(std::vector<CH3DPrintModelPtr> objs, bool visual)

{
    for (int i = 0; i < objs.size(); i++)
    {
        if (objs[i] != nullptr)
        {
            objs[i]->setVisuable(visual);
            objs[i]->updateToScene();
        }
    }

    emit modelObjsStatusChanged(ModelStatusChangedType::VisuableChanged);
    calcVisibleModelCount();
    
    modelCheckSceneIn();
}

void CHDoc::setObjsVisuable(std::vector<CHMeshShowObjPtr> objs, bool visual)

{
    for (int i = 0; i < objs.size(); i++)
    {
        if (objs[i] != nullptr)
        {
            objs[i]->setVisuable(visual);
            objs[i]->updateToScene();
        }
    }

    emit modelObjsStatusChanged(ModelStatusChangedType::VisuableChanged);
    calcVisibleModelCount();
    
    modelCheckSceneIn();
}

void CHDoc::setObjVisuable(CHMeshShowObjPtr obj, bool visual)
{
    std::vector<CHMeshShowObjPtr> objs;
    objs.push_back(obj);
    setObjsVisuable(objs, visual);
}

void CHDoc::setObjVisuable(CH3DPrintModelPtr obj, bool visual)
{
    std::vector<CH3DPrintModelPtr> objs;
    objs.push_back(obj);
    setObjsVisuable(objs, visual);
}

void CHDoc::resetObjs(std::vector<CH3DPrintModelPtr> objs)
{
    for (int i = 0; i < objs.size(); i++)
    {
        if (objs[i] != nullptr)
        {
            objs[i]->deleteAllSupportMeshes();
            objs[i]->resetTransform();
            objs[i]->calRealAABB();
        }
    }

    emit modelCheckSceneInChanged();
    emit modelObjsStatusChanged(ModelStatusChangedType::ResetMesh);
}


void CHDoc::calcVisibleModelCount()

{
    int count =0;
    for (int i = 0; i < m_printObjs.size(); i++)
    {
         if (m_printObjs[i]->getVisuable())
         {
             count++;
         }
    }

    emit visibleModelCountChanged(count);

}

void CHDoc::modelCheckSceneIn() 
{
    
    
    bool allInPrintBox = true;
    int visuableCount = 0;
    for(int i = 0; i < m_printObjs.size(); i++)
    {
        if(!m_printObjs[i] -> getVisuable()) 
        {
            continue;
        }
        visuableCount++;
        
        //m_printObjs[i]->isSceneIn(m_printObjs[i]->getRealAABB(), m_machineBox->m_baseAABB);
        auto printAABB = m_printObjs[i]->getRealAABB();
        for(int j = 0;j< m_printObjs[i]->m_supportMeshes.size();j++)
        {
            
            auto sptAABB = m_printObjs[i]->m_supportMeshes[j]->getRealAABB();
            if (sptAABB.m_Zmin > -1 && sptAABB.m_Zmin <= 0)
            {
                sptAABB.m_Zmin = 0;
            }
            printAABB.add(sptAABB);
        }
        m_printObjs[i]->isSceneIn(printAABB, m_machineBox->m_baseAABB);
        allInPrintBox = allInPrintBox && m_printObjs[i]->getIsSceneIn();

    }
    if(visuableCount > 0)
    {
        emit modelOutOfRangeChanged(allInPrintBox);
    }
}













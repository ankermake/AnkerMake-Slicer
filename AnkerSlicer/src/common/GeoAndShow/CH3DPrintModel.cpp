#include "CH3DPrintModel.h"
#include "CHScene.h"
#include <QApplication>
#define EPSINON1 0.001

CH3DPrintModel::CH3DPrintModel()
{
    m_oldMesh = 0;
    m_isSceneIn = true;
}

CH3DPrintModel::~CH3DPrintModel()
{
    if (m_oldMesh)
    {
        delete m_oldMesh;
    }
}

void CH3DPrintModel::updateToScene()
{
    CHMeshShowObj::updateToScene();

    for (int i = 0; i < m_supportMeshes.size(); i++)
    {
        m_supportMeshes[i]->updateToScene();
    }
}

void CH3DPrintModel::copy(CHShowObjPtr& obj)
{
    if (!obj)
    {
        obj = CH3DPrintModelPtr(new CH3DPrintModel);
    }

    CH3DPrintModelPtr out = std::dynamic_pointer_cast<CH3DPrintModel>(obj);
    if (!out)
    {
        return;
    }
    CHMeshShowObj::copy(obj);
    out->m_filePath = m_filePath;
    out->m_meshPath = m_meshPath;

    out->m_oldMesh = new CMeshO(*m_oldMesh);
}





bool CH3DPrintModel::matrixEqual(QMatrix4x4 tran1, QMatrix4x4 tran2)
{
    
    int scale = 10;
    auto floatEqual = [&](float v1, float v2)->bool {
        //return (int)(v1 * scale) == (int)(v2 * scale);
        return (qAbs(v1 - v2) <= 0.01f);
    };
    if (floatEqual(tran1(0, 0), tran2(0, 0))
        && floatEqual(tran1(0, 1), tran2(0, 1))
        && floatEqual(tran1(0, 2), tran2(0, 2))
        && floatEqual(tran1(0, 3), tran2(0, 3))

        && floatEqual(tran1(1, 0), tran2(1, 0))
        && floatEqual(tran1(1, 1), tran2(1, 1))
        && floatEqual(tran1(1, 2), tran2(1, 2))
        && floatEqual(tran1(1, 3), tran2(1, 3))

        && floatEqual(tran1(2, 0), tran2(2, 0))
        && floatEqual(tran1(2, 1), tran2(2, 1))
        && floatEqual(tran1(2, 2), tran2(2, 2))
        && floatEqual(tran1(2, 3), tran2(2, 3))

        && floatEqual(tran1(3, 0), tran2(3, 0))
        && floatEqual(tran1(3, 1), tran2(3, 1))
        && floatEqual(tran1(3, 2), tran2(3, 2))
        && floatEqual(tran1(3, 3), tran2(3, 3))
        )
    {
        return true;
    }
    return false;
}
void CH3DPrintModel::setTransform(QMatrix4x4 tran)
{
    auto tmpTran = m_tran;
    m_tran = tran;
    if (!matrixEqual(tmpTran, tran))
    //if (!qFuzzyCompare(tmpTran,tran))
    {
        emit getDoc()->modelTransformChanged();
    }
}

void CH3DPrintModel::updateSupportMeshes()
{
    for (int i = 0; i < m_supportMeshes.size(); i++)
    {
        qDebug() << "Support Color:" << m_supportMeshes[i]->getColor();
        m_supportMeshes[i]->updateToScene();
    }
}

void CH3DPrintModel::setVisuable(bool show)
{
    m_visuable = show;
    for (int i = 0; i < m_supportMeshes.size(); i++)
    {
        m_supportMeshes[i]->setVisuable(show);
    }
}

void CH3DPrintModel::setDelete(bool del)
{
    m_delete = del;
    for (int i = 0; i < m_supportMeshes.size(); i++)
    {
        m_supportMeshes[i]->setDelete(del);
    }
}

void CH3DPrintModel::addSupportMesh(CHMeshShowObjPtr supportModel)
{
    auto sptModel = std::dynamic_pointer_cast<SupportMesh>(supportModel);
    m_supportMeshes.append(sptModel);
}

void CH3DPrintModel::deleteSupportMesh(const CHMeshShowObjPtr supportModel)
{
    auto sptModel = std::dynamic_pointer_cast<SupportMesh>(supportModel);
    int i = m_supportMeshes.indexOf(sptModel);
    if (i >= 0) {
        m_supportMeshes[i]->setDelete(true);
        m_supportMeshes[i]->updateToScene();
        m_supportMeshes.removeAt(i);
    }
}

//void CH3DPrintModel::deleteAllSupportMeshes()
//{
//    for (int i = 0; i < m_supportMeshes.size(); i++)
//    {
//        m_supportMeshes[i]->setDelete(true);
//        m_supportMeshes[i]->updateToScene();
//
//        QApplication::processEvents();
//    }
//    m_supportMeshes.clear();
//}

void CH3DPrintModel::deleteAllSupportMeshes()
{
    for (int i = 0; i < m_supportMeshes.size(); i++)
    {
        m_supportMeshes[i]->setDelete(true);
        m_supportMeshes[i]->updateToScene();

        if (i%60 == 0)
        {
            QApplication::processEvents();
        }
    }
    m_supportMeshes.clear();
}

void CH3DPrintModel::cmeshToMyModel()
{
    //CH3DPrintModelPtr outmesh(new CH3DPrintModel);
    //this->m_filePath = "";
    //this->m_oldMesh = new CMeshO(cm);
    if (m_oldMesh == nullptr)
        return;
    
    //uv
    m_baseAABB = CHAABB3D(m_oldMesh->bbox.min.X(), m_oldMesh->bbox.min.Y(), m_oldMesh->bbox.min.Z(),
        m_oldMesh->bbox.max.X(), m_oldMesh->bbox.max.Y(), m_oldMesh->bbox.max.Z());
    m_realAABB = m_baseAABB;
    if (true)
    {
        std::vector<QVector3D>& points = m_vertices;
        std::vector<QVector3D>& nors = m_nors;
        std::vector<MyTriangle>& faces = m_trians;
        points.resize(m_oldMesh->face.size() * 3);
        nors.resize(m_oldMesh->face.size() * 3);
        faces.resize(m_oldMesh->face.size());
        int p = 0;
        for (auto it = m_oldMesh->face.begin(); it != m_oldMesh->face.end(); it++)
        {
            points[3 * p][0] = it->V(0)->P().X();
            points[3 * p][1] = it->V(0)->P().Y();
            points[3 * p][2] = it->V(0)->P().Z();

            points[3 * p + 1][0] = it->V(1)->P().X();
            points[3 * p + 1][1] = it->V(1)->P().Y();
            points[3 * p + 1][2] = it->V(1)->P().Z();

            points[3 * p + 2][0] = it->V(2)->P().X();
            points[3 * p + 2][1] = it->V(2)->P().Y();
            points[3 * p + 2][2] = it->V(2)->P().Z();

            nors[3 * p][0] = it->N().X();
            nors[3 * p][1] = it->N().Y();
            nors[3 * p][2] = it->N().Z();
            nors[3 * p + 1] = nors[3 * p];
            nors[3 * p + 2] = nors[3 * p];

            faces[p].m_index1 = 3 * p;
            faces[p].m_index2 = 3 * p + 1;
            faces[p].m_index3 = 3 * p + 2;
            p++;
        }
    }
    else
    {
        std::vector<QVector3D>& points = m_vertices;
        std::vector<QVector3D>& nors = m_nors;
        points.resize(m_oldMesh->vert.size());
        nors.resize(m_oldMesh->vert.size());
        int i = 0;
        for (auto it = m_oldMesh->vert.begin(); it != m_oldMesh->vert.end(); it++)
        {
            points[i][0] = it->P().X();
            points[i][1] = it->P().Y();
            points[i][2] = it->P().Z();

            nors[i][0] = it->N().X();
            nors[i][1] = it->N().Y();
            nors[i][2] = it->N().Z();

            i++;
        }

        std::vector<MyTriangle>& faces = m_trians;
        faces.resize(m_oldMesh->face.size());
        int p = 0;
        for (auto it = m_oldMesh->face.begin(); it != m_oldMesh->face.end(); it++)
        {
            faces[p].m_index1 = it->V(0) - m_oldMesh->vert.data();
            faces[p].m_index2 = it->V(1) - m_oldMesh->vert.data();
            faces[p].m_index3 = it->V(2) - m_oldMesh->vert.data();
            p++;
        }
    }
    /*setTransform(QMatrix4x4(m_oldMesh->Tr.ElementAt(0, 0), m_oldMesh->Tr.ElementAt(0, 1), m_oldMesh->Tr.ElementAt(0, 2), m_oldMesh->Tr.ElementAt(0, 3),
        m_oldMesh->Tr.ElementAt(1, 0), m_oldMesh->Tr.ElementAt(1, 1), m_oldMesh->Tr.ElementAt(1, 2), m_oldMesh->Tr.ElementAt(1, 3),
        m_oldMesh->Tr.ElementAt(2, 0), m_oldMesh->Tr.ElementAt(2, 1), m_oldMesh->Tr.ElementAt(2, 2), m_oldMesh->Tr.ElementAt(2, 3),
        m_oldMesh->Tr.ElementAt(3, 0), m_oldMesh->Tr.ElementAt(3, 1), m_oldMesh->Tr.ElementAt(3, 2), m_oldMesh->Tr.ElementAt(3, 3)));  */
    delete m_oldMesh;
    m_oldMesh = nullptr;
}

bool CH3DPrintModel::getIsSceneIn() const
{
    return m_isSceneIn;
}

void CH3DPrintModel::setIsSceneIn(bool value)
{
    if (m_isSceneIn != value)
    {
        m_isSceneIn = value;

//        bool allInPrintBox = true;
//        for (int i = 0; i < getDoc()->m_printObjs.size(); i++)
//        {
//            if (!getDoc()->m_printObjs[i]->getIsSceneIn())
//            {
//                allInPrintBox = false;
//                break;
//            }
//        }
//        emit getDoc()->modelOutOfRangeChanged(allInPrintBox);
        updateToScene();
    }

}

bool CH3DPrintModel::isSceneIn(const CH3DPrintModel& model)
{


    return true;
}

void CH3DPrintModel::getBoxPoints(const CHAABB3D& box, std::vector<QVector3D>& points)
{
    QVector3D iMin = box.getLeftDownPoint();
    QVector3D iMax = box.getRightTopPoint();


    points.push_back(QVector3D(iMin.x(), iMin.y(), iMin.z()));
    points.push_back(QVector3D(iMax.x(), iMin.y(), iMin.z()));
    points.push_back(QVector3D(iMax.x(), iMax.y(), iMin.z()));
    points.push_back(QVector3D(iMin.x(), iMax.y(), iMin.z()));
    points.push_back(QVector3D(iMin.x(), iMin.y(), iMax.z()));
    points.push_back(QVector3D(iMin.x(), iMax.y(), iMax.z()));
    points.push_back(QVector3D(iMax.x(), iMax.y(), iMax.z()));
    points.push_back(QVector3D(iMax.x(), iMin.y(), iMax.z()));
}

bool CH3DPrintModel::isSceneIn(const CHAABB3D& modelBox, const CHAABB3D& baseBox)
{
    std::vector<QVector3D> points;
    getBoxPoints(modelBox, points);
    for (auto p : points)
    {
        if (!isBoxIn(baseBox, p))
        {
            setIsSceneIn(false);
            return m_isSceneIn;
        }
    }
    setIsSceneIn(true);
    return m_isSceneIn;
}

bool CH3DPrintModel::isBoxIn(const CHAABB3D& box, const QVector3D& p)
{
    return ((p.x() - box.m_Xmin > -EPSINON1) &&
        (p.y() - box.m_Ymin > -EPSINON1) &&
        (p.z() - box.m_Zmin > -EPSINON1) &&
        (box.m_Xmax - p.x() > -EPSINON1) &&
        (box.m_Ymax - p.y() > -EPSINON1) &&
        (box.m_Zmax - p.z() > -EPSINON1));
}

















#include "CHBaseAlg.h"
#include "TransformPack.h"
#include "QFile"
#include "../common/utilities/tlogger.h"
#include "errno.h"
#include <QTextCodec>
using namespace AkUtil;
using namespace std;



CHBaseAlg::CHBaseAlg()
{

}

CHBaseAlg::~CHBaseAlg()
{

}

CHBaseAlg* CHBaseAlg::instance()
{
    static CHBaseAlg* rtn;
    if (!rtn)
    {
        rtn = new CHBaseAlg;
    }
    return rtn;
}

bool CHBaseAlg::writeMeshesToStlFile(CHMeshShowObjPtr meshPtr, const QString& path, bool binary)
{
    std::vector<CHMeshShowObjPtr> meshes;
    meshes.push_back(meshPtr);
    return writeMeshesToStlFile(meshes, path, binary);
}

bool CHBaseAlg::writeMeshesToStlFile(const std::vector<CHMeshShowObjPtr>& meshes, const QString& path, bool binary)
{
    if (meshes.size() == 0)
    {
        return false;
    }

    FILE* fp = 0;
    //fp = fopen(path.toStdString().c_str(), "wb");
    string filePath = std::string(path.toLocal8Bit().constData());

    fp = fopen(filePath.c_str(), "wb"); 
    if (fp == 0)
    {
        TError(QString("open file %1 failed, errno=%2, reason is %3").arg(path).arg(errno).arg(strerror(errno)));
        return false;
    }

    if (binary)
    {
        // Write Header
        char header[128] = "VCG                                                                                                  ";

        fwrite(header, 80, 1, fp);
        // write number of facets
        int faceSum = 0;
        for (int i = 0; i < meshes.size(); i++)
        {
            faceSum += meshes[i]->m_trians.size();
        }
        fwrite(&faceSum, 1, sizeof(int), fp);

        for (int i = 0; i < meshes.size(); i++)
        {
            for (int p = 0; p < meshes[i]->m_trians.size(); p++)
            {
                QVector3D pt1 = TransformPack::pRot(meshes[i]->getTransform(), meshes[i]->m_vertices[meshes[i]->m_trians[p].m_index1]);
                QVector3D pt2 = TransformPack::pRot(meshes[i]->getTransform(), meshes[i]->m_vertices[meshes[i]->m_trians[p].m_index2]);
                QVector3D pt3 = TransformPack::pRot(meshes[i]->getTransform(), meshes[i]->m_vertices[meshes[i]->m_trians[p].m_index3]);

                
                QVector3D tnor = QVector3D::crossProduct(pt2 - pt1, pt3 - pt1).normalized();
                fwrite(&tnor, 3, sizeof(float), fp);

                fwrite(&pt1, 3, sizeof(float), fp);
                fwrite(&pt2, 3, sizeof(float), fp);
                fwrite(&pt3, 3, sizeof(float), fp);

                unsigned short attributes = 0;
                fwrite(&attributes, 1, sizeof(short), fp);
            }
        }
    }
    else
    {
        fprintf(fp, "solid vcg\n");
        for (int i = 0; i < meshes.size(); i++)
        {
            for (int p = 0; p < meshes[i]->m_trians.size(); p++)
            {
                QVector3D pt1 = TransformPack::pRot(meshes[i]->getTransform(), meshes[i]->m_vertices[meshes[i]->m_trians[p].m_index1]);
                QVector3D pt2 = TransformPack::pRot(meshes[i]->getTransform(), meshes[i]->m_vertices[meshes[i]->m_trians[p].m_index2]);
                QVector3D pt3 = TransformPack::pRot(meshes[i]->getTransform(), meshes[i]->m_vertices[meshes[i]->m_trians[p].m_index3]);

                
                QVector3D tnor = QVector3D::crossProduct(pt2 - pt1, pt3 - pt1).normalized();

                fprintf(fp, "  facet normal %13e %13e %13e\n", tnor[0], tnor[1], tnor[2]);
                fprintf(fp, "    outer loop\n");

                fprintf(fp, "      vertex  %13e %13e %13e\n", pt1[0], pt1[1], pt1[2]);
                fprintf(fp, "      vertex  %13e %13e %13e\n", pt2[0], pt2[1], pt2[2]);
                fprintf(fp, "      vertex  %13e %13e %13e\n", pt3[0], pt3[1], pt3[2]);

                fprintf(fp, "    endloop\n");
                fprintf(fp, "  endfacet\n");
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

void CHBaseAlg::extremeBetweenLineAndLine(const CHLine3DPtr& line1, const CHLine3DPtr& line2,
    LineLineExtremeResult& res)
{
    QVector3D vec1 = line1->m_et - line1->m_st;
    vec1.normalized();
    QVector3D dir = line2->m_et - line2->m_st;
    float lenth = dir.length();
    QVector3D shareNor = QVector3D::crossProduct(vec1, dir);
    if (fabs(shareNor.length() / lenth) < 0.00001)
    {
        res.m_parallel = true;
    }
    else
    {
        res.m_parallel = false;

        
        
        QVector3D planeNor = QVector3D::crossProduct(vec1, shareNor);
        float m = QVector3D::dotProduct(dir, planeNor);
        float t = QVector3D::dotProduct(line1->m_st - line2->m_st, planeNor) / m;
        QVector3D intersection = line2->m_st + dir * t;

        
        res.m_u1 = line1->getClosestPoint(intersection);
        res.m_pt1 = line1->pointAt(res.m_u1);
        res.m_u2 = t;
        res.m_pt2 = intersection;
    }
}

void CHBaseAlg::extremeBetweenLineSegmentAndLine(const CHLineSegment3DShowObjPtr& lineSegment1, const CHLine3DPtr& line2,
    LineLineExtremeResult& res)
{
    
    CHLine3DPtr line1(new CHLine3D);
    line1->create(TransformPack::pRot(lineSegment1->getTransform(), lineSegment1->m_st),
        TransformPack::pRot(lineSegment1->getTransform(), lineSegment1->m_et));
    extremeBetweenLineAndLine(line1, line2, res);

    if (!res.m_parallel)
    {
        if (res.m_u1 < 0)
        {
            res.m_u1 = 0;
            res.m_pt1 = lineSegment1->m_st;
            res.m_u2 = line2->getClosestPoint(res.m_pt1);
            res.m_pt2 = line2->pointAt(res.m_u2);
        }
        else if (res.m_u1 > 1)
        {
            res.m_u1 = 1;
            res.m_pt1 = lineSegment1->m_et;
            res.m_u2 = line2->getClosestPoint(res.m_pt1);
            res.m_pt2 = line2->pointAt(res.m_u2);
        }
    }
}

QMatrix4x4 CHBaseAlg::calTransformFromParams(const QVector3D& rotCenter, const vector<float>& params)
{
    QMatrix4x4 tran1, tran2, tran3, tran4, tran5, tran6, tran7;
    tran1.translate(-rotCenter);
    tran2.scale(params[0], params[1], params[2]);
    tran3.rotate(params[3], QVector3D(1, 0, 0));
    tran4.rotate(params[4], QVector3D(0, 1, 0));
    tran5.rotate(params[5], QVector3D(0, 0, 1));
    tran6.translate(QVector3D(params[6], params[7], params[8]));
    tran7.translate(rotCenter);

    return tran7 * tran6 * tran5 * tran4 * tran3 * tran2 * tran1;
}

void CHBaseAlg::calEulerAnglesFromRotMatrix(const QMatrix4x4& rotTran, double& angleX, double& angleY, double& angleZ)
{
    float value[4][4];
    float* tdata = rotTran.transposed().data();
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            value[i][j] = tdata[i * 4 + j];
        }
    }

    double tol = 0.00000001;
    if (fabs(value[2][0] - 1) <= tol)//=1
    {
        angleY = -CH_PI / 2.0;
        angleZ = 0;
        angleX = -angleZ + atan2(-value[0][1], -value[0][2]);
    }
    else if (fabs(value[2][0] + 1) <= tol)//=-1
    {
        angleY = CH_PI / 2.0;
        angleZ = 0;
        angleX = angleZ + atan2(value[0][1], value[0][2]);
    }
    else   
    {
        angleY = -asin(value[2][0]);
        

        double tt = cos(angleY);
        angleX = atan2(value[2][1] / tt, value[2][2] / tt);
        angleZ = atan2(value[1][0] / tt, value[0][0] / tt);
    }
}

bool CHBaseAlg::LineMeshIntersect(CMeshO& mesh, const CHLine3DPtr& line, std::vector<CHPointOnMesh>& intersections)
{
    
    QMatrix4x4 mat(
        mesh.Tr.ElementAt(0, 0), mesh.Tr.ElementAt(0, 1), mesh.Tr.ElementAt(0, 2), mesh.Tr.ElementAt(0, 3),
        mesh.Tr.ElementAt(1, 0), mesh.Tr.ElementAt(1, 1), mesh.Tr.ElementAt(1, 2), mesh.Tr.ElementAt(1, 3),
        mesh.Tr.ElementAt(2, 0), mesh.Tr.ElementAt(2, 1), mesh.Tr.ElementAt(2, 2), mesh.Tr.ElementAt(2, 3),
        mesh.Tr.ElementAt(3, 0), mesh.Tr.ElementAt(3, 1), mesh.Tr.ElementAt(3, 2), mesh.Tr.ElementAt(3, 3));
    QMatrix4x4 invertMat = mat.inverted();

    CHLine3DPtr copyline(new CHLine3D);
    copyline->create(TransformPack::pRot(invertMat, line->m_st), TransformPack::pRot(invertMat, line->m_et));
    vector<int> outindexs;

    {
        const auto& bbox = mesh.bbox;
        QVector3D boxMin = QVector3D(bbox.min.X(), bbox.min.Y(), bbox.min.Z());
        QVector3D boxMax = QVector3D(bbox.max.X(), bbox.max.Y(), bbox.max.Z());
        bool isPickAabb = existIntersectionBetweenLineAndBox(copyline, boxMin, boxMax);
        if (!isPickAabb) { return isPickAabb; }
    }
    
    outindexs.resize(mesh.face.size());
    for (int i = 0; i < outindexs.size(); i++)
    {
        outindexs[i] = i;
    }


    for (int i = 0; i < outindexs.size(); i++)
    {
        CFaceO* tface = (CFaceO*)mesh.face.data() + outindexs[i];
        vcgTriMesh::VertexType* pv0 = tface->V(0);
        vcgTriMesh::VertexType* pv1 = tface->V(1);
        vcgTriMesh::VertexType* pv2 = tface->V(2);

        QVector3D pt1(pv0->P().V()[0], pv0->P().V()[1], pv0->P().V()[2]);
        QVector3D pt2(pv1->P().V()[0], pv1->P().V()[1], pv1->P().V()[2]);
        QVector3D pt3(pv2->P().V()[0], pv2->P().V()[1], pv2->P().V()[2]);
        CHPointOnMesh outpt;
        if (existIntersectionBetweenLineAndTriangle(copyline, pt1, pt2, pt3, outpt))
        {
            outpt.m_faceIndex = outindexs[i];

            
            outpt.m_pt = TransformPack::pRot(mat, outpt.m_pt);
            outpt.m_nor = TransformPack::pRot(invertMat.transposed(), outpt.m_nor);

            intersections.push_back(outpt);
        }
    }

    return intersections.size() > 0 ? true : false;
}

bool CHBaseAlg::LineSegmentMeshIntersect(CMeshO& mesh, const CHLineSegment3DShowObjPtr& lineSegment, std::vector<CHPointOnMesh>& intersections)
{
    CHLine3DPtr  line = CHLine3DPtr(new CHLine3D);
    line->create(TransformPack::pRot(lineSegment->getTransform(), lineSegment->m_st),
        TransformPack::pRot(lineSegment->getTransform(), lineSegment->m_et));
    std::vector<CHPointOnMesh> tps;
    LineMeshIntersect(mesh, line, tps);

    for (int i = 0; i < tps.size(); i++)
    {
        double uu = line->getClosestPoint(tps[i].m_pt);
        if (uu >= 0 && uu <= 1)
        {
            intersections.push_back(tps[i]);
        }
    }
    return intersections.size() > 0 ? true : false;
}

bool CHBaseAlg::LineMeshIntersect(const CHMeshShowObjPtr& mesh, const CHLine3DPtr& line, std::vector<CHPointOnMesh>& intersections)
{
    
    QMatrix4x4 mat = mesh->getTransform();
    QMatrix4x4 invertMat = mat.inverted();

    CHLine3DPtr copyline(new CHLine3D);
    copyline->create(TransformPack::pRot(invertMat, line->m_st), TransformPack::pRot(invertMat, line->m_et));

    {
        if (mesh->m_baseAABB.isEmpty()) {
            mesh->calBaseAABB();
        }
        const CHAABB3D& aabb = mesh->m_baseAABB;

        QVector3D boxMin = aabb.getLeftDownPoint();
        QVector3D boxMax = aabb.getRightTopPoint();
        bool isPickAabb = existIntersectionBetweenLineAndBox(copyline, boxMin, boxMax);
        if (!isPickAabb) { return isPickAabb; }
    }

    for (int i = 0; i < mesh->m_trians.size(); i++)
    {
        const QVector3D& pt1 = mesh->m_vertices[mesh->m_trians[i].m_index1];
        const QVector3D& pt2 = mesh->m_vertices[mesh->m_trians[i].m_index2];
        const QVector3D& pt3 = mesh->m_vertices[mesh->m_trians[i].m_index3];
        CHPointOnMesh outpt;
        if (existIntersectionBetweenLineAndTriangle(copyline, pt1, pt2, pt3, outpt))
        {
            outpt.m_faceIndex = i;
            outpt.m_pt = TransformPack::pRot(mat, outpt.m_pt);

            
            
            //QVector3D a_pt1 = mat * pt1;
            //QVector3D a_pt2 = mat * pt2;
            //QVector3D a_pt3 = mat * pt3;
            //QVector3D v0 = a_pt3 - a_pt1;
            //QVector3D v1 = a_pt2 - a_pt1;
            

            
            
            
            
            //outpt.m_nor = (invertMat.normalMatrix().transposed() * outpt.m_nor).normalized();

            //auto x = TransformPack::pRot(invertMat.transposed(), outpt.m_nor).normalized();
            //auto y = (invertMat.normalMatrix().transposed() * outpt.m_nor).normalized();
            //auto z = (invertMat.transposed()* QVector4D(outpt.m_nor,1)).normalized();
            //auto w = QVector3D(z);
            //auto p = w.normalized();

            
            outpt.m_nor = QVector3D(invertMat.transposed() * QVector4D(outpt.m_nor, 1)).normalized();

            intersections.push_back(outpt);
        }
    }

    return intersections.size() > 0 ? true : false;
}

bool CHBaseAlg::LineSegmentMeshIntersect(const CHMeshShowObjPtr& mesh, const CHLineSegment3DShowObjPtr& lineSegment, std::vector<CHPointOnMesh>& intersections)
{
    CHLine3DPtr  line = CHLine3DPtr(new CHLine3D);
    line->create(TransformPack::pRot(lineSegment->getTransform(), lineSegment->m_st),
        TransformPack::pRot(lineSegment->getTransform(), lineSegment->m_et));

    

    std::vector<CHPointOnMesh> tps;
    LineMeshIntersect(mesh, line, tps);

    for (int i = 0; i < tps.size(); i++)
    {
        double uu = line->getClosestPoint(tps[i].m_pt);
        if (uu >= 0 && uu <= 1)
        {
            intersections.push_back(tps[i]);
        }
    }
    return intersections.size() > 0 ? true : false;
}

bool CHBaseAlg::existIntersectionBetweenLineAndTriangle(const CHLine3DPtr& line, const QVector3D& pt1,
    const QVector3D& pt2, const QVector3D& pt3, CHPointOnMesh& outpt)
{
    QVector3D v0 = pt3 - pt1;
    QVector3D v1 = pt2 - pt1;
    QVector3D nor = QVector3D::crossProduct(v1, v0);
    const float tol = 0.0000001;
    if (fabs(nor.x()) < tol && fabs(nor.y()) < tol && fabs(nor.z()) < tol)
    {
        QVector3D vv0 = pt2 - pt3;
        nor = QVector3D::crossProduct(vv0, v0) + QVector3D::crossProduct(vv0, v1);
    }

    QVector3D st = line->m_st;
    QVector3D et = line->m_et;
    QVector3D dir = et - st;
    float t = 0;
    float m = QVector3D::dotProduct(dir, nor);
    if (fabs(m) < tol)
    {
        return false;
    }
    else
    {
        t = QVector3D::dotProduct(pt1 - st, nor) / m;
    }
    QVector3D intersection = st + dir * t;

    
    QVector3D v2 = intersection - pt1;
    float a = QVector3D::dotProduct(v0, v1);
    float b = QVector3D::dotProduct(v0, v0);
    float e = QVector3D::dotProduct(v1, v1);
    float beLine = b * e - a * a;

    if (abs(beLine) < tol)
    {
        return false;
    }

    float c = QVector3D::dotProduct(v2, v0);
    float d = QVector3D::dotProduct(v2, v1);
    float u = (e * c - a * d) / beLine;
    float v = (b * d - a * c) / beLine;
    if (u >= 0 && v >= 0 && u + v <= 1)
    {
        
        outpt.m_pt = intersection;
        outpt.m_nor = nor;
        
        if (outpt.m_pt.z() < 0)
        {
            int x = 0;
        }
    }
    else
    {
        return false;
    }
    return true;
}


bool CHBaseAlg::existIntersectionBetweenLineAndRectangle(
    const CHLine3DPtr& line,
    const QVector3D& pt1, const QVector3D& pt2, const QVector3D& pt3, const QVector3D& pt4)
{
    QVector3D v13 = pt3 - pt1;
    QVector3D v12 = pt2 - pt1;
    QVector3D nor = QVector3D::crossProduct(v12, v13);
    const float tol = 0.0000001;

    if (fabs(nor.x()) < tol && fabs(nor.y()) < tol && fabs(nor.z()) < tol)
    {
        QVector3D v32 = pt2 - pt3;
        nor = QVector3D::crossProduct(v32, v13) + QVector3D::crossProduct(v32, v12);
    }

    const QVector3D  dir = line->m_st - line->m_et;

    float m = QVector3D::dotProduct(dir, nor);
    if (fabs(m) < tol)
    {
        return false;
    }

    float t = QVector3D::dotProduct(pt1 - line->m_st, nor) / m;
    QVector3D intersection = line->m_st + dir * t;

    {   
        float a1 = QVector3D::dotProduct(nor, QVector3D::crossProduct(pt2 - pt1, intersection - pt1));
        float a2 = QVector3D::dotProduct(nor, QVector3D::crossProduct(pt3 - pt2, intersection - pt2));
        float a3 = QVector3D::dotProduct(nor, QVector3D::crossProduct(pt4 - pt3, intersection - pt3));
        float a4 = QVector3D::dotProduct(nor, QVector3D::crossProduct(pt1 - pt4, intersection - pt4));

        bool in = false;
        if (a1 < 0 && a2 < 0 && a3 < 0 && a4 < 0) { in = true; }
        if (a1 > 0 && a2 > 0 && a3 > 0 && a4 > 0) { in = true; }
        return in;
    }
}

int CHBaseAlg::existIntersectionBetweenLineAndBox(const CHLine3DPtr& line, const QVector3D& boxMin, const QVector3D& boxMax)
{
    const static int Index[24] = {
                  // 0
        4,5,6,7,  
        0,1,2,3,  
        3,0,4,7,  
        1,2,6,5,  
        0,1,5,4,  
        2,3,7,6   
    };

    const QVector3D points[8] = {
        {boxMin.x(), boxMin.y(), boxMin.z()}, // 0
        {boxMax.x(), boxMin.y(), boxMin.z()}, // 1
        {boxMax.x(), boxMax.y(), boxMin.z()}, // 2
        {boxMin.x(), boxMax.y(), boxMin.z()}, // 3

        {boxMin.x(), boxMin.y(), boxMax.z()}, // 4
        {boxMax.x(), boxMin.y(), boxMax.z()}, // 5
        {boxMax.x(), boxMax.y(), boxMax.z()}, // 6
        {boxMin.x(), boxMax.y(), boxMax.z()}, // 7
    };

    int faceMask = 0x0000;
    for (int i = 0; i < 24; i += 4) {
        bool res = existIntersectionBetweenLineAndRectangle(line, points[Index[i + 0]], points[Index[i + 1]], points[Index[i + 2]], points[Index[i + 3]]);
        if (res) {
            faceMask |= (1 << (i/4));
        }
    }
    return faceMask;
}

int CHBaseAlg::existIntersectionBetweenLineAndBox(const CHLine3DPtr &line, const CHAABB3D &box, const QMatrix4x4 &boxMat44)
{
    CHLine3DPtr copyline(new CHLine3D);
    if(!boxMat44.isIdentity()){
        QMatrix4x4 invertMat = boxMat44.inverted();
        copyline->create(invertMat * line->m_st, invertMat * line->m_et);
    }else{
        copyline->create(line->m_st, line->m_et);
    }

    const CHAABB3D& aabb = box;
    QVector3D boxMin = aabb.getLeftDownPoint();
    QVector3D boxMax = aabb.getRightTopPoint();
    return existIntersectionBetweenLineAndBox(copyline, boxMin, boxMax);
}

/*
bool LineAndPlane(
        const QVector3D& line_st, const QVector3D& line_et,
        const QVector3D& pt1,const QVector3D& pt2, const QVector3D& pt3,
        QVector3D& intersection)
{
    const static int PointCount = 3;
    const static float TOL = 0.0000001;

    QVector3D v12 = pt2 - pt1;
    QVector3D v23 = pt3 - pt2;
    QVector3D v31 = pt1 - pt3;

    const QVector3D * pt[4]   = {&pt1, &pt2, &pt3, &pt1};
    const QVector3D * edge[4] = {&v12, &v23, &v31, &v12};

    QVector3D face_normal;
    for(int i=0; i<PointCount; ++i){

        if(face_normal.lengthSquared() > TOL){
            break;
        }
        else if(i==(PointCount-1)){
            return false;
        }
    }

    const QVector3D line_dir = line_et - line_st;

    float m = QVector3D::dotProduct(line_dir, face_normal);
    if (fabs(m) < TOL){
        return false;
    }

    float t = QVector3D::dotProduct(pt1 - line_st, face_normal) / m;

};
*/






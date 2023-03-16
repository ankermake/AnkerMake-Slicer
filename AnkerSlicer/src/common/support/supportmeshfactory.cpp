//#include "supportmeshfactory.h"
//#include "../GeoAndShow/CHTetrahedron.h"
//#include "GeoAndShow/CHCylinder.h"
//#include "GeoAndShow/TransformPack.h"
//#include "GeoAndShow/CHCone.h"
//#include "GeoAndShow/CHTrapezoidBody.h"
//#include "GeoAndShow/CHSphere.h"
//#include "GeoAndShow/CHCuboid.h"
//#include <QDebug>
//
//SupportMeshFactory::SupportMeshFactory()
//{
//
//}
//
////
//QList<CHShowObjPtr> SupportMeshFactory::GenerateSupportMeshes(QList<SupportLine> supportLines)
//{
//    QList<CHShowObjPtr> supportMeshes;

//    //for each(const SupportLine& line, supportLines)
//    for(int i = 0; i< supportLines.size(); i++)
//    {
//        SupportLine& line = supportLines[i];
//        switch (line.type) {
//            case Top:
//            {
//                supportMeshes.append(CreateTop(line));
//                break;
//            }
//            case Body:
//            {
//                supportMeshes.append(CreateBody(line));
//                break;
//            }
//            case Bottom:
//            {
//                supportMeshes.append(CreateBottom(line));
//                break;
//            }
//            case Raft:
//            {
//                supportMeshes.append(CreateRaft(line));
//                break;
//            }
//
//        }
//    }
//    return supportMeshes;
//}
//
//QVector3D SupportMeshFactory::goThoughVector(QVector3D point, QVector3D normal, int length)
//{

//    long normalSize = normal.length();
//    QVector3D offset;
//    offset.setX((normal.x() / (double)normalSize) * length);
//    offset.setY((normal.y() / (double)normalSize) * length);
//    offset.setZ((normal.z() / (double)normalSize) * length);
//    return point + offset;
//
//}
//
//QList<CHShowObjPtr> SupportMeshFactory::CreateTopCuboid(SupportLine & line)
//{
//    QList<CHShowObjPtr> out;

//

//    if (line.connectPoints.size() == 4)
//    {
//        qDebug() << "CreateTop: line.connectPoints.size() is 4 create CHTetrahedronPtr" ;
//        CHTetrahedronPtr obj(new CHTetrahedron);
//        obj->create(line.connectPoints[0], line.connectPoints[1], line.connectPoints[2], line.connectPoints[3]);
//        out.append(obj);
//    }

//    else if  (line.connectPoints.size() == 2)
//    {
//        qDebug() << "CreateTop:  line.connectPoints.size() is 2 create CHCylinderPtr" ;
//        CHCuboidPtr obj(new CHCuboid);
//        obj->create(1, 1, 1);
//
//        QMatrix4x4 tran1, tran2, tran3;
//        tran1.scale(radius, radius, line.connectPoints[0].distanceToPoint(line.connectPoints[1]));
//        tran2 = TransformPack::rotMat(QVector3D(0, 0, 1), line.connectPoints[1] - line.connectPoints[0], QVector3D(0, 0, 0));
//        tran3.translate(line.connectPoints[0]);
//        obj->setTransform(tran3 * tran2 * tran1);
//        out.append(obj);
//    }

//    {
////        CHCylinderPtr connectObj(new CHCylinder);
////        connectObj->create(1, 1, 1);
////        QMatrix4x4 tran1, tran2, tran3;
////        tran1.scale(radius/2, radius/2, radius);
////        tran2 = TransformPack::rotMat(QVector3D(0, 0, 1), QVector3D(1, 0, 0), QVector3D(0, 0, 0));
////        tran3.translate(line.connectPoints[1]);
////        connectObj->setTransform(tran3 * tran2 * tran1);
////        out.append(connectObj);
//
//

//        CHSpherePtr ball(new CHSphere);
//        ball->create(line.connectPoints.last(),3);
//        out.append(ball);
//
//    }
//
//    return out;
//}
//
//QList<CHShowObjPtr> SupportMeshFactory::CreateBodyCuboid(SupportLine &line)
//{
//    QList<CHShowObjPtr> out;
//
//    int num = (int)line.connectPoints.size();
//    if (num < 2)
//    {
//        return out;
//    }
//    qDebug() << QString("CreateBody:  line.connectPoints.size() is %1 create CHCylinderPtr").arg(num) ;
//
//    for (int i = 0; i < num - 1; i++)
//    {

//        CHCuboidPtr obj(new CHCuboid);
//        obj->create(1, 1, 1);
//
//        QMatrix4x4 tran1, tran2, tran3;
//        tran1.scale(radius, radius, line.connectPoints[i + 1].distanceToPoint(line.connectPoints[i]));
//        tran2 = TransformPack::rotMat(QVector3D(0, 0, 1), line.connectPoints[i + 1] - line.connectPoints[i], QVector3D(0, 0, 0));
//        tran3.translate(line.connectPoints[i]);
//        obj->setTransform(tran3 * tran2 * tran1);
//        out.append(obj);
//    }
//

////    CHSpherePtr ball(new CHSphere);
////    ball->create(line.connectPoints.last(),3);
////    out.append(ball);
//
//    return out;
//
//}
//
//QList<CHShowObjPtr> SupportMeshFactory::CreateBottomCuboid(SupportLine &line)
//{
//    QList<CHShowObjPtr> out;
//    return out;
//}
//
//QList<CHShowObjPtr> SupportMeshFactory::CreateRaftCuboid(SupportLine &line)
//{
//    QList<CHShowObjPtr> out;
//    return out;
//}
//
//QList<CHShowObjPtr> SupportMeshFactory::CreateTop(SupportLine & line)
//{
//    QList<CHShowObjPtr> out;
//

//    if (line.connectPoints.size() == 4)
//    {
//        qDebug() << "CreateTop: line.connectPoints.size() is 4 create CHTetrahedronPtr" ;
//        CHTetrahedronPtr obj(new CHTetrahedron);
//        obj->create(line.connectPoints[0], line.connectPoints[1], line.connectPoints[2], line.connectPoints[3]);
//        out.append(obj);       
//    }

//    else if  (line.connectPoints.size() == 2)
//    {
//        qDebug() << "CreateTop:  line.connectPoints.size() is 2 create CHCylinderPtr" ;

//        CHCylinderPtr obj(new CHCylinder);
//        obj->create(1, 1, 1, 6);
//
//		QMatrix4x4 tran1, tran2, tran3;
//		tran1.scale(radius, radius, line.connectPoints[0].distanceToPoint(line.connectPoints[1]));
//        tran2 = TransformPack::rotMat(QVector3D(0, 0, 1), line.connectPoints[1] - line.connectPoints[0], QVector3D(0, 0, 0));
//        tran3.translate(line.connectPoints[0]);
//        obj->setTransform(tran3 * tran2 * tran1);
//        out.append(obj);
//    }
//
//
//
//
//    return out;
//}
//
//QList<CHShowObjPtr> SupportMeshFactory::CreateBody(SupportLine & line)
//{
//
//    QList<CHShowObjPtr> out;
//
//    int num = (int)line.connectPoints.size();
//    if (num < 2)
//    {
//        return out;
//    }
//    qDebug() << QString("CreateBody:  line.connectPoints.size() is %1 create CHCylinderPtr").arg(num) ;
//
//	for (int i = 0; i < num - 1; i++)
//	{

//		CHCylinderPtr obj(new CHCylinder);
//        obj->create(1, 1, 1, 6);
//
//		QMatrix4x4 tran1, tran2, tran3;
//		tran1.scale(radius, radius, line.connectPoints[i + 1].distanceToPoint(line.connectPoints[i]));
//		tran2 = TransformPack::rotMat(QVector3D(0, 0, 1), line.connectPoints[i + 1] - line.connectPoints[i], QVector3D(0, 0, 0));
//		tran3.translate(line.connectPoints[i]);
//		obj->setTransform(tran3 * tran2 * tran1);
//		out.append(obj);
//	}
//

//    CHSpherePtr ball(new CHSphere);
//    ball->create(line.connectPoints.first(),3,10);
//    out.append(ball);
//
//    return out;
//}
//

//QList<CHShowObjPtr> SupportMeshFactory::CreateBottom(SupportLine & line)
//{
//	QList<CHShowObjPtr> out;
//
//    qDebug() << QString("CreateBottom:  line.connectPoints.size() is %1 create CHCylinderPtr").arg(line.connectPoints.size() ) ;
//    if (line.connectPoints.size() == 2)
//    {

//        CHConePtr obj(new CHCone);
//        obj->create(1, 1);
//
//		QMatrix4x4 tran1, tran2, tran3;
//		tran1.scale(radius, radius, line.connectPoints[1].distanceToPoint(line.connectPoints[0]));
//		tran2 = TransformPack::rotMat(QVector3D(0, 0, 1), line.connectPoints[1] - line.connectPoints[0], QVector3D(0, 0, 0));
//		tran3.translate(line.connectPoints[0]);
//		obj->setTransform(tran3 * tran2 * tran1);
//		out.append(obj);
//    }

//    CHSpherePtr ball(new CHSphere);
//    ball->create(line.connectPoints.last(),3);
//    out.append(ball);
//
//    return out;
//}
//

//QList<CHShowObjPtr> SupportMeshFactory::CreateRaft(SupportLine & line)
//{
//	QList<CHShowObjPtr> out;
//
//	if (line.connectPoints.size() == 2)
//	{
//        float topLenth = 6;
//        float downLenth = 10;
//        CHTrapezoidBodyPtr obj(new CHTrapezoidBody);
//        obj->create(QVector3D(0, 0, 1), QVector3D(0, 0, 0), topLenth, topLenth, downLenth, downLenth);
//
//		QMatrix4x4 tran1, tran2, tran3;
//		tran1.scale(1, 1, line.connectPoints[1].distanceToPoint(line.connectPoints[0]));
//		tran2 = TransformPack::rotMat(QVector3D(0, 0, 1), line.connectPoints[0] - line.connectPoints[1], QVector3D(0, 0, 0));
//		tran3.translate(line.connectPoints[1]);
//		obj->setTransform(tran3 * tran2 * tran1);
//		out.append(obj);
//	}
//	return out;
//}

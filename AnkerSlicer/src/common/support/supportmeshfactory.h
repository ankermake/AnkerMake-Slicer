//#ifndef SUPPORTMESHFACTORY_H
//#define SUPPORTMESHFACTORY_H
//#include "../GeoAndShow/CHShowObj.h"
//#include <QList>
//#include "supportline.h"
//
//class SupportMeshFactory
//{
//public:
//    SupportMeshFactory();
//    static SupportMeshFactory* instance()
//    {
//        static SupportMeshFactory obj;
//        return &obj;
//    }
//

//    QList<CHShowObjPtr> GenerateSupportMeshes(QList<SupportLine> supportLines);
//
//    QVector3D goThoughVector(QVector3D point, QVector3D normal, int length);
//
//private:
//    QList<CHShowObjPtr> CreateTop(SupportLine& line);
//    QList<CHShowObjPtr> CreateBody(SupportLine& line);
//    QList<CHShowObjPtr> CreateBottom(SupportLine& line);
//    QList<CHShowObjPtr> CreateRaft(SupportLine& line);
//
//    QList<CHShowObjPtr> CreateTopCuboid(SupportLine &line);
//    QList<CHShowObjPtr> CreateBodyCuboid(SupportLine& line);
//    QList<CHShowObjPtr> CreateBottomCuboid(SupportLine& line);
//    QList<CHShowObjPtr> CreateRaftCuboid(SupportLine& line);
//};
//
//#endif // SUPPORTMESHFACTORY_H

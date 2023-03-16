//#include "supportdata.h"
//#include <QFileInfo>
//#include "utilities/ioapi.h"
//using namespace AkUtil;
//#include <QDebug>
//
//SupportData::SupportData()
//{
//
//}
//

//QList<SupportLine> SupportData::GetSupportLines()
//{
//    qDebug() << "enter GetSupportLines ";
//    QString dateSource = "C:/log/support.log";
//    QList<SupportLine>  sptLines;
//    QList<QString> lines = IoApi::getLines(dateSource);
//    SupportLine sptLine;
//    for(int i = 0; i<lines.size();i++)
//    {
//        QString line = lines[i].trimmed();
//

//        if(line.startsWith("line"))
//        {
//            qDebug() << line << "startsWith line";

//            if (sptLine.connectPoints.size()>0)
//            {
//                sptLines.append(sptLine);
//                sptLine.connectPoints.clear();
//            }
//

//            int idx = line.indexOf("is");
//            QString typeStr = line.mid(idx+2).trimmed();
//            if (typeStr == "Top")
//            {
//                sptLine.type = SupportPathType::Top;
//            }
//            else if (typeStr == "Body")
//            {
//                sptLine.type = SupportPathType::Body;
//            }
//            else if (typeStr == "Bottom")
//            {
//                sptLine.type = SupportPathType::Bottom;
//            }
//            else
//            {
//                sptLine.type = SupportPathType::Raft;
//            }
//            qDebug() << line << " sptLine.type  is " << sptLine.type;
//        }

//        else if(line.startsWith("point"))
//        {
//            qDebug() << line << "startsWith point";
//
//            int idx = line.indexOf("is");
//            QString pointStr = line.mid(idx+2).trimmed();
//            auto points = pointStr.split(',');
//            if (points.size()!=3)
//            {
//                throw std::exception("points.size()!=3");
//            }
//
//            QVector3D point;
//            point.setX(points[0].toFloat());
//            point.setY(points[1].toFloat());
//            point.setZ(points[2].toFloat());
//            sptLine.connectPoints.append(point);
//        }
//    }
//

//    if (sptLine.connectPoints.size() > 0)
//    {
//        sptLines.append(sptLine);
//    }
//    return sptLines;
//}
//

//QList<SupportLine> SupportData::GetSupportLines(SupportPoint sptPoint)
//{
//    QList<SupportLine> lines;
//    SupportLine line1;
//

//    line1.connectPoints.append(sptPoint.touchPoint);
//

//    auto goThoughVector=[](QVector3D point, QVector3D normal, int length)-> QVector3D
//    {

//        long normalSize = normal.length();
//        QVector3D offset;
//        offset.setX((normal.x() / (double)normalSize) * length);
//        offset.setY((normal.y() / (double)normalSize) * length);
//        offset.setZ((normal.z() / (double)normalSize) * length);
//        return point + offset;
//    };
//    auto endPoint = goThoughVector(sptPoint.touchPoint,sptPoint.normal,3);
//    line1.connectPoints.append(endPoint);
//    line1.type = SupportPathType::Top;
//    lines.append(line1);
//

//    SupportLine line2;
//    line2.connectPoints.append(endPoint);
//    QVector3D raftPoint(endPoint.x(),endPoint.y(),1);
//    line2.connectPoints.append(raftPoint);
//    line2.type = SupportPathType::Body;
//    lines.append(line2);
//

//    SupportLine line3;
//    line3.connectPoints.append(raftPoint);
//    QVector3D rootPoint(raftPoint.x(),raftPoint.y(),0);
//    line3.connectPoints.append(rootPoint);
//    line3.type = SupportPathType::Raft;
//    lines.append(line3);
//
//    QStringList sb;
//    for (int i = 0; i < lines.size(); i++)
//    {
//       auto line = lines[i];
//       sb.append(QString("line %1 type is %2").arg(i).arg(line.type));
//       for (int j = 0; j < line.connectPoints.size(); j++)
//       {
//           auto point = line.connectPoints[j];
//           sb.append(QString("    point %1 is %2,%3,%4").arg(j).arg(point.x()).arg(point.y()).arg(point.z()));
//       }
//    }
//
//    qDebug() << sb.join("\r\n");
//
//    return lines;
//}


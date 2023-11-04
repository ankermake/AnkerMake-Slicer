#ifndef AKSLICEINFO_H
#define AKSLICEINFO_H
#include <QString>
#include <QStringList>
#include <QDebug>


class AkSliceInfo
{
public:

//    SliceStatus status;

//    QString description;

//    float percent;
    
    QString originalStlName;
    
    QString stlFile;
    //mesh count
    int meshCount;

    QString gcodeFile;
    
    QString sliceCmd;
    
    float sliceTime;
    
    float printTime;
    
    float filamentCost;
public:
    inline QString toString(){
        QStringList resultList ;
        resultList << "stl_origin_name:" << originalStlName << "\r\n";
        resultList << "stl_path:" << stlFile << "\r\n";
        resultList << "result_path:" << gcodeFile << "\r\n";
        resultList << "SliceParam:" << sliceCmd << "\r\n";
        resultList << "TimeCost(s):" << QString("%1").arg(sliceTime) << "\r\n";
        resultList << "PrintTime(s):" << QString("%1").arg(printTime) << "\r\n";
        resultList << "FilamentCost(mm^3):" << QString("%1").arg(filamentCost) << "\r\n";
        return resultList.join("");
    }
};


#endif // AKSLICEINFO_H

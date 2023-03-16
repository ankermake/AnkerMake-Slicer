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
    
    QString gcodeFile;
    
    QString sliceCmd;
    
    float sliceTime;
    
    float printTime;
    
    float filamentCost;
public:
    inline QString toString(){
        QStringList resultList ;
        resultList << "stl原生名称:" << originalStlName << "\r\n";

        resultList << "stl路径:" << stlFile << "\r\n";

        resultList << "结果路径:" << gcodeFile << "\r\n";

        resultList << "切片参数:" << sliceCmd << "\r\n";

        resultList << "切片时长(s):" << QString("%1").arg(sliceTime) << "\r\n";

        resultList << "打印时长(s):" << QString("%1").arg(printTime) << "\r\n";

        resultList << "耗材体积(mm^3):" << QString("%1").arg(filamentCost) << "\r\n";

        return resultList.join("");
    }
};


#endif // AKSLICEINFO_H

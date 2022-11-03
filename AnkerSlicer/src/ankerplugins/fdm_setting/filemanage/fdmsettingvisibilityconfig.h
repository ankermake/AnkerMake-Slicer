#ifndef FDMSETTINGVISIBILITYCONFIG_H
#define FDMSETTINGVISIBILITYCONFIG_H

#include <QString>
#include <QStringList>
#include <QFile>
#include <QMultiHash>
#include <QTextStream>
#include <QDebug>
#include "fdmparamdata.h"

namespace fdmsettings {

using VisibilityConfig = QHash<QString,QStringList>;

class FdmSettingVisibilityConfig
{
public:
    explicit FdmSettingVisibilityConfig();

    QList<FdmParamNode *> loadList(FdmParamNode *root,const QString &filepath);

private:
    QStringList readConfigFile(const QString &filepath);

    void insertDatas(FdmParamNode *node, int level);

    void insertlevel(FdmParamNode *node, int level);
    void insertIcon(FdmParamNode *node);

    void print();

    QString remove(QString str);

private:
    VisibilityConfig configHash;
    QList<FdmParamNode *> nodeList;
    QStringList lineList;

};


}
#endif // FDMSETTINGVISIBILITYCONFIG_H

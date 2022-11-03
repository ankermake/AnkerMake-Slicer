#include "fdmqmlengine.h"

#include <QFileInfo>

QObject *FdmQmlEngine::loadQmlFile(const QUrl &qmlFile){
    const QList<QObject *> rootObjectsOld = this->rootObjects();
    this->load(qmlFile);
    const QList<QObject *> rootObjectsNow = this->rootObjects();

    QList<QObject *> rootObjectsNew;
    for(QObject * obj : rootObjectsNow){
        if(!rootObjectsOld.contains(obj)){
            rootObjectsNew.append(obj);
        }
    }

    //qDebug() << "rootObjectsOld" << rootObjectsOld;
    qDebug() << "rootObjectsNew" << rootObjectsNew;

    if(rootObjectsNew.size() == 1){
        return rootObjectsNew[0];
    }
    return nullptr;
}

QObject * FdmQmlEngine::loadQmlFile(const QString &fileName){
    QFileInfo info(fileName);
    if(!info.exists()){
        qWarning()  << "QFileInfo(path).exists()==" << info.exists() << fileName;
        return nullptr;
    }

    QString path;
    
    //path += "file:///";
    path += info.absoluteFilePath();
    //return loadQmlFile(QUrl(path));
    return loadQmlFile(QUrl::fromLocalFile(path));
}

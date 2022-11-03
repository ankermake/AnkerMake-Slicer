#ifndef FDMQMLENGINE_H
#define FDMQMLENGINE_H

#include <QQmlApplicationEngine>

/*================ class FdmQmlEngine loadQmlFile ================*/
class FdmQmlEngine : public QQmlApplicationEngine{
    using ThisType = FdmQmlEngine;
    using SuperType = QQmlApplicationEngine;
    using SuperType::SuperType;
public:
    static ThisType* instance(){
        static ThisType staticInstance;
        return &staticInstance;
    }
    QObject * loadQmlFile(const QUrl & qmlFile);
    QObject * loadQmlFile(const QString & qmlFile);
};

#endif // FDMQMLENGINE_H

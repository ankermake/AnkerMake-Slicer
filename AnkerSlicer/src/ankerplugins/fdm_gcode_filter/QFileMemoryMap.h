#ifndef QFILEMEMORYMAP_H
#define QFILEMEMORYMAP_H

#include <QFile>
#include <QDir>
#include <QObject>
#include <QDebug>
#include "QMemoryReference.h"


//  QFileMemoryMap ===============================================
class QFileMemoryMap
{
protected:
    QFileInfo m_info;
    QFile     m_file;
    MemRef    m_memRef;
    union{
        MemRef    r_cursor;
        MemRef    w_cursor;
    };
public:
    using SuperType = QFileMemoryMap;
    QFileMemoryMap(){}
    virtual ~QFileMemoryMap(){ close(); }
    virtual void close();
    virtual void remove();

    static bool touchFilePath(QString filePath, QIODevice::OpenMode mode = QIODevice::ReadOnly);

    const  QFileInfo & getQInfo()  const {return m_info     ;}
    const  QFile     & getQFile()  const {return m_file     ;}
    inline MemRef      getMemRef() const {return m_memRef   ;}

protected:
    //  read
    MemRef readMemoryFile (QString filePath);
    MemRef readCursor() { return !!m_memRef ? r_cursor : MemRef();}
    bool   readAlready(){ return !!m_memRef ; }

    //  write
    void   writeMemoryFile(QString filePath, qint64 capacity = 0);
    MemRef writeCursor(){return !!m_memRef ? r_cursor : MemRef();}
    bool   writeAlready(){ return (!!m_memRef) || m_file.isWritable(); }
    qint64 writeAppend(MemRef src);
    qint64 writeAllOneTime(QList<MemRef> srcList);
    void   writeFinish();  //  only call before close()

    QString __writeFileTest();
};

class QFileMemoryMapRead : public QFileMemoryMap{
public:
    using SuperType::readMemoryFile ;
    using SuperType::readAlready    ;
    using SuperType::__writeFileTest;
};

class QFileMemoryMapWrite : public QFileMemoryMap{
public:
    using SuperType::writeMemoryFile;
    using SuperType::writeAlready   ;
    using SuperType::writeAppend    ;
    using SuperType::writeAllOneTime ;
    using SuperType::writeFinish    ;
    bool toBeDelete{ false }        ;
};



#endif // QFILEMEMORYMAP_H

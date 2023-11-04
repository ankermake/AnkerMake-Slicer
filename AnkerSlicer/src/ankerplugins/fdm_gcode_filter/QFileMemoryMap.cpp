#include "QFileMemoryMap.h"
#include <QStandardPaths>
#include <QTime>

//  QFileMemoryMap ===============================================
bool QFileMemoryMap::touchFilePath(QString filePath, QIODevice::OpenMode mode)
{
    QFile file(filePath);
    bool res = false;

    if(mode & QIODevice::WriteOnly){
        if(!file.exists()){
            //QString relative = QDir::current().relativeFilePath(filePath);
            QFileInfo info(filePath);
            info.dir().mkpath(info.absolutePath());
        }

        if(file.open(mode | QIODevice::Truncate)){
            res = true;
        }
    }
    else if(mode & QIODevice::ReadOnly){
        if(file.exists() && file.size() > 0){
            if(file.open(mode)){
                res = true;
            }
        }
    }

    if(file.isOpen()){
        file.close();
    }
    return res;
}

void QFileMemoryMap::close(){
    if(m_file.isOpen()){
        if(!!m_memRef){
            bool b_unmap = m_file.unmap(m_memRef.quint8ptr);
            qDebug() << FUNC_LINE << (b_unmap ? "SUCCESS" : "ERROR") << ": QFile.unmap." << m_file << m_memRef.toString();
            m_memRef.clear();
        }
        if(!!w_cursor){
            w_cursor.clear();
        }
        m_file.close();
        qDebug() << FUNC_LINE << "CLOSE: " << m_file << m_file.size();
    }
}

void QFileMemoryMap::remove()
{
    SuperType::close();
    Q_ASSERT(!m_file.isOpen());
    m_file.remove();
}

MemRef QFileMemoryMap::readMemoryFile(QString filePath)
{
    m_info.setFile(filePath);
    if(touchFilePath(filePath, QIODevice::ReadOnly)){
        m_file.setFileName(filePath);
        if(m_file.open(QIODevice::ReadOnly)){
            m_memRef.quint8ptr = m_file.map(0, m_file.size());
            m_memRef.m_size = m_file.size();
        }
    }

    {   //  newLine   Must be executed immediately after QFile::map
        MemRef::SetNewLine(m_memRef);

        if(MemRef::NewLine.isEmpty()){
            const char * error = "[ERROR]:The newline error in the file";
            Q_ASSERT_X(false, "", error);
        }
    }

    qDebug() << FUNC_LINE << (readAlready() ? "SUCCESS" : "ERROR") << ": QFile.map." << m_file << m_memRef.toString() << m_file.isOpen() << m_file.isReadable();
    return m_memRef;
}

void QFileMemoryMap::writeMemoryFile(QString filePath, qint64 capacity)
{
    m_info.setFile(filePath);
    if(touchFilePath(filePath, QIODevice::WriteOnly)){
        m_file.setFileName(filePath);
        if(m_file.open(QIODevice::ReadWrite | QIODevice::Append)){
            if(capacity > 0 && m_file.resize(capacity) && m_file.size() == capacity){
                m_memRef.quint8ptr = m_file.map(0, capacity);
                if(m_memRef.ptr){
                    m_memRef.m_size = capacity;
                    w_cursor.ptr    = m_memRef.ptr;
                    w_cursor.m_size = 0;
                }
            }
        }
    }

    qDebug() << FUNC_LINE << (writeAlready() ? "SUCCESS" : "ERROR") << ": QFile.map." << m_file << m_memRef.toString() << m_file.isOpen() << m_file.isWritable();
}

qint64 QFileMemoryMap::writeAllOneTime(QList<MemRef> srcList)
{
    QTime runningTimer;
    double time_ms = runningTimer.restart();

    int srcListSize = srcList.size();
    int memBlockSize = srcListSize;// * 20;
    qintptr summation = 0;

    QList<qintptr> offsetList;
    offsetList.reserve(memBlockSize);

    for(int i = 0; i < memBlockSize; ++i){
        offsetList.append(summation);
        summation += srcList[i % srcListSize].m_size;
    }

    QFileMemoryMap::close();
    QFileMemoryMap::writeMemoryFile(m_file.fileName(), summation);
    // Parallel
    if( !! m_memRef ){

        if(OMP_GET_MAX_THREADS() > 1){
            #pragma omp parallel for schedule(dynamic)
            for(int i=0; i < memBlockSize; ++i){
                MemCharRef toWrite = srcList[i % srcListSize];
                int offset = offsetList[i];

                void * resptr = memcpy(m_memRef.begin() + offset, toWrite.begin(), toWrite.m_size);

                if(resptr == (m_memRef.begin() + offset)){
                    //qDebug() << FUNC_LINE << "OMP:" << OMP_GET_THREAD_NUM() << (resptr == (m_memRef.begin() + offset) ? "SUCCESS" : "ERROR") << ": memcpy" << toWrite.toString() << offset << resptr;
                    #pragma omp atomic
                    w_cursor.m_size += toWrite.m_size;
                }
                else{
                    qDebug() << FUNC_LINE << "OMP:" << OMP_GET_THREAD_NUM() << (resptr == (m_memRef.begin() + offset) ? "SUCCESS" : "ERROR") << ": memcpy" << toWrite.toString() << offset << resptr;
                }
            }
        }
        else{
            for(int i=0; i < memBlockSize; ++i){
                MemCharRef toWrite = srcList[i % srcListSize];
                int offset = offsetList[i];

                void * resptr = memcpy(m_memRef.begin() + offset, toWrite.begin(), toWrite.m_size);

                if(resptr == (m_memRef.begin() + offset)){
                    //qDebug() << FUNC_LINE << OMP_GET_MAX_THREADS() << (resptr == (m_memRef.begin() + offset) ? "SUCCESS" : "ERROR") << ": memcpy" << toWrite.toString() << offset << resptr;
                    w_cursor.m_size += toWrite.m_size;
                }
                else{
                    qDebug() << FUNC_LINE << OMP_GET_MAX_THREADS() << (resptr == (m_memRef.begin() + offset) ? "SUCCESS" : "ERROR") << ": memcpy" << toWrite.toString() << offset << resptr;
                }
            }
        }
        time_ms = runningTimer.restart();
        qDebug().nospace() << FUNC_LINE << "[INFO]: " << OMP_GET_MAX_THREADS() << " write file size = " << w_cursor.m_size << "B = " << (w_cursor.m_size / 1024.0 / 1024.0)
                           << "MB, time = "  << time_ms << " ms, speed = " << (time_ms == 0.0 ? 11.11 : (w_cursor.m_size / time_ms * 1000 / 1024 / 1024)) << "MB/s";
        return w_cursor.m_size;
    }
    // serial
    else if(m_file.isOpen() && m_file.isWritable()){

        for(int i=0; i < memBlockSize; ++i){
            MemCharRef toWrite = srcList[i];
            qint64 wb = m_file.write(toWrite.m_data, toWrite.m_size);
            if(wb == toWrite.m_size){
                w_cursor.m_size += toWrite.m_size;
            }
            else
                qDebug() << FUNC_LINE << (wb == toWrite.m_size ? "SUCCESS" : "ERROR") << ": QFile.write." << m_file << m_memRef.toString();
        }
        time_ms = runningTimer.restart();
        qDebug().nospace() << FUNC_LINE << "[INFO]: " << OMP_GET_THREAD_NUM() << " write file size = " << w_cursor.m_size << "B = " << (w_cursor.m_size / 1024.0 / 1024.0)
                           << "MB, time = "  << time_ms << " ms, speed = " << (time_ms == 0.0 ? 11.11 : (w_cursor.m_size / time_ms * 1000 / 1024 / 1024)) << "MB/s";
        return w_cursor.m_size;
    }

    return 0;
}


qint64 QFileMemoryMap::writeAppend(MemRef src)
{
    if(!!m_memRef){
        memcpy(w_cursor.quint8ptr, src.quint8ptr, src.m_size);
        w_cursor.quint8ptr += src.m_size;
        w_cursor.m_size += src.m_size;
        qDebug() << FUNC_LINE << (true ? "SUCCESS" : "ERROR") << ": memcpy" << m_file << m_memRef.toString();
        return src.m_size;
    }
    else if(m_file.isOpen() && m_file.isWritable()){
        qint64 wb = m_file.write(src.m_data, src.m_size);
        if(wb == src.m_size){
            w_cursor.m_size += src.m_size;
        }
        else
            qDebug() << FUNC_LINE << (wb == src.m_size ? "SUCCESS" : "ERROR") << ": QFile.write." << m_file << m_memRef.toString();

        return wb;
    }
    return 0;
}

void QFileMemoryMap::writeFinish()
{
    if(!!m_memRef && !!w_cursor){
        //m_file.resize(w_cursor.m_size);
    }
    if( m_file.isOpen() ){
        m_file.flush();
    }
}

QString QFileMemoryMap::__writeFileTest()
{
    //  OK  if(m_memRef){return m_info.dir().path() + "/123/456/" + m_info.fileName();}
    //  OK  return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/123/456/" + m_info.fileName();
    QString _filepath = m_info.filePath();
    return  _filepath.left(_filepath.lastIndexOf('.')) +  + "_ai.gcode";
}

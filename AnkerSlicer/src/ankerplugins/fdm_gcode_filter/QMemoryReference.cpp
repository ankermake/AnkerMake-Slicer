#include "QMemoryReference.h"
#include <QDebug>
#include <QTime>

const static QLatin1String _R_("\r");
const static QLatin1String _N_("\n");
const static QLatin1String _RN("\r\n");

QLatin1String MemRef::NewLine    = QLatin1String();
char          MemRef::NewLineEnd = 0x00;

//  After QFile::map
void MemRef::SetNewLine(MemFileRef srcFileRef)
{
    MemRef::NewLine = QLatin1String();
    MemRef::NewLineEnd = 0x00;
    for(int i = 0; i < srcFileRef.m_size; ++i){
        if( '\r' == srcFileRef.m_data[i] ){
            if('\n' == srcFileRef.m_data[srcFileRef.minSize(i+2)-1]){
                MemRef::NewLine = _RN;
                MemRef::NewLineEnd = '\n';
            }
            else{
                MemRef::NewLine = _R_;
                MemRef::NewLineEnd = '\r';
            }
            break;
        }
        if( '\n' == srcFileRef.m_data[i] ){
            MemRef::NewLine = _N_;
            MemRef::NewLineEnd = '\n';
            break;
        }
    }
}


char * __newLineUp (const MemFileRef &srcFileRef, const MemCharRef & local){
    if(!MemRef::NewLineEnd){ return srcFileRef.begin();}
    char * begin = srcFileRef.begin();
    char * pos = local.begin();
    while( begin < pos  &&  *(pos-1) != MemRef::NewLineEnd ){ --pos; }
    return pos;
}
char * __newLineDown (const MemFileRef &srcFileRef, const MemCharRef & local){
    if(!MemRef::NewLineEnd){ return srcFileRef.end();}
    char * end = srcFileRef.end();
    char * pos = local.end()-1;
    while( pos < end && *(pos++) != MemRef::NewLineEnd );
    return pos;
}


MemRef MemRef::FindChars(const MemRef &doc, const QLatin1String &sub_, bool bDownToUp, QList<MemRef> * findALL){
    const MemCharRef sub(sub_);
    if(doc.m_size < sub.m_size || sub.m_size == 0){
        return MemRef();
    }

    if(sub.m_size == 1 || sub.m_size == 2 || sub.m_size == 4 || sub.m_size == 8)
    {
        MemRef res;
        MemRef tmp = doc;
        while(true){
            char * begin = (tmp.m_data);
            char * end = (tmp.m_data + tmp.m_size  - sub.m_size);

            if(!bDownToUp){
                tmp.m_data = begin;
                switch (sub.m_size) {
                    case 1: { while(tmp.m_data <= end  ){ if(*sub. quint8ptr == *tmp. quint8ptr){ res.reset(tmp.m_data, sub.m_size); break; } ++tmp.m_data; } break; }
                    case 2: { while(tmp.m_data <= end  ){ if(*sub.quint16ptr == *tmp.quint16ptr){ res.reset(tmp.m_data, sub.m_size); break; } ++tmp.m_data; } break; }
                    case 4: { while(tmp.m_data <= end  ){ if(*sub.quint32ptr == *tmp.quint32ptr){ res.reset(tmp.m_data, sub.m_size); break; } ++tmp.m_data; } break; }
                    case 8: { while(tmp.m_data <= end  ){ if(*sub.quint64ptr == *tmp.quint64ptr){ res.reset(tmp.m_data, sub.m_size); break; } ++tmp.m_data; } break; }
                }
            }
            else{
                tmp.m_data = end;
                switch (sub.m_size) {
                    case 1: { while(begin <= tmp.m_data){ if(*sub. quint8ptr == *tmp. quint8ptr){ res.reset(tmp.m_data, sub.m_size); break; } --tmp.m_data; } break; }
                    case 2: { while(begin <= tmp.m_data){ if(*sub.quint16ptr == *tmp.quint16ptr){ res.reset(tmp.m_data, sub.m_size); break; } --tmp.m_data; } break; }
                    case 4: { while(begin <= tmp.m_data){ if(*sub.quint32ptr == *tmp.quint32ptr){ res.reset(tmp.m_data, sub.m_size); break; } --tmp.m_data; } break; }
                    case 8: { while(begin <= tmp.m_data){ if(*sub.quint64ptr == *tmp.quint64ptr){ res.reset(tmp.m_data, sub.m_size); break; } --tmp.m_data; } break; }
                }
            }

            if(!!res){
                if(findALL){
                    (*findALL).append( res );
                    tmp = (!bDownToUp) ? doc.docCutDown(res) : doc.docCutUp(res);
                    res.clear();
                    continue;   // continue while(true)
                }
            }
            break;  // break while(true)
        }   // end of while(true)

        if(findALL && findALL->size() > 0){
            res = findALL->first();  // bDownToUp ? findALL->last(): findALL->first();
        }

        return res;
    }

    if( !(sub.m_size == 1 || sub.m_size == 2 || sub.m_size == 4 || sub.m_size == 8) )
    {
        MemRef res;
        const int j_sta = -1, j_end = doc.m_size;   // [ ]
        const int k_sta = -1, k_end = sub.m_size;       // ( )
        const int off   = (!bDownToUp) ? +1 : -1;

        int j = (!bDownToUp) ? j_sta : j_end;
        int k = (!bDownToUp) ? k_sta : k_end;
        while(true){

            if(!bDownToUp){
                // up to down
                while ( j < j_end && k < k_end) {
                    if (k == k_sta || doc.m_data[j] == sub.m_data[k]) {
                        ++j;
                        ++k;
                    } else {
                        k = k_sta;
                    }
                }
            }
            else{
                // down to up
                while ( j_sta < j && k_sta < k ) {
                    if (k == k_end || doc.m_data[j] == sub.m_data[k]) {
                        --j;
                        --k;
                    } else {
                        k = k_end;
                    }
                }
            }

            if( (!bDownToUp && k == k_end) || (bDownToUp && k == k_sta)){
                j -= off;
                res.reset(doc.m_data + ( (!bDownToUp) ? (j - (k - 1)) : j ), sub.m_size);
                if(findALL){
                    (*findALL).append( res );
                    k = (!bDownToUp) ? k_sta : k_end;
                    res.clear();
                    continue;   // continue while(true)
                }
            }
            break;  // break while(true)
        }// end of while(true)

        if(findALL && findALL->size() > 0){
            res = findALL->first();  // bDownToUp ? findALL->last(): findALL->first();
        }

        return res;
    }

    return MemRef();
}


MemLineRef MemRef::DocFindLine(const MemDocuRef &doc, QLatin1String token, bool bDownToUp)
{
    MemCharRef find = FindChars(doc, token, bDownToUp);
    MemLineRef line = GetLocalLine(doc, find);
    return line;
}

QList<MemCharRef> MemRef::DocFindLineAll(const MemDocuRef &doc, QLatin1String token, bool tryParallel)
{
    QTime runningTimer;
    double time_ms = runningTimer.restart();

    do{
        int memBlockSize  = std::max(512 * 1024,  token.size() * 1024);
        int memBlockCount = (doc.m_size + memBlockSize - 1) / memBlockSize;

        int MaxThreads = OMP_GET_MAX_THREADS();
        MaxThreads = std::min( MaxThreads , memBlockCount);

        qDebug() << FUNC_LINE << "MaxThreads =" << MaxThreads << ", memBlockSize =" << memBlockSize << ", memBlockCount =" << memBlockCount;
        if( !tryParallel || MaxThreads <= 1 ){ break; }

        // parallel
        QVector<QList<MemLineRef>> allLineListList(memBlockCount, QList<MemLineRef>());

        #pragma omp parallel for schedule(dynamic)
        for(int k = 0; k < memBlockCount; ++k){
            QList<MemLineRef> & lineList = allLineListList[k];
            lineList.reserve(250 * 10 / memBlockCount + 10);
            char * sta = doc.begin() + memBlockSize * k;
            char * end = std::min(sta + memBlockSize + token.size() - 1, doc.end());
            MemDocuRef subDoc( sta, static_cast<int>(end - sta) );

            if( !! FindChars(subDoc, token, false, &lineList) ){
                for(int i=0; i < lineList.size(); ++i){
                    lineList[i] = GetLocalLine(doc, lineList[i]);
                }
            }
            //qDebug() << FUNC_LINE << "OMP:" << omp_get_thread_num() << subDoc.toString() << ": FindChars" << lineList.size();
        }

        int lineCount=0;
        for(const auto & l : allLineListList)   {lineCount   += l.size();}

        QList<MemLineRef> allLineList; allLineList.reserve(lineCount);
        for(const auto & l : allLineListList)   {allLineList += l;}


        time_ms = runningTimer.restart();time_ms = std::max(time_ms, 1.001);
        qDebug().nospace() << FUNC_LINE << " [INFO]: " << OMP_GET_MAX_THREADS() << " read file size = " << doc.m_size << "B = " << (doc.m_size / 1024.0 / 1024.0)
                           << "MB, time = "  << time_ms << " ms, speed = " << (time_ms == 0.0 ? 11.11 : (doc.m_size / time_ms * 1000 / 1024 / 1024)) << "MB/s";
        return allLineList;
    }while(false);

    // serial
    QList<MemLineRef> allLine; allLine.reserve(1024);
    if( !! FindChars(doc, token, false, &allLine) ){
        for(int i=0; i < allLine.size(); ++i){
            allLine[i] = GetLocalLine(doc, allLine[i]);
        }
    }
    time_ms = runningTimer.restart(); time_ms = std::max(time_ms, 1.001);
    qDebug().nospace() << FUNC_LINE << " [INFO]: " << OMP_GET_MAX_THREADS() << " read file size = " << doc.m_size << "B = " << (doc.m_size / 1024.0 / 1024.0)
                       << "MB, time = "  << time_ms << " ms, speed = " << (doc.m_size / time_ms * 1000 / 1024 / 1024) << "MB/s";
    return allLine;
}

QList<MemLineRef> MemRef::DocFindLineAll_Zebra(const MemDocuRef &doc, QLatin1String token)
{
    QList<MemLineRef> tokenLines = MemRef::DocFindLineAll(doc, token);
    QList<MemLineRef> zebra; zebra.reserve(tokenLines.size() * 2 + 1);

    MemCharRef noMatch = doc;
    for(int i = 0; i < tokenLines.size(); ++i){
        MemLineRef layer = tokenLines[i];

        noMatch = noMatch.docCutUp(layer);

        zebra.append(noMatch);
        zebra.append(layer  );

        noMatch = doc.docCutDown(layer);
    }
    zebra.append(noMatch);

    return zebra;
}


//  firstButNotInclude == token
QList<MemLineRef> MemRef::DocFindLineAll_Zebra(const MemDocuRef &doc, QLatin1String token, QLatin1String firstButNotInclude, QLatin1String finishNotMatchButInclude)
{
    QList<MemLineRef> tokenLines = MemRef::DocFindLineAll(doc, token);
    if(tokenLines.size() <= 0){
        return tokenLines;
    }

    MemLineRef first  =  MemRef::DocFindLine(doc, firstButNotInclude,      false);
    MemLineRef finish =  MemRef::DocFindLine(doc, finishNotMatchButInclude, true );

    bool b_firstButNotInclude       = !!first   && first.begin() == tokenLines.first().begin();
    bool b_finishNotMatchButInclude = !!finish  && finish.begin() > tokenLines.last ().begin();

    if(b_firstButNotInclude){
        tokenLines.removeAt(0);
    }
    if(b_finishNotMatchButInclude){
        tokenLines.append(finish);
    }

    QList<MemLineRef> zebra; zebra.reserve(tokenLines.size() * 2 + 1);

    MemCharRef noMatch = doc;
    for(int i = 0; i < tokenLines.size(); ++i){
        MemLineRef layer = tokenLines[i];

        noMatch = noMatch.docCutUp(layer);

        zebra.append(noMatch);
        zebra.append(layer  );

        noMatch = doc.docCutDown(layer);
    }
    zebra.append(noMatch);

    return zebra;
}




MemLineRef MemRef::GetLocalLine(const MemDocuRef &doc, const MemCharRef &local)
{
    if(!MemRef::NewLineEnd)              { return MemLineRef();}
    if(!MemRef::MemContains(doc, local) ){ return MemLineRef();}

    char * handHead = __newLineUp  (doc, local);
    char * handTail = __newLineDown(doc, local);

    return MemLineRef(handHead, handTail-handHead);
}

MemLineRef MemRef::GetLocalLineUp(const MemDocuRef &doc, const MemCharRef &local)
{
    if(!MemRef::NewLineEnd)              { return MemLineRef();}
    MemLineRef line = MemRef::GetLocalLine(doc, local);
    if( !line )                          { return MemLineRef();}
    MemCharRef temp = MemLineRef(line.begin()-1, 1);    // up char
    if(!MemRef::MemContains(doc, temp) ) { return MemLineRef();}

    char * handHead = __newLineUp  (doc, temp);
    char * handTail = __newLineDown(doc, temp);

    return MemLineRef(handHead, handTail-handHead);
}

MemLineRef MemRef::GetLocalLineDown(const MemDocuRef &doc, const MemCharRef &local)
{
    if(!MemRef::NewLineEnd)              { return MemLineRef();}
    MemLineRef line = MemRef::GetLocalLine(doc, local);
    if( !line )                          { return MemLineRef();}
    MemCharRef temp = MemLineRef(line.end(), 1);    // down char
    if(!MemRef::MemContains(doc, temp) ) { return MemLineRef();}

    char * handHead = __newLineUp  (doc, temp);
    char * handTail = __newLineDown(doc, temp);

    return MemLineRef(handHead, handTail-handHead);
}


//  static  sub MemCharRef in MemFileRef
bool MemRef::MemContains(MemFileRef src, MemCharRef sub){
    return (!!src && !!sub && src.begin() <= sub.begin()) &&  sub.end() <= src.end();
}

bool MemRef::FixInBounds(MemFileRef src, MemCharRef &sub){
    char *begin = std::max(src.begin(), sub.begin());
    char *end   = std::min(src.end()  , sub.end()  );
    MemCharRef temp(begin, end - begin);
    if(MemRef::MemContains(src, temp)){
        sub = temp;
        return true;
    }
    return false;
}


MemFileRef MemFileRef::GetCut(MemFileRef src, MemFileRef cutPos, bool bKeepLeft, bool bInclude){
    Q_ASSERT(MemFileRef::MemContains(src, cutPos));

    MemRef res;
    if(bKeepLeft){
        char * begin = src.begin();
        int    len = int( (bInclude ? cutPos.end() : cutPos.begin()) - begin);
        res.reset(begin, len);
    }
    else{ // bKeepRight
        char * begin = bInclude ? cutPos.begin() : cutPos.end();
        int    len = int(src.end() - begin);
        res.reset(begin, len);
    }
    return res;
}

//  debug
QString MemRef::toString() const {
    return QString("[0x%1:%2]").arg((qintptr)ptr, 8, 16, QChar('0')).arg(m_size, 8, 10, QChar(' '));}

QString MemRef::toString2() const { return QString("[0x%1:%2]%3").arg((qintptr)ptr, 8, 16, QChar('0')).arg(m_size, 8, 10, QChar(' ')).arg(!!(*this) ? QString(toQByteArray()) : QString("Empty!"));}

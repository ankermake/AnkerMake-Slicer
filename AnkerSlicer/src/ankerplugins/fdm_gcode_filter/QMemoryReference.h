#ifndef QMEMORYREFERENCE_H
#define QMEMORYREFERENCE_H

#include <QObject>
#include <cstddef>

#ifdef GCODE_FILTER_OMP
#include <omp.h>
#define OMP_GET_THREAD_NUM()  omp_get_thread_num()
#define OMP_GET_MAX_THREADS() omp_get_max_threads()
//#pragma omp parallel for schedule(dynamic)
//#pragma omp atomic
#else
#define OMP_GET_THREAD_NUM()  (-1)
#define OMP_GET_MAX_THREADS() (+1)
#endif

#define FUNC_LINE "[" << __LINE__ << ":" << __FUNCTION__ << "]"


//  MemRef ===============================================
struct MemRef;
using  MemDocuRef = MemRef;
using  MemFileRef = MemRef;
using  MemLineRef = MemRef;
using  MemCharRef = MemRef;

struct MemRef
{
    using ThisType = MemRef;
    //  struct data like QLatin1String
    union {
        struct
        {
            int m_size;  // qint64  qintptr
            union{
                void    * ptr;
                char    * m_data;
                quint8  *  quint8ptr;
                quint16 * quint16ptr;
                quint32 * quint32ptr;
                quint64 * quint64ptr;
            };
        };
        QLatin1String latin;
    };

    //  init and hlep
    MemRef(void * p=nullptr, int l=0):ptr(p),m_size(l){}
    inline void reset(void * p, int l){ptr = p; m_size = l;}
    inline void clear(){reset(nullptr, 0);}
    inline bool operator !()  const {return !(ptr && m_size>0);}
    inline char * begin()     const {return m_data;}
    inline char * end  ()     const {return m_data+m_size;}
    inline int  minSize(int n)const {return m_size < n ? m_size : n;}

    //  static newline token
    static QLatin1String NewLine;
    static char          NewLineEnd;
    static void          SetNewLine(MemFileRef srcFileRef);

    //  QByteArray
    explicit MemRef(QByteArray bytes):m_data(bytes.data()),m_size(bytes.size()){}
    QByteArray toQByteArray() const {return QByteArray(m_data, m_size);}
    std::string toStdString() const {return std::string(m_data, m_size);}

    //  QLatin1String
    MemRef(const QLatin1String & bytes): latin(bytes){}
    MemRef(      QLatin1String &&bytes): latin(bytes){}
    inline operator QLatin1String()   const noexcept{return latin; }
    inline QLatin1String operator()() const noexcept{return latin; }

    //  static  memContains, fixInBounds,  sub MemCharRef in MemFileRef
    static bool MemContains (MemFileRef src, MemCharRef  sub);
    static bool FixInBounds (MemFileRef src, MemCharRef& sub);


    //  src and cutPos have to be in the MemDocRef/MemFileRef
    static MemFileRef GetCut(MemFileRef src, MemLineRef cutPos, bool bKeepLeft, bool bInclude);
    inline MemFileRef docCutUp          (MemLineRef cutPos) const {return MemFileRef::GetCut(*this, cutPos, true , false);}
    inline MemFileRef docCutUpInclude   (MemLineRef cutPos) const {return MemFileRef::GetCut(*this, cutPos, true , true );}
    inline MemFileRef docCutDown        (MemLineRef cutPos) const {return MemFileRef::GetCut(*this, cutPos, false, false);}
    inline MemFileRef docCutDownInclude (MemLineRef cutPos) const {return MemFileRef::GetCut(*this, cutPos, false, true );}

    inline MemCharRef left  (int n) { n = minSize(n); return MemFileRef(begin(), n);}
    inline MemCharRef right (int n) { n = minSize(n); return MemFileRef(end()-n, n);}
    inline MemCharRef mid   (int pos, int n = -1) { pos = minSize(pos);  n = std::min((uint)(m_size - pos), uint(n) ); return MemFileRef(begin() + pos, n);}

    //  memmory operator + -  Under the class
    //  MemCharRef has to be in the MemFileRef
    //  inline static QByteArray operator +(const QLatin1String & l, const QLatin1String & r);
    //  inline static MemCharRef operator +(const MemCharRef &l, const MemCharRef &r);
    //  inline static MemCharRef operator +(MemCharRef l, int r);
    //  inline static MemCharRef operator +(int l, MemCharRef r);
    //  inline static MemCharRef operator -(MemCharRef l, int r);
    //  inline static MemCharRef operator -(int l, MemCharRef r);


    //  chars view reference,  find (MemCharRef)sub  in  MemCharRef/MemLineRef/MemFileRef
    static MemCharRef FindChars(const MemRef & src, const QLatin1String & sub, bool bDownToUp = false, QList<MemRef> * findALL = nullptr); //  sub == find string
    inline bool       contains(                     const QLatin1String & sub) { return !!FindChars(*this, sub);}
    inline MemCharRef findChars(                    const QLatin1String & sub) { return   FindChars(*this, sub, false);}
    inline MemCharRef findLastChars(                const QLatin1String & sub) { return   FindChars(*this, sub, true );}
    inline MemCharRef findCharsAll(                 const QLatin1String & sub, QList<MemRef> * findALL) { Q_ASSERT(findALL); return FindChars(*this, sub, false, findALL);}
    inline MemCharRef findLastCharsAll(             const QLatin1String & sub, QList<MemRef> * findALL) { Q_ASSERT(findALL); return FindChars(*this, sub, true , findALL);}
    inline bool       startsWith(                   const QLatin1String & sub) { return !(operator !()) && FindChars(*this, sub, false).begin() == this->begin();}
    inline bool       endsWith(                     const QLatin1String & sub) { return !(operator !()) && FindChars(*this, sub, true ).end  () == this->end  ();}
    inline bool       startWith(                    const char sub)            { return !(operator !()) && sub == *begin();}
    inline bool       endWith(                      const char sub)            { return !(operator !()) && sub == *(end()-1);}

    //  line view reference, in
    static MemLineRef DocFindLine(const MemDocuRef &doc,  QLatin1String token, bool bDownToUp = false);
    inline MemLineRef docFindLine    ( QLatin1String token) const { return this->docFindLineDown(token);}
    inline MemLineRef docFindLineUp  ( QLatin1String token) const { return DocFindLine(*this, token, true ); }
    inline MemLineRef docFindLineDown( QLatin1String token) const { return DocFindLine(*this, token, false); }

    static QList<MemLineRef> DocFindLineAll      (const MemDocuRef &doc, QLatin1String token, bool tryParallel = true);
    static QList<MemLineRef> DocFindLineAll_Zebra(const MemDocuRef &doc, QLatin1String token);
    static QList<MemLineRef> DocFindLineAll_Zebra(const MemDocuRef &doc, QLatin1String token, QLatin1String firstButNotIncluded, QLatin1String finishNotMatchButInclude);


    //  get local line, after FindChars()
    static MemLineRef GetLocalLine      (const MemDocuRef &doc, const MemCharRef & localCharsOrLine);
    static MemLineRef GetLocalLineUp    (const MemDocuRef &doc, const MemCharRef & localCharsOrLine);
    static MemLineRef GetLocalLineDown  (const MemDocuRef &doc, const MemCharRef & localCharsOrLine);

    inline MemLineRef docLocalLine      (const MemCharRef & local){ return GetLocalLine    (*this, local);}
    inline MemLineRef docLocalLineUp    (const MemCharRef & local){ return GetLocalLineUp  (*this, local);}
    inline MemLineRef docLocalLineDown  (const MemCharRef & local){ return GetLocalLineDown(*this, local);}



    //  debug
    QString toString() const;
    QString toString2() const;
};


//  memmory operator + -  MemCharRef has to be in the MemFileRef
inline static QByteArray operator +(const QLatin1String & l, const QLatin1String & r){  //  Requires local variables
    return (QByteArray(l.data()) + QByteArray(r.data()));
}

inline static MemCharRef operator +(const MemCharRef &l, const MemCharRef &r){
    Q_ASSERT(!!l || !!r);
    if( !l ){ return r; }
    if( !r ){ return l; }

    if(l.end() == r.begin()){
        return MemCharRef(l.begin(), r.end() - l.begin());
    }
    else if(r.end() == l.begin()){
        return MemCharRef(r.begin(), l.end() - r.begin());
    }
    else{
        return MemCharRef();
        // throw std::exception();
    }
}

//  MemCharRef has to be in the MemFileRef
inline static MemCharRef operator +(MemCharRef l, int r){ return MemCharRef(l.m_data    , l.m_size + r); }
inline static MemCharRef operator +(int l, MemCharRef r){ return MemCharRef(r.m_data - l, l + r.m_size); }
inline static MemCharRef operator -(MemCharRef l, int r){ return operator +( l, -r); }
inline static MemCharRef operator -(int l, MemCharRef r){ return operator +(-l,  r); }

#endif // QMEMORYREFERENCE_H

#ifndef AKPOINT3I_H
#define AKPOINT3I_H
#include <QVector3D>
#include <cmath>
#include <QStringList>
#include "../GeoAndShow/CHMeshShowObj.h"

using AkInt = qint64;
class AkPoint3i
{
public:
    static const int scaleFactor = 1000;
    
    CHMeshShowObj* m_meshPtr = nullptr;

    AkPoint3i():v{0,0,0}{}
    AkPoint3i(AkInt x, AkInt y, AkInt z):v{x, y, z}{}

    QVector3D toVector3D() const
    {
        float scale = scaleFactor;
        return QVector3D(v[0] / scale, v[1] / scale, v[2] / scale);
    }
    static AkPoint3i fromVector3D(const QVector3D& v)
    {
        int scale = scaleFactor;
        return AkPoint3i(v.x()*scale, v.y()*scale, v.z()*scale);
    }
    friend inline AkPoint3i operator+(const AkPoint3i& p1, const AkPoint3i& p2);
    friend inline AkPoint3i operator-(const AkPoint3i& p1, const AkPoint3i& p2);
    friend inline AkPoint3i operator*(const AkPoint3i& p1, AkInt factor);
    friend inline AkPoint3i operator/(const AkPoint3i& p1, AkInt factor);

    inline AkInt length() const
    {
        return (AkInt)std::sqrt(length2());
    }
    inline AkInt length2() const
    {
         return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    }
    bool lessThan(AkInt len)
    {
        if ( v[0] > len ||  v[0] < -len)
            return false;
        if ( v[1] > len ||  v[1] < -len)
            return false;
        if ( v[2]> len ||  v[2] < -len)
            return false;
        return length2() <= len * len;
    }
//    inline static AkInt dotProduct(const AkPoint3i& v1, const AkPoint3i& v2)
//    {
//        return v1.v[0] * v2.v[0] + v1.v[1] * v2.v[1] + v1.v[2] * v2.v[2];
//    }
//    inline static AkPoint3i crossProduct(const AkPoint3i& v1, const AkPoint3i& v2)
//    {
//        return AkPoint3i(v1.v[1] * v2.v[2] - v1.v[2] * v2.v[1],
//                         v1.v[2] * v2.v[0] - v1.v[0] * v2.v[2],
//                         v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0]);
//    }

    inline AkInt dotProduct(const AkPoint3i& v2) const
    {
        return v[0] * v2.v[0] + v[1] * v2.v[1] + v[2] * v2.v[2];
    }
    inline AkPoint3i crossProduct(const AkPoint3i& v2) const
    {
        return AkPoint3i(v[1] * v2.v[2] - v[2] * v2.v[1],
                         v[2] * v2.v[0] - v[0] * v2.v[2],
                         v[0] * v2.v[1] - v[1] * v2.v[0]);
    }

    AkPoint3i normalized(AkInt len)
    {
        AkInt t = length();
        if (t < 1) return AkPoint3i(len, 0, 0);
        return *this * len / t;
    }

    
    inline AkInt getHashKey()
    {
        //2^63
        AkInt hash = v[0] ^ (v[1] << 21) ^ (v[2] << 42);
        return hash;
    }

    //inline QString toString()
    //{
    
    //    //return QString("%1_%2_%3").arg(x()).arg(y()).arg(z());
    //    QStringList list;
    //    list << QString::number(v[0]);
    //    list << QString::number(v[1]);
    //    list << QString::number(v[2]);
    //    return list.join("_");
    //}

    static AkPoint3i parseKey(QString key)
    {
        AkPoint3i point(-999,-999,-999);
        auto xyz = key.split("_", Qt::SkipEmptyParts);
        point.setX(xyz[0].toInt());
        point.setY(xyz[1].toInt());
        point.setZ(xyz[2].toInt());
        return point;

//        AkPoint3i point(-999,-999,-999);
//        try
//        {
//            auto xyz = key.split("_", Qt::SkipEmptyParts);
//            point.setX(xyz[0].toInt());
//            point.setY(xyz[1].toInt());
//            point.setZ(xyz[2].toInt());
//            return point;
//        }
//        catch(...)
//        {
//            return point;
//        }
//        return point;
    }

    inline AkInt x() const { return v[0];}
    inline void setX(AkInt newX) { v[0] = newX;}
    inline AkInt y() const{return v[1];}
    inline void setY(AkInt newY){ v[1] = newY;}
    inline AkInt z() const{return v[2];}
    inline void setZ(AkInt newZ){v[2] = newZ;}
private:
    AkInt v[3];

};


inline AkPoint3i operator+(const AkPoint3i& p1, const AkPoint3i& p2)
{
    return AkPoint3i(p1.x() + p2.x(), p1.y() + p2.y(), p1.z() + p2.z());
}
inline AkPoint3i operator-(const AkPoint3i& p1, const AkPoint3i& p2)
{
    return AkPoint3i(p1.x() - p2.x(), p1.y() - p2.y(), p1.z() - p2.z());
}
inline AkPoint3i operator*(const AkPoint3i& p1, AkInt factor)
{
    return AkPoint3i(p1.x() * factor, p1.y() * factor, p1.z() * factor);
}
inline AkPoint3i operator/(const AkPoint3i& p1, AkInt factor)
{
    return AkPoint3i(p1.x() / factor, p1.y() / factor, p1.z() / factor);
}




#endif // AKPOINT3I_H

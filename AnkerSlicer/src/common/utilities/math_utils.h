#pragma once
#include "akUtilSpace.h"
#include <QVector3D>
#include <cmath>
#include <stdlib.h>
#include "AkPoint3i.h"
AK_UTIL_NAME_SPACE_BEGIN
#define FLOAT_ZERO 0.000001f
using AkInt = qint64;

class MathUtils
{
    
    static const int scale = 1000;
public:
    inline static bool fuzzyCompare(float v1, float v2, float fZero = FLOAT_ZERO)
    {
        return abs(v1 - v2) < fZero;
    }

    inline static bool fuzzyCompare(const QVector3D &v1, const QVector3D& v2)
    {
        return  fuzzyCompare(v1.x(), v2.x()) && fuzzyCompare(v1.y(), v2.y()) && fuzzyCompare(v1.z(), v2.z());
    }

    
    inline static QString getHashKey(const QVector3D& v1)
    {
        return QString("%1_%2_%3").arg((int)(v1.x() * scale)).arg((int)(v1.y() * scale)).arg((int)(v1.z() * scale));
    }

    
    inline static QVector3D getfuzzyVector3D(const QVector3D& pointF)
    {
        int x = pointF.x() * scale;
        int y = pointF.y() * scale;
        int z = pointF.z() * scale;
        return QVector3D(x / (float)scale, y / (float)scale, z / (float)scale);
    }
    template<class T>
    static void swap(T& p0, T& p1)
    {
        T tmp = p0;
        p0 = p1;
        p1 = tmp;
    }

    
    static bool pointInTriangle(const QVector3D& a, const QVector3D& b, const QVector3D& c,const QVector3D& p);
    
    static bool isLine(const QVector3D& p0, const QVector3D& p1, const QVector3D& p2);
    static bool isPointOnFace(const QVector3D& p0, const QVector3D& v0, const QVector3D& v1, const QVector3D& v2, const QVector3D& Normal, float D);
    
    static bool isLineCrossFace(const QVector3D& p0, const QVector3D& p1, const QVector3D& v0, const QVector3D& v1, const QVector3D& v2, const QVector3D& Normal, float D, QVector3D& interPt, bool includeP0, bool includeP1);
    static bool isLineCrossFace(const QVector3D& p0, const QVector3D& p1, const QVector3D& v0, const QVector3D& v1, const QVector3D& v2, QVector3D& interPt, bool includeP0 = true, bool includeP1 = true);
    
    static bool isLineCrossFace(const QVector3D &p0, const QVector3D& p1, const QVector3D& v0, const QVector3D& v1, const QVector3D& v2, bool includeP0 = true, bool includeP1 = true);
    static QVector3D goThoughVector(const QVector3D& point,const QVector3D & normal, float length);;

    
    
    static bool pointInTriangle(const AkPoint3i& a, const AkPoint3i& b, const AkPoint3i& c,const AkPoint3i& p);
    
    static bool isLine(const AkPoint3i& p0, const AkPoint3i& p1, const AkPoint3i& p2);
    static bool isPointOnFace(const AkPoint3i& p0, const AkPoint3i& v0, const AkPoint3i& v1, const AkPoint3i& v2, const AkPoint3i& Normal, float D);
    
    static bool isLineCrossFace(const AkPoint3i& p0, const AkPoint3i& p1, const AkPoint3i& v0, const AkPoint3i& v1, const AkPoint3i& v2, const AkPoint3i& Normal, float D, AkPoint3i& interPt, bool includeP0, bool includeP1);
    static bool isLineCrossFace(const AkPoint3i& p0, const AkPoint3i& p1, const AkPoint3i& v0, const AkPoint3i& v1, const AkPoint3i& v2, AkPoint3i& interPt, bool includeP0 = true, bool includeP1 = true);
    
    static bool isLineCrossFace(const AkPoint3i &p0, const AkPoint3i& p1, const AkPoint3i& v0, const AkPoint3i& v1, const AkPoint3i& v2, bool includeP0 = true, bool includeP1 = true);
    static AkPoint3i goThoughVector(const AkPoint3i& point,const AkPoint3i & normal, AkInt length);

    static AkInt area(const AkPoint3i& v0, const AkPoint3i& v1, const AkPoint3i& v2);
};
AK_UTIL_NAME_SPACE_END

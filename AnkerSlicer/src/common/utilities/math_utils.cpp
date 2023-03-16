#include "math_utils.h"
AK_UTIL_NAME_SPACE_BEGIN

bool MathUtils::pointInTriangle(const QVector3D &a, const QVector3D &b, const QVector3D &c, const QVector3D &p)
{
    auto v0 = c - a;
    auto v1 = b - a;
    auto v2 = p - a;

    auto dot00 = QVector3D::dotProduct(v0, v0);
    auto dot01 = QVector3D::dotProduct(v0, v1);
    auto dot02 = QVector3D::dotProduct(v0, v2);
    auto dot11 = QVector3D::dotProduct(v1, v1);
    auto dot12 = QVector3D::dotProduct(v1, v2);

    auto value = (dot00 * dot11 - dot01 * dot01);
    if (fuzzyCompare(value, 0))
    {
        return false;
    }

    auto inverDeno = 1 / value;

    auto u = (dot11 * dot02 - dot01 * dot12) * inverDeno;
    if (u < 0 || u > 1) // if u out of range, return directly
    {
        return false;
    }

    auto v = (dot00 * dot12 - dot01 * dot02) * inverDeno;
    if (v < 0 || v > 1) // if v out of range, return directly
    {
        return false;
    }

    return u + v <= 1;
}

bool MathUtils::isLine(const QVector3D &p0, const QVector3D &p1, const QVector3D &p2)
{
    auto vector1 = p0 - p1;
    auto vector2 = p2 - p1;
    auto diff0 = vector1.normalized();
    auto diff1 = vector2.normalized();
    
    float d = QVector3D::dotProduct(diff0, diff1);
    return (d < -0.9999f);
}

bool MathUtils::isPointOnFace(const QVector3D &p0, const QVector3D &v0, const QVector3D &v1, const QVector3D &v2, const QVector3D &Normal, float D)
{
    auto dP0 = std::abs(Normal.x() * p0.x() + Normal.y() * p0.y() + Normal.z() * p0.z() + D);
    auto permitDiff = 0.200f;
    
    if (dP0 < permitDiff)
    {
        
        if (dP0 < permitDiff)
        {
            bool p0InTriangle = pointInTriangle(v0, v1, v2, p0);
            if (p0InTriangle)
            {
                return true;
            }
        }
    }
    
    return false;
}

bool MathUtils::isLineCrossFace(const QVector3D &p0, const QVector3D &p1, const QVector3D &v0, const QVector3D &v1, const QVector3D &v2, const QVector3D &Normal, float D, QVector3D &interPt, bool includeP0, bool includeP1)
{
    

    auto dP0 = std::abs(Normal.x() * p0.x() + Normal.y() * p0.y() + Normal.z() * p0.z() + D);
    auto dP1 = std::abs(Normal.x() * p1.x() + Normal.y() * p1.y() + Normal.z() * p1.z() + D);

    auto permitDiff = 0.010;//
    
    if (dP0 < permitDiff || dP1 < permitDiff)
    {
        
        if (dP0 < permitDiff && includeP0)
        {
            bool p0InTriangle = pointInTriangle(v0, v1, v2, p0);
            if (p0InTriangle)
            {
                interPt.setX(p0.x());
                interPt.setY(p0.y());
                interPt.setZ(p0.z());
                return true;
            }
        }
        if (dP1 < permitDiff && includeP1)
        {
            bool p1InTriangle = pointInTriangle(v0, v1, v2, p1);
            if (p1InTriangle)
            {
                interPt.setX(p1.x());
                interPt.setY(p1.y());
                interPt.setZ(p1.z());
                return true;
            }
        }
        
        return false;
    }

    
    auto V = (p1 - p0).normalized();
    //V.Normalize();

    
    auto s = QVector3D::dotProduct(Normal.normalized(), V);

    
    if (std::abs(s) < FLOAT_ZERO)
    {
        return false;
    }

    //double q = -D - Normal.Dot(p0);
    auto t = (-D - QVector3D::dotProduct(Normal,p0)) / s;

    //Point3 interPt = new Point3();
    
    interPt.setY(p0.y() + t * V.y());
    interPt.setZ(p0.z() + t * V.z());
    interPt.setX(p0.x() + t * V.x());

    
    bool ptInTriangle = pointInTriangle(v0, v1, v2, interPt);
    
    auto dot = QVector3D::dotProduct((p1 - interPt), (p0 - interPt));
    
    if (dot <= 0 && ptInTriangle)
    {
        return true;
    }

    return false;
}

bool MathUtils::isLineCrossFace(const QVector3D &p0, const QVector3D &p1, const QVector3D &v0, const QVector3D &v1, const QVector3D &v2, QVector3D &interPt, bool includeP0, bool includeP1)
{
    
    //Point3 Normal = (v2 - v0).Cross(v1 - v0).Normalize(SlashConst.Scale);
    //auto Normal = QVector3D::normal(v2 - v0, v1 - v0);
    auto Normal = QVector3D::normal(v1 - v0, v2 - v0);
    //Normal.Normalize();
    auto d = -(Normal.x() * v0.x() + Normal.y() * v0.y() + Normal.z() * v0.z());
    bool cross = isLineCrossFace(p0, p1, v0, v1, v2, Normal, d, interPt, includeP0, includeP1);
    if (cross)
    {
        if (!includeP0 && fuzzyCompare(p0, interPt))
        {
            return false;
        }
        if (!includeP1 && fuzzyCompare(p1, interPt))
        {
            return false;
        }
    }
    return cross;
}

bool MathUtils::isLineCrossFace(const QVector3D &p0, const QVector3D &p1, const QVector3D &v0, const QVector3D &v1, const QVector3D &v2, bool includeP0, bool includeP1)
{
    //Point3 interPt = new Point3();
    QVector3D interPt;
    bool cross = isLineCrossFace(p0, p1, v0, v1, v2, interPt, includeP0, includeP1);
    
    if (cross)
    {
        if (!includeP0 && fuzzyCompare(p0, interPt))
        {
            return false;
        }
        if (!includeP1 && fuzzyCompare(p1, interPt))
        {
            return false;
        }
    }
    return cross;
}

QVector3D MathUtils::goThoughVector(const QVector3D &point, const QVector3D &normal, float length)
{
    
    auto normalSize = normal.length();
    QVector3D offset;
    offset.setX((normal.x() / normalSize) * length);
    offset.setY((normal.y() / normalSize) * length);
    offset.setZ((normal.z() / normalSize) * length);
    return point + offset;
}


bool MathUtils::pointInTriangle(const AkPoint3i &a, const AkPoint3i &b, const AkPoint3i &c, const AkPoint3i &p)
{
    auto v0 = c - a;
    auto v1 = b - a;
    auto v2 = p - a;

    auto dot00 = v0.dotProduct(v0);
    auto dot01 = v0.dotProduct(v1);
    auto dot02 = v0.dotProduct(v2);
    auto dot11 = v1.dotProduct(v1);
    auto dot12 = v1.dotProduct(v2);
    auto value = (dot00 * dot11 - dot01 * dot01);
    if (value == 0)
    {
        return false;
    }

    double inverDeno = 1 / (double)value;

    double u = (dot11 * dot02 - dot01 * dot12) * inverDeno;
    if (u < 0 || u > 1) // if u out of range, return directly
    {
        return false;
    }

    double v = (dot00 * dot12 - dot01 * dot02) * inverDeno;
    if (v < 0 || v > 1) // if v out of range, return directly
    {
        return false;
    }
    return u + v <= 1;
}

bool MathUtils::isLine(const AkPoint3i &p0, const AkPoint3i &p1, const AkPoint3i &p2)
{
    AkInt NormalizeLength = 10000000L;
    auto vector1 = p1 - p0;
    auto vector2 = p1 - p2;
    auto diff0 = vector1.normalized(NormalizeLength);
    auto diff1 = vector2.normalized(NormalizeLength);
    
    auto d = diff0.dotProduct(diff1);
    return (d < -(NormalizeLength * NormalizeLength - 10000)) ;
}

bool MathUtils::isPointOnFace(const AkPoint3i &p0, const AkPoint3i &v0, const AkPoint3i &v1, const AkPoint3i &v2, const AkPoint3i &Normal, float D)
{
    AkInt dP0 = qAbs(Normal.x() * p0.x() + Normal.y() * p0.y() + Normal.z() * p0.z() + D);
    AkInt permitDiff = 200;
    
    if (dP0 < permitDiff)
    {
        
        if (dP0 < permitDiff)
        {
            bool p0InTriangle = pointInTriangle(v0, v1, v2, p0);
            if (p0InTriangle)
            {
                return true;
            }
        }
    }
    
    return false;
}

bool MathUtils::isLineCrossFace(const AkPoint3i &p0, const AkPoint3i &p1, const AkPoint3i &v0, const AkPoint3i &v1, const AkPoint3i &v2, const AkPoint3i &Normal, float D, AkPoint3i &interPt, bool includeP0, bool includeP1)
{
    
    auto dP0 = qAbs(Normal.x() * p0.x() + Normal.y() * p0.y() + Normal.z() * p0.z() + D);
    auto dP1 = qAbs(Normal.x() * p1.x() + Normal.y() * p1.y() + Normal.z() * p1.z() + D);
    auto permitDiff = 10;
    
    if (dP0 < permitDiff || dP1 < permitDiff)
    {
        
        if (dP0 < permitDiff && includeP0)
        {
            bool p0InTriangle = pointInTriangle(v0, v1, v2, p0);
            if (p0InTriangle)
            {
                interPt = p0;
                return true;
            }
        }
        if (dP1 < permitDiff && includeP1)
        {
            bool p1InTriangle = pointInTriangle(v0, v1, v2, p1);
            if (p1InTriangle)
            {
                interPt = p1;
                return true;
            }
        }
        
        return false;
    }

    
    auto V = (p1 - p0).normalized(scale);
    //V.Normalize();

    
    double s = Normal.dotProduct(V);

    
    if (std::abs(s) < FLOAT_ZERO)
    {
        return false;
    }

    //double q = -D - Normal.Dot(p0);
    double t = (-D - Normal.dotProduct(p0)) / s;

    //Point3 interPt = new Point3();
    
    interPt.setY((AkInt)(p0.y() + t * V.y()));
    interPt.setZ((AkInt)(p0.z() + t * V.z()));
    interPt.setX((AkInt)(p0.x() + t * V.x()));

    
    //bool ptInTriangle = pointInTriangle(v0, v1, v2, interPt);
    auto v0V3d = v0.toVector3D();
    QMatrix4x4 rotMatrix;
    rotMatrix.translate(v0V3d);
    rotMatrix.rotate(QQuaternion::rotationTo(Normal.toVector3D(), QVector3D(0, 0, 1)));
    rotMatrix.translate(v0V3d);
    auto fv0 = rotMatrix * v0.toVector3D();
    auto fv1 = rotMatrix * v1.toVector3D();
    auto fv2 = rotMatrix * v2.toVector3D();
    auto fInterP = rotMatrix * interPt.toVector3D();
    fv0.setZ(0);
    fv1.setZ(0);
    fv2.setZ(0);
    fInterP.setZ(0);
    bool ptInTriangle = pointInTriangle(fv0, fv1, fv2, fInterP);

    
    AkInt dot = (p1 - interPt).dotProduct(p0 - interPt);
    
    if (dot <= 0 && ptInTriangle)
    {
        return true;
    }

    return false;

}

bool MathUtils::isLineCrossFace(const AkPoint3i &p0, const AkPoint3i &p1, const AkPoint3i &v0, const AkPoint3i &v1, const AkPoint3i &v2, AkPoint3i &interPt, bool includeP0, bool includeP1)
{
    
    //auto Normal = (v2 - v0).crossProduct(v1 - v0).normalized(scale);
    auto Normal = (v1 - v0).crossProduct(v2 - v0).normalized(scale);
    //Normal.Normalize();
    AkInt D = -(Normal.x() * v0.x() + Normal.y() * v0.y() + Normal.z() * v0.z());
    bool cross = isLineCrossFace(p0,p1,v0,v1,v2, Normal,D,interPt, includeP0, includeP1);
    if (cross)
    {
        if (!includeP0 && p0.x() == interPt.x() && p0.y() == interPt.y() && p0.z() == interPt.z())
        {
            return false;
        }
        if (!includeP1 && p1.x() == interPt.x() && p1.y() == interPt.y() && p1.z() == interPt.z())
        {
            return false;
        }
    }
    return cross;

}

bool MathUtils::isLineCrossFace(const AkPoint3i &p0, const AkPoint3i &p1, const AkPoint3i &v0, const AkPoint3i &v1, const AkPoint3i &v2, bool includeP0, bool includeP1)
{
    AkPoint3i interPt;
    bool cross = isLineCrossFace(p0,p1,v0,v1,v2, interPt, includeP0,includeP1);
    
    if (cross)
    {
        if (!includeP0 && p0.x() == interPt.x() && p0.y() == interPt.y() && p0.z() == interPt.z())
        {
            return false;
        }
        if (!includeP1 && p1.x() == interPt.x() && p1.y() == interPt.y() && p1.z() == interPt.z())
        {
            return false;
        }
    }
    return cross;
}

AkPoint3i MathUtils::goThoughVector(const AkPoint3i &point, const AkPoint3i &normal, AkInt length)
{
    
    AkInt normalSize = normal.length();
    AkPoint3i offset;
    offset.setX((AkInt)((normal.x() / (double)normalSize) * length));
    offset.setY((AkInt)((normal.y() / (double)normalSize) * length));
    offset.setZ((AkInt)((normal.z() / (double)normalSize) * length));
    return point + offset;
}

AkInt MathUtils::area(const AkPoint3i& v0, const AkPoint3i& v1, const AkPoint3i& v2)
{
    auto p1 = v1 - v0;
    auto p2 = v2 - v0;
    auto normal = p1.crossProduct(p2);
    return 0.5 * normal.length();
}


AK_UTIL_NAME_SPACE_END

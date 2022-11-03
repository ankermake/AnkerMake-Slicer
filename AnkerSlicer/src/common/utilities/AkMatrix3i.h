#ifndef AKMATRIX3I_H
#define AKMATRIX3I_H
#include "AkPoint3i.h"
#include <QMatrix4x4>

class AkMatrix3i
{
public:
    AkMatrix3i()
    {
        m[0][0] = 1;
        m[1][0] = 0;
        m[2][0] = 0;
        m[0][1] = 0;
        m[1][1] = 1;
        m[2][1] = 0;
        m[0][2] = 0;
        m[1][2] = 0;
        m[2][2] = 1;
    }
    AkMatrix3i(AkInt matrixValues[])
    {
        m[0][0] = matrixValues[0];
        m[1][0] = matrixValues[1];
        m[2][0] = matrixValues[2];
        m[0][1] = matrixValues[3];
        m[1][1] = matrixValues[4];
        m[2][1] = matrixValues[5];
        m[0][2] = matrixValues[6];
        m[1][2] = matrixValues[7];
        m[2][2] = matrixValues[8];
    }
    inline AkPoint3i transform(const AkPoint3i& p)
    {
        return AkPoint3i(
            (p.x() * m[0][0] + p.y() * m[1][0] + p.z() * m[2][0]),
            (p.x() * m[0][1] + p.y() * m[1][1] + p.z() * m[2][1]),
            (p.x() * m[0][2] + p.y() * m[1][2] + p.z() * m[2][2]));
    }
    inline AkPoint3i Revert(const AkPoint3i& p)
    {
        return AkPoint3i(
            (p.x() * m[0][0] + p.y() * m[0][1] + p.z() * m[0][2]),
            (p.x() * m[1][0] + p.y() * m[1][1] + p.z() * m[1][2]),
            (p.x() * m[2][0] + p.y() * m[2][1] + p.z() * m[2][2]));
    }

private:
    AkInt m[3][3];
};



#endif // AKMATRIX3I_H

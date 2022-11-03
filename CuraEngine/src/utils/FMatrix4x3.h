//Copyright (c) 2020 Ultimaker B.V.
//CuraEngine is released under the terms of the AGPLv3 or higher.

#ifndef FMATRIX4X3_H
#define FMATRIX4X3_H

namespace cura
{

class FPoint3;
class Point3;
class Ratio;

/*!
 * A 4x3 affine transformation matrix.
 *
 * This matrix behaves as if it's a 4x4 transformation matrix, but the bottom
 * row is always identity.
 */
class FMatrix4x3
{
public:
    /*!
     * Create a scaling matrix with a uniform scale.
     * \param scale The scale factor that this matrix should apply.
     * \param origin The coordinate origin to apply the scale from. If the scale
     * is reduced, all coordinates will go towards this origin. If the scale is
     * increased, all coordinates will go away from this origin.
     */
    static FMatrix4x3 scale(const Ratio scale, const Point3 origin);

    /*!
     * The matrix data, row-endian.
     * 
     * The first index is the column. The second index is the row.
     */
    double m[4][3];

    /*!
     * Construct an identity matrix.
     */
    FMatrix4x3();

    /*!
     * Apply this transformation to a coordinate.
     *
     * The result will also be converted to an integer-based coordinate
     * (``Point3``).
     * \param p The coordinate to transform.
     * \return A transformed coordinate.
     */
    Point3 apply(const FPoint3& p) const;

    /*!
     * Apply this transformation to a coordinate.
     * \param p The coordinate to transform.
     * \return A transformed coordinate.
     */
    Point3 apply(const Point3& p) const;

    //  add  @2022-03-02 by CL
    inline bool operator==(const FMatrix4x3 & r) const{
        return m[0][0] == r.m[0][0] &&
               m[1][0] == r.m[1][0] &&
               m[2][0] == r.m[2][0] &&
               m[3][0] == r.m[3][0] &&
               m[0][1] == r.m[0][1] &&
               m[1][1] == r.m[1][1] &&
               m[2][1] == r.m[2][1] &&
               m[3][1] == r.m[3][1] &&
               m[0][2] == r.m[0][2] &&
               m[1][2] == r.m[1][2] &&
               m[2][2] == r.m[2][2] &&
               m[3][2] == r.m[3][2] ;
    }
};

} //namespace cura
#endif //FMATRIX4X3_H

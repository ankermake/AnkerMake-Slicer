/****************************************************************************
Revision 0.1  2022/02/08 15:12:36  Aden Hu
****************************************************************************/


#ifndef SCENE_PARAM_H
#define SCENE_PARAM_H

#include "../ml_document/cmesh.h"
#include <QObject>
#include <QVariant>
#include <QVector3D>
#include <QColor>
#include <QRect>
#include <QPixmap>

#define SCENE_PARAM_NAME "SceneParam"
#define EPSINON 0.000001

struct PrintMachineBoxParam
{
	
	float m_length;
	float m_width;
	float m_height;

	
	QColor m_color;

	
	float m_lineWidth;

	
	int num;

	inline void clear()
	{
		m_length = 0.0;
		m_width = 0.0;
		m_height = 0.0;

		m_color = QColor(255, 255, 255, 255);
		m_lineWidth = 1.0f;
		num = 30;
	}

	inline PrintMachineBoxParam& operator= (const PrintMachineBoxParam& pmbp)
	{
		m_length = pmbp.m_length;
		m_width = pmbp.m_width;
		m_height = pmbp.m_height;

		m_color = pmbp.m_color;
		m_lineWidth = pmbp.m_lineWidth;

		num = pmbp.num;
		return *this;
	}

	inline bool operator==(const PrintMachineBoxParam& pmbp)
	{
		return (fabsf(m_length - pmbp.m_length) < EPSINON) &&
			(fabsf(m_width - pmbp.m_width) < EPSINON) &&
			(fabsf(m_height - pmbp.m_height) < EPSINON) &&
			(m_color == pmbp.m_color) &&
			(fabsf(m_lineWidth - pmbp.m_lineWidth) < EPSINON) &&
			(num == pmbp.num);
	}
};

struct MyTriangle2
{
    MyTriangle2() {}
    MyTriangle2(const int& index1, const int& index2, const int& index3)
    {
        m_index1 = index1;
        m_index2 = index2;
        m_index3 = index3;
    }
    int m_index1;
    int m_index2;
    int m_index3;

    inline MyTriangle2& operator= (const MyTriangle2& triangle)
    {
        m_index1 = triangle.m_index1;
        m_index2 = triangle.m_index2;
        m_index3 = triangle.m_index3;
        return *this;
    }
};

inline QDataStream& operator <<(QDataStream& out, const MyTriangle2& item)
{
    out << item.m_index1 << item.m_index2 << item.m_index3;
    return out;
}

inline QDataStream& operator >>(QDataStream& in, MyTriangle2& item)
{
    in >> item.m_index1 >> item.m_index2 >> item.m_index3;
    return in;
}
struct MyMesh
{
    QVector<QVector3D> points;
    QVector<QVector3D> nors;
    QVector<MyTriangle2> faces;
    QMatrix4x4 trans;

    inline MyMesh& operator= (const MyMesh& mesh)
    {
        points = mesh.points;
        nors = mesh.nors;
        faces = mesh.faces;
        trans = mesh.trans;
        return *this;
    }
};

struct ViewButtonPixmap
{
	
	QPixmap m_justBtnPixmap;
	QPixmap m_frontBtnPixmap;
	QPixmap m_backBtnPixmap;
	QPixmap m_leftBtnPixmap;
	QPixmap m_rightBtnPixmap;
	QPixmap m_topBtnPixmap;
	QPixmap m_bottomBtnPixmap;



	inline ViewButtonPixmap& operator= (const ViewButtonPixmap& vbp)
	{
		m_justBtnPixmap = vbp.m_justBtnPixmap;
		m_frontBtnPixmap = vbp.m_frontBtnPixmap;
		m_backBtnPixmap = vbp.m_backBtnPixmap;
		m_leftBtnPixmap = vbp.m_leftBtnPixmap;
		m_rightBtnPixmap = vbp.m_rightBtnPixmap;
		m_topBtnPixmap = vbp.m_topBtnPixmap;
		m_bottomBtnPixmap = vbp.m_bottomBtnPixmap;
		return *this;
	}

	inline bool operator==(const ViewButtonPixmap& vbp)
	{
		return true;
	}
};

struct SceneParam
{
    QVector3D m_eye;
    QVector3D m_up;
    QVector3D m_front;

    CMeshO logoMesh;  //logo CMeshO
    MyMesh logo;

	PrintMachineBoxParam m_printMachineBox;	 

	QColor m_backgroundColor;

	//ViewButtonPixmap m_viewBtnPixmap;

	inline void clear()
	{
		m_printMachineBox.clear();
		m_eye = QVector3D(0, 0, 0);
		m_up = QVector3D(0, 0, 0);
		m_front = QVector3D(0, 0, 0);

        logoMesh.Clear();
        m_backgroundColor = QColor(0, 0, 0, 255);
	}

	inline SceneParam& operator= (const SceneParam& sp)
	{
		m_eye = sp.m_eye;
		m_up = sp.m_up;
		m_front = sp.m_front;
        logoMesh = sp.logoMesh;
		m_printMachineBox = sp.m_printMachineBox;
		m_backgroundColor = sp.m_backgroundColor;
        logo = sp.logo;
		//m_viewBtnPixmap = sp.m_viewBtnPixmap;

		return *this;
	}

	inline bool operator==(const SceneParam& sp)
	{
		return (m_eye == sp.m_eye) &&
			(m_up == sp.m_up) &&
			(m_front == sp.m_front) &&
            (logoMesh == sp.logoMesh) &&
			(m_printMachineBox == sp.m_printMachineBox) &&
			(m_backgroundColor == sp.m_backgroundColor);

	}


};

inline QDataStream& operator <<(QDataStream& out, const SceneParam& item)
{
    out<<item.m_eye<<item.m_up<<item.m_front<<item.m_printMachineBox.m_color<<
         item.m_printMachineBox.m_height<<item.m_printMachineBox.m_length<<
         item.m_printMachineBox.m_lineWidth<<item.m_printMachineBox.m_width<<
         item.m_printMachineBox.num<<item.m_backgroundColor<< item.logo.points<< item.logo.nors<< item.logo.faces<< item.logo.trans;;
    return out;
}

inline QDataStream& operator >>(QDataStream& in, SceneParam& item)
{
    in>>item.m_eye>>item.m_up>>item.m_front>>item.m_printMachineBox.m_color>>
         item.m_printMachineBox.m_height>>item.m_printMachineBox.m_length>>
            item.m_printMachineBox.m_lineWidth>>item.m_printMachineBox.m_width>>
            item.m_printMachineBox.num>>item.m_backgroundColor>>
            item.logo.points>> item.logo.nors >> item.logo.faces>> item.logo.trans;
    return in;
}
Q_DECLARE_METATYPE(MyMesh)
Q_DECLARE_METATYPE(SceneParam)
struct passSceneParam
{
    QVector3D m_eye;
    QVector3D m_up;
    QVector3D m_front;
    PrintMachineBoxParam m_printMachineBox;
    QColor m_backgroundColor;
    MyMesh logo;
};

inline QDataStream& operator <<(QDataStream& out, const passSceneParam& item)
{
    out<<item.m_eye<<item.m_up<<item.m_front<<item.m_printMachineBox.m_length
      <<item.m_printMachineBox.m_width<<item.m_printMachineBox.m_height<<item.m_printMachineBox.m_color
      <<item.m_printMachineBox.m_lineWidth<<item.m_printMachineBox.num<<item.m_backgroundColor
      << item.logo.points<< item.logo.nors<< item.logo.faces<< item.logo.trans;
//    out<<item.m_eye<<item.m_up<<item.m_front;
//    out<<item.m_backgroundColor;
    return out;
}

inline QDataStream& operator >>(QDataStream& in, passSceneParam& item)
{
    in>>item.m_eye>>item.m_up>>item.m_front>>item.m_printMachineBox.m_length>>
             item.m_printMachineBox.m_width>>item.m_printMachineBox.m_height>>
            item.m_printMachineBox.m_color>>item.m_printMachineBox.m_lineWidth>>
            item.m_printMachineBox.num>>item.m_backgroundColor >>
            item.logo.points>> item.logo.nors >> item.logo.faces>> item.logo.trans;

//    in>>item.m_eye>>item.m_up>>item.m_front;
//    in>>item.m_backgroundColor;
    return in;
}

Q_DECLARE_METATYPE(passSceneParam)



#endif // !SCENE_PARAM_H

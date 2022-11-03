/****************************************************************************
Revision 0.1  2021/12/18 11:57:03  Aden Hu
****************************************************************************/

#ifndef BOXBORDER_H
#define BOXBORDER_H

#include <QColor>
#include <vcg/complex/algorithms/update/bounding.h>
#include <common/ml_document/cmesh.h>
#include <meshlab/glarea.h>
#include <QVector3D>
#include <common/GLExtensionsManager.h>
#define PI 3.1415926535898
#define EPSINON 0.01
class BoxBorder
{
public:
	BoxBorder();
	~BoxBorder();

	void setParam(GLArea* gla, MeshModel& m);
	void checkBorder(MeshModel* model);
	static BoxBorder* getInstancePtr()
	{
		if (m_instance_ptr == nullptr)
		{
			m_instance_ptr = new BoxBorder;
		}
		return m_instance_ptr;
	}
	bool isInBBox();
	bool isInBBox(const MeshModel* meshmodel);
	bool isFirst() const;

	//???????????
	bool isInBox(const Box3m& box, const vcg::Point3f& p);
	void setDefaultColor(const QColor& color);
	void setModelColor(const QColor& color);
	void setModelColor(const vcg::Color4b& color);
	void setDefaultColor();
	vcg::Color4b& getModelColor();
	vcg::Color4b modelColor();
private:
	GLArea* m_gla;
	MeshModel* mm;

	vcg::Color4b defaultColor;
	QColor firstColor;
	MLRenderingData defaultMLRenderData;
	bool m_isIn;

	bool m_isFirst;
	static BoxBorder* m_instance_ptr;
};





#endif // !BOXBORDER_H


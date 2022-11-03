#include "BoxBorder.h"

#include <vcg/simplex/face/component_ocf.h>

BoxBorder* BoxBorder::m_instance_ptr = nullptr;

BoxBorder::BoxBorder() : m_gla(nullptr), mm(nullptr), m_isIn(false), m_isFirst(true)
{

}
BoxBorder::~BoxBorder()
{

}

bool BoxBorder::isFirst() const
{
	return m_isFirst;
}

void BoxBorder::setParam(GLArea* gla, MeshModel& m)
{
	m_gla = gla;
	mm = &m;
	if (m_gla != NULL && mm != NULL)
	{

		if (m_isFirst)
		{
			defaultColor = modelColor();

			m_isFirst = false;
		}
		if (isInBBox())
		{
			//setModelColor(defaultColor);
			setDefaultColor();
		}
		else
		{
			setModelColor(QColor(125, 0, 0, 255));
		}
	}
}

void BoxBorder::checkBorder(MeshModel* model)
{
	if (model == NULL)
	{
		return;
	}
	if (m_isFirst)
	{
		firstColor = model->m_showmesh->getColor();
		m_isFirst = false;
	}
}


void BoxBorder::setDefaultColor(const QColor& color)
{
	defaultColor = vcg::Color4b(color.red(), color.green(), color.blue(), color.alpha());
}

void BoxBorder::setDefaultColor()
{
	MLSceneGLSharedDataContext* datacont = m_gla->mvc()->sharedDataContext();
	if (datacont == NULL)
		return;
	datacont->setRenderingDataPerMeshView(mm->id(), m_gla->context(), defaultMLRenderData);
}

void BoxBorder::setModelColor(const vcg::Color4b& color)
{
	MLSceneGLSharedDataContext* datacont = m_gla->mvc()->sharedDataContext();
	if (datacont == NULL)
		return;
	MLRenderingData curr;
	datacont->getRenderInfoPerMeshView(mm->id(), m_gla->context(), curr);
	MLPerViewGLOptions opts;
	if (curr.get(opts) == false)
		throw MLException(QString("GLArea: invalid MLPerViewGLOptions"));
	opts._persolid_fixed_color_enabled = true;
	opts._persolid_fixed_color = color;
	//opts._persolid_noshading = false;
	curr.set(opts);
	datacont->setRenderingDataPerMeshView(mm->id(), m_gla->context(), curr);
}

void BoxBorder::setModelColor(const QColor& color)
{
	MLSceneGLSharedDataContext* datacont = m_gla->mvc()->sharedDataContext();
	if (datacont == NULL)
		return;
	MLRenderingData curr;
	datacont->getRenderInfoPerMeshView(mm->id(), m_gla->context(), curr);
	MLPerViewGLOptions opts;
	if (curr.get(opts) == false)
		throw MLException(QString("GLArea: invalid MLPerViewGLOptions"));
	opts._persolid_fixed_color_enabled = true;
	opts._persolid_fixed_color = vcg::Color4b(color.red(), color.green(), color.blue(), color.alpha());
	//opts._persolid_noshading = true;
	curr.set(opts);
	datacont->setRenderingDataPerMeshView(mm->id(), m_gla->context(), curr);
}

vcg::Color4b BoxBorder::modelColor()
{
	MLSceneGLSharedDataContext* datacont = m_gla->mvc()->sharedDataContext();
	MLRenderingData curr;
	datacont->getRenderInfoPerMeshView(mm->id(), m_gla->context(), curr);
	defaultMLRenderData = curr;
	MLPerViewGLOptions opts;
	if (curr.get(opts) == false)
		throw MLException(QString("GLArea: invalid MLPerViewGLOptions"));

	return opts._persolid_fixed_color;
}

vcg::Color4b& BoxBorder::getModelColor()
{
	if (mm == NULL)
		return defaultColor;
	for (CMeshO::FaceIterator fi = mm->cm.face.begin(); fi != mm->cm.face.end(); fi++)
	{
		if (!fi->IsColorEnabled())
		{
			fi->Base().EnableColor();
		}
		if (fi->HasColor())
		{
			defaultColor = fi->C();
			fi->Base().DisableColor();
			break;
		}
	}
	setModelColor(defaultColor);
	return defaultColor;
}

bool BoxBorder::isInBox(const Box3m& box, const vcg::Point3f& p)
{
	return ((p.X() - box.min.X() > -EPSINON) &&
		(p.Y() - box.min.Y() > -EPSINON) &&
		(p.Z() - box.min.Z() > -EPSINON) &&
		(box.max.X() - p.X() > -EPSINON) &&
		(box.max.Y() - p.Y() > -EPSINON) &&
		(box.max.Z() - p.Z() > -EPSINON));
}

bool BoxBorder::isInBBox()
{
	if (m_gla == NULL || mm == NULL)
		return true;

	Box3m modelBox = mm->cm.calCurrentAABB();
	Box3m sceneBox = m_gla->sceneBox;
	m_isIn = true;

	for (int i = 0; i < 8; i++)
	{
		if (!isInBox(sceneBox, modelBox.P(i)))
		{
			m_isIn = false;
			break;
		}
	}
	return m_isIn;
}

bool BoxBorder::isInBBox(const MeshModel* meshmodel)
{
	if (meshmodel == NULL)
		return false;
	//Box3m modelBox = meshmodel->cm.calCurrentAABB();
	//meshmodel->m_bePrintObj.
}



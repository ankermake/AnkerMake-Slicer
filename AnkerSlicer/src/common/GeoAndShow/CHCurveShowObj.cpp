#include "CHCurveShowObj.h"
#include "CHScene.h"



CHCurveShowObj::CHCurveShowObj()
{
    m_lineWide = 2.0;
	m_priority = Curve;
    m_calLight = false;
}

CHCurveShowObj::~CHCurveShowObj()
{
}

bool CHCurveShowObj::pick(int pixelX, int pixelY, PickResult& result, int pickTol)
{
	return false;
}

void CHCurveShowObj::updateToScene()
{
	if (curScene)
	{
		curScene->updateShowObj(shared_from_this());
	}
}

std::vector<QVector3D>& CHCurveShowObj::getShowPoints()
{
	return m_showPoints;
}

void CHCurveShowObj::setLineWide(float value)
{
	m_lineWide = value;
}

float CHCurveShowObj::getLineWide()
{
	return m_lineWide;
}












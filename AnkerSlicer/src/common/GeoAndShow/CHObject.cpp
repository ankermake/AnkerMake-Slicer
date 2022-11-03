#include "CHObject.h"
#include <QDateTime>

CHObject::CHObject()
{
	static qint64 id = QDateTime::currentMSecsSinceEpoch() / 1000;
    
	id++;
	m_id = id;
	m_name = QString::number(m_id);
}

CHObject::~CHObject()
{
	
}

qint64 CHObject::getId()
{
	return m_id;
}

void CHObject::setId(qint64 id)
{
	m_id = id;
}

void CHObject::setObjectName(const QString& name)
{
	m_name = name;
}

QString CHObject::getObjectName()
{
	return m_name;
}















#include "transform.h"

const QVector3D Transform::LocalForward(0.0f, 0.0f, 1.0f);
const QVector3D Transform::LocalUp(0.0f, 1.0f, 0.0f);
const QVector3D Transform::LocalRight(1.0f, 0.0f, 0.0f);

void Transform::translate(const QVector3D& dt)
{
	m_dirty = true;
	m_translation += dt;
}

void Transform::scale(const QVector3D& ds)
{
	m_dirty = true;
	m_scale *= ds;
}

void Transform::rotate(const QQuaternion& dr)
{
	m_dirty = true;
	m_rotation = dr * m_rotation;
}

void Transform::grow(const QVector3D& ds)
{
	m_dirty = true;
	m_scale += ds;
}

void Transform::setTranslation(const QVector3D& t)
{
	m_dirty = true;
	m_translation = t;
}

void Transform::setScale(const QVector3D& s)
{
	m_dirty = true;
	m_scale = s;
}

void Transform::setRotation(const QQuaternion& r)
{
	m_dirty = true;
	m_rotation = r;
}

const QMatrix4x4& Transform::toMatrix()
{
	if (m_dirty)
	{
		m_dirty = false;
		m_world.setToIdentity();
		m_world.translate(m_translation);
		m_world.rotate(m_rotation);
		m_world.scale(m_scale);
	}
	return m_world;
}

QVector3D Transform::forward() const
{
	return m_rotation.rotatedVector(LocalForward);
}

QVector3D Transform::up() const
{
	return m_rotation.rotatedVector(LocalUp);
}

QVector3D Transform::right() const
{
	return m_rotation.rotatedVector(LocalRight);
}

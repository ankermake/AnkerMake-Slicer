#include "camera.h"

const QVector3D Camera::LocalForward(0.0f, 0.0f, -1.0f);
const QVector3D Camera::LocalUp(0.0f, 1.0f, 0.0f);
const QVector3D Camera::LocalRight(1.0f, 0.0f, 0.0f);

void Camera::translate(const QVector3D& dt)
{
	m_dirty = true;
	m_translation += dt;
}

void Camera::rotate(const QQuaternion& dr)
{
	m_dirty = true;
	m_rotation = dr * m_rotation;
}

void Camera::setTranslation(const QVector3D& t)
{
	m_dirty = true;
	m_translation = t;
}

void Camera::setRotation(const QQuaternion& r)
{
	m_dirty = true;
	m_rotation = r;
}


const QMatrix4x4& Camera::toMatrix()
{
	if (m_dirty)
	{
		m_dirty = false;
		m_world.setToIdentity();
		m_world.rotate(m_rotation.conjugated());
		m_world.translate(-m_translation);
	}
	return m_world;
}


QVector3D Camera::forward() const
{
	return m_rotation.rotatedVector(LocalForward);
}

QVector3D Camera::up() const
{
	return m_rotation.rotatedVector(LocalUp);
}

QVector3D Camera::right() const
{
	return m_rotation.rotatedVector(LocalRight);
}

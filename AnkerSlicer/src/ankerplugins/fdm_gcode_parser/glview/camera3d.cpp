#include "camera3d.h"
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 
Camera3D::Camera3D()
{
	reset();
}

Camera3D::~Camera3D()
{

}

const glm::vec3& Camera3D::getCenter()
{
	return mCenter;
}

const glm::vec3& Camera3D::getEye()
{
	return mEye;
}

const glm::mat4& Camera3D::getMatrix()
{
	return mMatrix;
}

const float* Camera3D::getMatrixFlat()
{
	return glm::value_ptr(mMatrix);
}

const glm::vec3& Camera3D::getUp()
{
	return mUp;
}

void Camera3D::setMatrix(const float *matrix)
{
	mMatrix = glm::make_mat4(matrix);
}

void Camera3D::reset()
{
	/*mEye.x = 0.f;
	mEye.y = 0.f;
	mEye.z = 1.f;
	mCenter.x = 0.f;
	mCenter.y = 0.f;
	mCenter.z = 0.f;
	mUp.x = 0.f;
	mUp.y = 1.f;
	mUp.z = 0.f;   */

    mEye.x = 90.f;
    mEye.y = -90.f;
    mEye.z = 170.f;
    mCenter.x = 100.f;
    mCenter.y = 100.f;
    mCenter.z = 0.f;
	mUp.x = 0.f;
    mUp.y = 1.f;
	mUp.z = 0.f;
	update();
}

void Camera3D::setEye(float x, float y, float z)
{
	mEye.x = x;
	mEye.y = y;
	mEye.z = z;
}

void Camera3D::setEye(const glm::vec3& e)
{
	mEye = e;
}

void Camera3D::setCenter(float x, float y, float z)
{
	mCenter.x = x;
	mCenter.y = y;
	mCenter.z = z;
}

void Camera3D::setCenter(const glm::vec3& c)
{
	mCenter = c;
}

void Camera3D::setUp(float x, float y, float z)
{
	mUp.x = x;
	mUp.y = y;
	mUp.z = z;
}

void Camera3D::setUp(const glm::vec3& u)
{
	mUp = u;
}

void Camera3D::update()
{
	mMatrix = glm::lookAt(mEye, mCenter, mUp);
}



/****************************************************************************
Revision 0.1  2021/12/10 11:47:40  Aden Hu
****************************************************************************/
#ifndef CAMERA3D_H
#define CAMERA3D_H
#include <include/glm/glm.hpp>

class Camera3D
{
public:
	Camera3D();
	~Camera3D();

	const glm::mat4& getMatrix();
	const float* getMatrixFlat();
	const glm::vec3& getCenter();
	const glm::vec3& getEye();
	const glm::vec3& getUp();
	void reset();
	void setMatrix(const float *matrix);
	void setCenter(float x, float y, float z);
	void setCenter(const glm::vec3& c);
	void setEye(float x, float y, float z);
	void setEye(const glm::vec3& e);
	void setUp(float x, float y, float z);
	void setUp(const glm::vec3& u);
	void update();

private:
	glm::vec3 mCenter;
	glm::vec3 mEye;
	glm::mat4 mMatrix;
	glm::vec3 mUp;
};



#endif // !CAMERA3D_H

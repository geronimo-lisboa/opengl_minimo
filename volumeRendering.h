//#pragma once
//#include "rendering3d.h"
//#include <eigen3\Eigen\Geometry>
//using namespace std;
//using namespace Eigen;
//class CubeExample
//{
//private:
//	shared_ptr<Shader> myShader;
//	vector<GLfloat> vertexes;
//	GLuint vao, vertexesVbo;
//	//Por enquanto a infraestrutura de câmera ficará aqui. Depois movo para um lugar mais conveniente
//	Matrix4f mViewMatrix, mProjectionMatrix, mModelMatrix;
//	void lookAt(const Eigen::Vector3f& position, const Eigen::Vector3f& target, const Eigen::Vector3f& up)
//	{
//		Matrix3f R;
//		R.col(2) = (position - target).normalized();
//		R.col(0) = up.cross(R.col(2)).normalized();
//		R.col(1) = R.col(2).cross(R.col(0));
//		mViewMatrix.topLeftCorner<3, 3>() = R.transpose();
//		mViewMatrix.topRightCorner<3, 1>() = -R.transpose() * position;
//		mViewMatrix(3, 3) = 1.0f;
//	}
//	void setPerspective(float fovY, float aspect, float _near, float _far)
//	{
//		float theta = fovY*0.5;
//		float range = _far - _near;
//		float invtan = 1. / tan(theta);
//		mProjectionMatrix(0, 0) = invtan / aspect;
//		mProjectionMatrix(1, 1) = invtan;
//		mProjectionMatrix(2, 2) = -(_near + _far) / range;
//		mProjectionMatrix(3, 2) = -1;
//		mProjectionMatrix(2, 3) = -2 * _near * _far / range;
//		mProjectionMatrix(3, 3) = 0;
//	}
//	//Fim da infraestrutura de câmera.
//
//	template <typename T>const GLuint CreateBuffer(GLenum target, std::vector<T> &vec)
//	{
//		GLuint resultBuffer = 0;
//		glGenBuffers(1, &resultBuffer);
//		glBindBuffer(target, resultBuffer);
//		glBufferData(target, vec.size() * sizeof(vec.at(0)), vec.data(), GL_STATIC_DRAW);
//		return resultBuffer;
//	}
//public:
//	CubeExample();
//	~CubeExample();
//	void Render();
//	void RotateAround(int axisId, float degs);
//};
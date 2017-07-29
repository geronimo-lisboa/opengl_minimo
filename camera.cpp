#include "camera.h"
namespace my3d
{
	Camera::Camera(array<float, 3> eye, array<float, 3> focus, array<float, 3> vUp, float fovY, float aspect, float zNear, float zFar)
	{
		mProjectionMatrix << 1, 0, 0, 0,
						 0, 1, 0, 0,
						 0, 0, 1, 0,
						 0, 0, 0, 1;
		mViewMatrix << 1, 0, 0, 0,
				   0, 1, 0, 0,
				   0, 0, 1, 0,
				   0, 0, 0, 1;
		this->SetPerspective(fovY, aspect, zNear, zFar);
		this->LookAt(eye, focus, vUp);
	}
	void Camera::LookAt(array<float, 3> eye, array<float, 3> focus, array<float, 3> vUp)
	{
		this->eye << eye[0], eye[1], eye[2];
		this->focus << focus[0], focus[1], focus[2];
		this->vUp << vUp[0], vUp[1], vUp[2];
		
		Matrix3f R;
		R.col(2) = (this->eye - this->focus).normalized();
		R.col(0) = this->vUp.cross(R.col(2)).normalized();
		R.col(1) = R.col(2).cross(R.col(0));
		mViewMatrix.topLeftCorner<3, 3>() = R.transpose();
		mViewMatrix.topRightCorner<3, 1>() = -R.transpose() * this->eye;
		mViewMatrix(3, 3) = 1.0f;
	}

	void Camera::SetPerspective(float fovY, float aspect, float _near, float _far)
	{
		float theta = fovY*0.5;
		float range = _far - _near;
		float invtan = 1. / tan(theta);
		mProjectionMatrix(0, 0) = invtan / aspect;
		mProjectionMatrix(1, 1) = invtan;
		mProjectionMatrix(2, 2) = -(_near + _far) / range;
		mProjectionMatrix(3, 2) = -1;
		mProjectionMatrix(2, 3) = -2 * _near * _far / range;
		mProjectionMatrix(3, 3) = 0;
	}
	Matrix<float, 4, 4> Camera::GetCameraMatrix()
	{
		return mProjectionMatrix * mViewMatrix;
	}
}



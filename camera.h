#pragma once
#include<array>
#include <eigen3\Eigen\Geometry>

using namespace std;
using namespace Eigen;

namespace my3d
{
	class Camera
	{
	private:
		Matrix<float, 4, 4, Eigen::DontAlign> mViewMatrix, mProjectionMatrix;
		Matrix<float, 3, 1, Eigen::DontAlign> eye, focus, vUp;
		void SetPerspective(float fovY, float aspect, float _near, float _far);
	public:
		int screenWidth, screenHeight;
		Camera(array<float, 3> eye, array<float, 3> focus, array<float, 3> vUp, float fovY, float aspect, float zNear, float zFar);
		void LookAt(array<float, 3> eye, array<float, 3> focus, array<float, 3> vUp);
		Matrix<float, 4, 4> GetCameraMatrix();
	};
}
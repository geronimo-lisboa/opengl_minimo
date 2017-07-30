#pragma once
#include <eigen3\Eigen\Geometry>
using namespace Eigen;
namespace my3d
{
	class Transform
	{
	protected:
		Matrix<float, 4, 4, Eigen::DontAlign> mModelMatrix;
	public:
		Transform()
		{
			mModelMatrix << 1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1;
		}
		void RotateAround(int axisId, float angle)
		{
			Vector3f axis;
			if (axisId == 0)axis << 1, 0, 0;
			if (axisId == 1)axis << 0, 1, 0;
			if (axisId == 2)axis << 0, 0, 1;

			Matrix3f rotation = AngleAxisf(angle *3.141592653589793 / 180.0, axis).toRotationMatrix();
			mModelMatrix(0, 0) = rotation(0, 0);
			mModelMatrix(1, 0) = rotation(1, 0);
			mModelMatrix(2, 0) = rotation(2, 0);
			mModelMatrix(0, 1) = rotation(0, 1);
			mModelMatrix(1, 1) = rotation(1, 1);
			mModelMatrix(2, 1) = rotation(2, 1);
			mModelMatrix(0, 2) = rotation(0, 2);
			mModelMatrix(1, 2) = rotation(1, 2);
			mModelMatrix(2, 2) = rotation(2, 2);
		}
	};
}
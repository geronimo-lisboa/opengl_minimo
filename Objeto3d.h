#pragma once
#include <gl/glew.h>
#include "transform.h"
namespace my3d
{
	class Object3d : public Transform
	{
	private:
		GLuint vao, vertexesVbo, colorsVbo, tcVbo, normalVbo;
	};
}
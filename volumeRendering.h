#pragma once
#include "rendering3d.h"
using namespace std;
class MyVolumeRenderer
{
private:
	shared_ptr<Shader> myShader;
	vector<GLfloat> vertexes;
	GLuint vao, vertexesVbo;

	template <typename T>const GLuint CreateBuffer(GLenum target, std::vector<T> &vec)
	{
		GLuint resultBuffer = 0;
		glGenBuffers(1, &resultBuffer);
		glBindBuffer(target, resultBuffer);
		glBufferData(target, vec.size() * sizeof(vec.at(0)), vec.data(), GL_STATIC_DRAW);
		return resultBuffer;
	}
public:
	MyVolumeRenderer();
	~MyVolumeRenderer();
	void Render();
};
#pragma once
#include <gl/glew.h>
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <itkImage.h>
#include "shader.h"
using namespace std;
using namespace my3d;
class Object3d
{
protected:
	GLuint texture;
	GLuint vao;
	GLuint vertexesVbo, colorsVbo, texVbo;
	Shader shader;
	vector<GLfloat> vertexes;
	vector<GLfloat> colors;
	vector<GLfloat> texCoords;
	template <typename T> const GLuint CreateBuffer(GLenum target, std::vector<T> &vec);
public:
	Object3d(std::string vsfile, std::string fsfile) :shader(vsfile, fsfile)
	{
		texture = 0;
		vao = 0;
		vertexesVbo = 0;
		colorsVbo = 0;
		texVbo = 0;
		glDisable(GL_CULL_FACE);
	}
	~Object3d()
	{
		if (vertexesVbo) glDeleteBuffers(1, &vertexesVbo);
		if (colorsVbo) glDeleteBuffers(1, &colorsVbo);
		if (texVbo) glDeleteBuffers(1, &texVbo);
		if (vao) glDeleteVertexArrays(1, &vao);
		if (texture) glDeleteTextures(1, &texture);
	}
	virtual void Render() = 0;
};

class Object3dTexture2d:public Object3d
{
private:
	itk::Image<float, 2>::Pointer image;
public:
	Object3dTexture2d(std::string vsfile, std::string fsfile, itk::Image<float, 2>::Pointer imagem);
	void Render();
};

class Object3dTexture3d :public Object3d
{
private:
	itk::Image<float, 3>::Pointer image;
public:
	Object3dTexture3d(std::string vsfile, std::string fsfile, itk::Image<float, 3>::Pointer imagem);
	void Render();
	void Avancar();
	void Recuar();
};
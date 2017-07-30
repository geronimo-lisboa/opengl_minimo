#pragma once
#include "rendering3d.h"
#include <eigen3\Eigen\Geometry>
#include "camera.h"
#include "transform.h"
#include "itkTypes.h"
using namespace std;
using namespace Eigen;
class CubeExample:public my3d::Transform
{
private:
	bool usandoTextura, usandoIluminacao;
	shared_ptr<Shader> myShader;
	vector<GLfloat> vertexes, colors, texCoords, normalCoords;//Onde os buffers ficaram guardados na memória antes de ir pra GPU
	GLuint vao, vertexesVbo, colorsVbo, tcVbo, normalVbo; //A vertex array object e seus vertexes buffers
	GLuint textura; //A textura do objeto. Por enquanto a sessão da textura está toda hardcoded


	template <typename T>const GLuint CreateBuffer(GLenum target, std::vector<T> &vec)
	{
		GLuint resultBuffer = 0;
		glGenBuffers(1, &resultBuffer);
		glBindBuffer(target, resultBuffer);
		glBufferData(target, vec.size() * sizeof(vec.at(0)), vec.data(), GL_STATIC_DRAW);
		return resultBuffer;
	}
public:
	CubeExample();
	~CubeExample();
	void SetTextura(ImageType::Pointer tex);
	void Render(shared_ptr<Camera> camera);
};
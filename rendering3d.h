#pragma once
#include <gl/glew.h>
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <itkImage.h>
using namespace std;
class Shader
{
private:
	GLuint programId, vsId, fsId;
	map<string, GLuint> attributes;
	map<string, GLuint> uniforms;

	std::string ReadShaderFile(std::string path);
	//Extrai a lista de atributos e uniformes do programa dado. Os pares nome/localiza��o no shader s�o retornados
	//nos mapas passados como par�metro.
	void introspectProgram(GLuint programId, map<string, GLuint> &attributes, map<string, GLuint> &uniforms);
	//Gera o log do shader. A responsabilidade de deletar a string alocada � do invocador da fun��o.
	string GetShaderInfoLog(GLuint object, PFNGLGETSHADERIVPROC glGet__iv, PFNGLGETSHADERINFOLOGPROC glGet__InfoLog);
	//Cria um shader do tipo dado a partir do c�digo fonte dado.
	GLuint MakeShader(GLenum type, std::string source);
	//Cria o programa de shader a partir dos shaders dados.
	GLuint MakeProgram(GLuint vertex_shader, GLuint fragment_shader);
public:
	Shader(string vsPath, string fsPath);
	~Shader();
	GLuint GetUniform(string name);
	GLuint GetAttribute(string name);
	GLuint GetProgramId();
	void UseProgram();
};

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
};
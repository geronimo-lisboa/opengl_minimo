#include "volumeRendering.h"
#include <sstream>
#include "rendering3d.h"

using namespace std;

stringstream defineVertexShader()
{
	stringstream ss;
	ss << "#version 400" << endl;
	ss << "layout(location = 0) in vec3 vertex;" << endl;
	ss << "uniform mat4 mvp;" << endl;
	ss << "void main(){" << endl;
	ss << "  gl_Position = mvp * vec4(vertex, 1.0);" << endl;
	ss << "}" << endl;
	return ss;
}
stringstream defineFragmentShader()
{
	stringstream ss;
	ss << "#version 400" << endl;
	ss << "out vec4 fragColor;" << endl;
	ss << "void main(){" << endl;
	ss << "  fragColor = vec4(1.0, 0.0, 0.0, 1.0);" << endl;
	ss << "}" << endl;
	return ss;
}
vector<GLfloat> defineVertexes()
{
	vector<GLfloat> v;
	v.push_back(-1.0); v.push_back(-1.0); v.push_back(0.0); //IE
	v.push_back( 1.0); v.push_back(-1.0); v.push_back(0.0); //ID
	v.push_back(-1.0); v.push_back( 1.0); v.push_back(0.0); //SE

	v.push_back( 1.0); v.push_back(-1.0); v.push_back(0.0); //ID
	v.push_back( 1.0); v.push_back( 1.0); v.push_back(0.0); //SD
	v.push_back(-1.0); v.push_back( 1.0);  v.push_back(0.0);//SE

	return v;
}

MyVolumeRenderer::MyVolumeRenderer()
{
	//O que tem que ser feito aqui:
	//1)Criar o shader do volume renderer
	myShader = make_shared<Shader>(defineVertexShader(), defineFragmentShader());
	//2)Criar o cubo de suporte
	vertexes = defineVertexes();
	vertexesVbo = CreateBuffer<float>(GL_ARRAY_BUFFER, vertexes);
	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	myShader->UseProgram();
	GLuint vpLocation = myShader->GetAttribute("vertex");
	glEnableVertexAttribArray(vpLocation);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexesVbo);
	glVertexAttribPointer(vpLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	mProjectionMatrix << 1, 0, 0, 0,
						 0, 1, 0, 0,
						 0, 0, 1, 0,
						 0, 0, 0, 1;
	mViewMatrix << 1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1;
}
MyVolumeRenderer::~MyVolumeRenderer()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vertexesVbo);
}
void MyVolumeRenderer::Render()
{
	//Por enquanto a câmera é aqui mesmo...
	Vector3f eye, focus, vup;
	eye << 0.0, 0.0, -3.0;
	focus << 0.0, 0.0, 0.0;
	vup << 0.0, 1.0, 0.0;
	lookAt(eye, focus, vup);
	setPerspective(45, 1, 0.01, 100);
	mModelMatrix << 1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1;
	Matrix4f mvp = mProjectionMatrix * mViewMatrix * mModelMatrix;
	//O que tem que ser feito aqui:
	//1)Exibir o cubo de suporte na tela
	//2)Rotacioná-lo para eu ver se está tudo ok.
	myShader->UseProgram();
	GLuint mvpLocation = myShader->GetUniform("mvp");
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, mvp.data());

	glBindVertexArray(vao);
	GLuint vertexLocation = myShader->GetAttribute("vertex");
	glBindAttribLocation(myShader->GetProgramId(), vertexLocation, "vertex");
	glDrawArrays(GL_TRIANGLES, 0, vertexes.size() / 3);
	std::cout << "aa" << endl;
}

#include "volumeRendering.h"
#include <sstream>
#include "rendering3d.h"

using namespace std;

stringstream defineVertexBuffer()
{
	stringstream ss;
	ss << "#version 400" << endl;
	ss << "layout(location = 0) in vec3 vertex;" << endl;
	ss << "void main(){" << endl;
	ss << "  gl_Position = vec4(vertex, 1.0);" << endl;
	ss << "}" << endl;
	return ss;
}
stringstream defineFragmentBuffer()
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
	myShader = make_shared<Shader>(defineVertexBuffer(), defineFragmentBuffer());
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
}
MyVolumeRenderer::~MyVolumeRenderer()
{
	//O que deve ser feito aqui:
	//Deletar buffers.
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vertexesVbo);
}
void MyVolumeRenderer::Render()
{
	//O que tem que ser feito aqui:
	//1)Exibir o cubo de suporte na tela
	//2)Rotacioná-lo para eu ver se está tudo ok.
	myShader->UseProgram();
	glBindVertexArray(vao);
	GLuint vertexLocation = myShader->GetAttribute("vertex");
	glBindAttribLocation(myShader->GetProgramId(), vertexLocation, "vertex");
	glDrawArrays(GL_TRIANGLES, 0, vertexes.size() / 3);
}

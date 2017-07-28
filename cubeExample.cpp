#include "volumeRendering.h"
#include <sstream>
#include "rendering3d.h"
#include "cubeExample.h"

using namespace std;

stringstream defineVertexShader()
{
	stringstream ss;
	ss << "#version 400" << endl;
	ss << "layout (location = 0) in vec3 vertex;" << endl;
	ss << "layout (location = 1) in vec3 color;" << endl;
	ss << "uniform mat4 modelMat;" << endl;
	ss << "uniform mat4 cameraMat;" << endl;
	ss << "out vec3 vertexColor;" << endl;
	ss << "void main(){" << endl;
	ss << "  gl_Position = cameraMat * modelMat * vec4(vertex, 1.0);" << endl;
	ss << "  vertexColor = color;" << endl;
	ss << "}";
	return ss;
}
//https://www.tomdalling.com/blog/modern-opengl/06-diffuse-point-lighting/
stringstream defineFragmentShader()
{
	stringstream ss;
	ss << "#version 400" << endl;
	ss << "in vec3 vertexColor;" << endl;
	ss << "out vec4 finalColor;" << endl;
	ss << "void main(){" << endl;
	ss << "  finalColor = vec4(vertexColor, 1.0);" << endl;
	ss << "}";
	return ss;
}
vector<GLfloat> defineVertexes()
{
	vector<GLfloat> v;
	v.push_back(-1.0); v.push_back(-1.0); v.push_back(1.0); //IEF
	v.push_back( 1.0); v.push_back(-1.0); v.push_back(1.0); //IDF
	v.push_back(-1.0); v.push_back( 1.0); v.push_back(1.0); //SEF

	v.push_back( 1.0); v.push_back(-1.0); v.push_back(1.0); //IDF
	v.push_back( 1.0); v.push_back( 1.0); v.push_back(1.0); //SDF
	v.push_back(-1.0); v.push_back( 1.0);  v.push_back(1.0);//SEF
	//--------
	v.push_back(-1.0); v.push_back(-1.0); v.push_back(-1.0); //IEB
	v.push_back(1.0); v.push_back(-1.0); v.push_back(-1.0); //IDB
	v.push_back(-1.0); v.push_back(1.0); v.push_back(-1.0); //SEB

	v.push_back(1.0); v.push_back(-1.0); v.push_back(-1.0); //IDB
	v.push_back(1.0); v.push_back(1.0); v.push_back(-1.0); //SDB
	v.push_back(-1.0); v.push_back(1.0);  v.push_back(-1.0);//SEB
	//--------
	v.push_back(-1.0); v.push_back(-1.0); v.push_back(-1.0);  //IEB
	v.push_back(1.0); v.push_back(-1.0); v.push_back(-1.0);  //IDB
	v.push_back(-1.0); v.push_back(-1.0); v.push_back(1.0);  //IES

	v.push_back(1.0); v.push_back(-1.0); v.push_back(-1.0);   //IDB
	v.push_back(1.0); v.push_back(-1.0); v.push_back(1.0);  //SDB
	v.push_back(-1.0); v.push_back(-1.0); v.push_back(1.0); //SEB
	//--------
	v.push_back(-1.0); v.push_back(1.0); v.push_back(-1.0);  //IEB
	v.push_back(1.0); v.push_back(1.0); v.push_back(-1.0);  //IDB
	v.push_back(-1.0); v.push_back(1.0); v.push_back(1.0);  //IES

	v.push_back(1.0); v.push_back(1.0); v.push_back(-1.0);   //IDB
	v.push_back(1.0); v.push_back(1.0); v.push_back(1.0);  //SDB
	v.push_back(-1.0); v.push_back(1.0); v.push_back(1.0); //SEB
	//------
	v.push_back(1.0); v.push_back(-1.0); v.push_back(-1.0);  //IEF
	v.push_back(1.0); v.push_back(1.0); v.push_back(-1.0);  //IDF
	v.push_back(1.0); v.push_back(-1.0); v.push_back(1.0);  //SEF

	v.push_back(1.0); v.push_back(1.0); v.push_back(-1.0); //IDF
	v.push_back(1.0); v.push_back(1.0); v.push_back(1.0); //SDF
	v.push_back(1.0); v.push_back(-1.0); v.push_back(1.0); //SEF
	//------
	v.push_back(-1.0); v.push_back(-1.0); v.push_back(-1.0);  //IEF
	v.push_back(-1.0); v.push_back(1.0); v.push_back(-1.0);  //IDF
	v.push_back(-1.0); v.push_back(-1.0); v.push_back(1.0);  //SEF

	v.push_back(-1.0); v.push_back(1.0); v.push_back(-1.0);  //IDF
	v.push_back(-1.0); v.push_back(1.0); v.push_back(1.0);  //SDF
	v.push_back(-1.0); v.push_back(-1.0); v.push_back(1.0); //SEF


	return v;
}

vector<GLfloat> defineColors()
{
	vector<GLfloat> vec;
	vec.push_back(0.0); vec.push_back(0.0); vec.push_back(0.0);
	vec.push_back(0.0); vec.push_back(0.0); vec.push_back(0.0);
	vec.push_back(0.0); vec.push_back(0.0); vec.push_back(0.0);
	vec.push_back(0.0); vec.push_back(0.0); vec.push_back(0.0);
	vec.push_back(0.0); vec.push_back(0.0); vec.push_back(0.0);
	vec.push_back(0.0); vec.push_back(0.0); vec.push_back(0.0);

	vec.push_back(1.0); vec.push_back(0.0); vec.push_back(0.0);
	vec.push_back(1.0); vec.push_back(0.0); vec.push_back(0.0);
	vec.push_back(1.0); vec.push_back(0.0); vec.push_back(0.0);
	vec.push_back(1.0); vec.push_back(0.0); vec.push_back(0.0);
	vec.push_back(1.0); vec.push_back(0.0); vec.push_back(0.0);
	vec.push_back(1.0); vec.push_back(0.0); vec.push_back(0.0);

	vec.push_back(1.0); vec.push_back(1.0); vec.push_back(0.0);
	vec.push_back(1.0); vec.push_back(1.0); vec.push_back(0.0);
	vec.push_back(1.0); vec.push_back(1.0); vec.push_back(0.0);
	vec.push_back(1.0); vec.push_back(1.0); vec.push_back(0.0);
	vec.push_back(1.0); vec.push_back(1.0); vec.push_back(0.0);
	vec.push_back(1.0); vec.push_back(1.0); vec.push_back(0.0);

	vec.push_back(1.0); vec.push_back(1.0); vec.push_back(1.0);
	vec.push_back(1.0); vec.push_back(1.0); vec.push_back(1.0);
	vec.push_back(1.0); vec.push_back(1.0); vec.push_back(1.0);
	vec.push_back(1.0); vec.push_back(1.0); vec.push_back(1.0);
	vec.push_back(1.0); vec.push_back(1.0); vec.push_back(1.0);
	vec.push_back(1.0); vec.push_back(1.0); vec.push_back(1.0);

	vec.push_back(0.0); vec.push_back(1.0); vec.push_back(1.0);
	vec.push_back(0.0); vec.push_back(1.0); vec.push_back(1.0);
	vec.push_back(0.0); vec.push_back(1.0); vec.push_back(1.0);
	vec.push_back(0.0); vec.push_back(1.0); vec.push_back(1.0);
	vec.push_back(0.0); vec.push_back(1.0); vec.push_back(1.0);
	vec.push_back(0.0); vec.push_back(1.0); vec.push_back(1.0);

	vec.push_back(0.0); vec.push_back(0.0); vec.push_back(1.0);
	vec.push_back(0.0); vec.push_back(0.0); vec.push_back(1.0);
	vec.push_back(0.0); vec.push_back(0.0); vec.push_back(1.0);
	vec.push_back(0.0); vec.push_back(0.0); vec.push_back(1.0);
	vec.push_back(0.0); vec.push_back(0.0); vec.push_back(1.0);
	vec.push_back(0.0); vec.push_back(0.0); vec.push_back(1.0);

	vec.push_back(1.0); vec.push_back(0.0); vec.push_back(1.0);
	vec.push_back(1.0); vec.push_back(0.0); vec.push_back(1.0);
	vec.push_back(1.0); vec.push_back(0.0); vec.push_back(1.0);
	vec.push_back(1.0); vec.push_back(0.0); vec.push_back(1.0);
	vec.push_back(1.0); vec.push_back(0.0); vec.push_back(1.0);
	vec.push_back(1.0); vec.push_back(0.0); vec.push_back(1.0);

	return vec;
}

vector<GLfloat> defineNormals()
{
	vector<GLfloat> v;
	v.push_back(0); v.push_back(0); v.push_back(1);
	v.push_back(0); v.push_back(0); v.push_back(1);
	v.push_back(0); v.push_back(0); v.push_back(1);
	v.push_back(0); v.push_back(0); v.push_back(1);
	v.push_back(0); v.push_back(0); v.push_back(1);
	v.push_back(0); v.push_back(0); v.push_back(1);

	v.push_back(0); v.push_back(0); v.push_back(-1);
	v.push_back(0); v.push_back(0); v.push_back(-1);
	v.push_back(0); v.push_back(0); v.push_back(-1);
	v.push_back(0); v.push_back(0); v.push_back(-1);
	v.push_back(0); v.push_back(0); v.push_back(-1);
	v.push_back(0); v.push_back(0); v.push_back(-1);

	v.push_back(0); v.push_back(-1); v.push_back(0);
	v.push_back(0); v.push_back(-1); v.push_back(0);
	v.push_back(0); v.push_back(-1); v.push_back(0);
	v.push_back(0); v.push_back(-1); v.push_back(0);
	v.push_back(0); v.push_back(-1); v.push_back(0);
	v.push_back(0); v.push_back(-1); v.push_back(0);

	v.push_back(0); v.push_back(1); v.push_back(0);
	v.push_back(0); v.push_back(1); v.push_back(0);
	v.push_back(0); v.push_back(1); v.push_back(0);
	v.push_back(0); v.push_back(1); v.push_back(0);
	v.push_back(0); v.push_back(1); v.push_back(0);
	v.push_back(0); v.push_back(1); v.push_back(0);

	v.push_back(1); v.push_back(0); v.push_back(0);
	v.push_back(1); v.push_back(0); v.push_back(0);
	v.push_back(1); v.push_back(0); v.push_back(0);
	v.push_back(1); v.push_back(0); v.push_back(0);
	v.push_back(1); v.push_back(0); v.push_back(0);
	v.push_back(1); v.push_back(0); v.push_back(0);

	v.push_back(-1); v.push_back(0); v.push_back(0);
	v.push_back(-1); v.push_back(0); v.push_back(0);
	v.push_back(-1); v.push_back(0); v.push_back(0);
	v.push_back(-1); v.push_back(0); v.push_back(0);
	v.push_back(-1); v.push_back(0); v.push_back(0);
	v.push_back(-1); v.push_back(0); v.push_back(0);
	return v;
}
CubeExample::CubeExample()
{
	//O que tem que ser feito aqui:
	//1)Criar o shader do volume renderer
	myShader = make_shared<Shader>(defineVertexShader(), defineFragmentShader());
	//2)Criar o cubo de suporte
	vertexes = defineVertexes();
	vertexesVbo = CreateBuffer<float>(GL_ARRAY_BUFFER, vertexes);
	colors = defineColors();
	colorsVbo = CreateBuffer<float>(GL_ARRAY_BUFFER, colors);
	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	myShader->UseProgram();
	GLuint vpLocation = myShader->GetAttribute("vertex");
	GLuint normalLocation = myShader->GetAttribute("color");
	glEnableVertexAttribArray(vpLocation);
	glEnableVertexAttribArray(normalLocation);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexesVbo);
	glVertexAttribPointer(vpLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, colorsVbo);
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	//Valores iniciais das matrizes
	mProjectionMatrix << 1, 0, 0, 0,
						 0, 1, 0, 0,
						 0, 0, 1, 0,
						 0, 0, 0, 1;
	mViewMatrix << 1, 0, 0, 0,
				   0, 1, 0, 0,
				   0, 0, 1, 0,
				   0, 0, 0, 1;
	mModelMatrix << 1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1;
}

void CubeExample::Render()
{
	//Por enquanto a câmera é aqui mesmo...
	Vector3f eye, focus, vup;
	eye << 0.0, 2.0, -3.0;
	focus << 0.0, 0.0, 0.0;
	vup << 0.0, 1.0, 0.0;
	lookAt(eye, focus, vup);
	setPerspective(45, 1, 0.01, 100);
	Matrix4f cameraMat = mProjectionMatrix * mViewMatrix;
	//O que tem que ser feito aqui:
	//1)Exibir o cubo de suporte na tela
	//2)Rotacioná-lo para eu ver se está tudo ok.
	myShader->UseProgram();
	GLuint cameraMatLocation = myShader->GetUniform("cameraMat");
	GLuint modelMatLocation = myShader->GetUniform("modelMat");
	glUniformMatrix4fv(cameraMatLocation, 1, GL_FALSE, cameraMat.data());
	glUniformMatrix4fv(modelMatLocation, 1, GL_FALSE, mModelMatrix.data());

	glBindVertexArray(vao);
	GLuint vertexLocation = myShader->GetAttribute("vertex");
	glBindAttribLocation(myShader->GetProgramId(), vertexLocation, "vertex");
	GLuint normalLocation = myShader->GetAttribute("color");
	glBindAttribLocation(myShader->GetProgramId(), normalLocation, "color");

	glDrawArrays(GL_TRIANGLES, 0, vertexes.size() / 3);
	std::cout << "aa" << endl;
}

CubeExample::~CubeExample()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vertexesVbo);
}
void CubeExample::RotateAround(int axisId, float degs)
{
	Vector3f axis;
	if (axisId == 0)axis << 1, 0, 0;
	if (axisId == 1)axis << 0, 1, 0;
	if (axisId == 2)axis << 0, 0, 1;

	Matrix3f rotation = AngleAxisf(degs *3.141592653589793 / 180.0, axis).toRotationMatrix();
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


#include "volumeRendering.h"
#include <sstream>
#include "rendering3d.h"
#include "cubeExample.h"

using namespace std;

stringstream defineVertexShader()
{
	stringstream ss;
	ss << "#version 400" << endl;
	ss << "layout(location = 0) in vec3 vertex;" << endl;
	ss << "layout(location = 1) in vec3 normal;" << endl;
	ss << "uniform mat4 mvp;" << endl;
	ss << "uniform mat4 modelMatrix;" << endl;
	ss << "out vec3 fragVert;" << endl;
	ss << "out vec3 fragNormal;" << endl;
	ss << "void main(){" << endl;
	ss << "  fragVert = vertex;" << endl;
	ss << "  fragNormal = normal;" << endl;
	ss << "  gl_Position = mvp * vec4(vertex, 1.0);" << endl;
	ss << "}" << endl;
	return ss;
}
//https://www.tomdalling.com/blog/modern-opengl/06-diffuse-point-lighting/
stringstream defineFragmentShader()
{
	stringstream ss;
	ss << "#version 400" << endl;
	ss << "uniform mat4 modelMatrix;" << endl;
	ss << "in vec3 fragVert;" << endl;
	ss << "in vec3 fragNormal;" << endl;
	ss << "out vec4 finalColor;" << endl;
	ss << "void main(){" << endl;
	//Calcula a normal em world coordinates
	ss << " mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));" << endl;
	ss << " vec3 normal = normalize(normalMatrix * fragNormal);" << endl;
	//Calcula onde esse fragmento está em world coordinaltes
	ss << " vec3 fragPosition = vec3(model * vec4(fragVert,1));" << endl;
	//Calcula o vetor entre esse pixel(em wc) e a fonte de luz
	ss << " vec3 lightPos = vec3(4.0, 4.0, 4.0);" << endl;
	ss << " vec3 surfaceToLight = lightPos - fragPosition;" << endl;
	//calcula o coseno do angulo de incidencia
	ss << " float brightness = dot(no"
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
	normals = defineNormals();
	normalsVbo = CreateBuffer<float>(GL_ARRAY_BUFFER, normals);
	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	myShader->UseProgram();
	GLuint vpLocation = myShader->GetAttribute("vertex");
	GLuint normalLocation = myShader->GetAttribute("normal");
	glEnableVertexAttribArray(vpLocation);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexesVbo);
	glVertexAttribPointer(vpLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, normalsVbo);
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
	Matrix4f mv = mViewMatrix * mModelMatrix;
	Matrix4f mvp = mProjectionMatrix * mViewMatrix * mModelMatrix;
	//O que tem que ser feito aqui:
	//1)Exibir o cubo de suporte na tela
	//2)Rotacioná-lo para eu ver se está tudo ok.
	myShader->UseProgram();
	GLuint mvpLocation = myShader->GetUniform("mvp");
	GLuint modelMatrixLocation = myShader->GetUniform("modelMatrix");
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, mModelMatrix.data());//Passa a mModelMatrix pro shader, necessária pra transformar as normais.
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, mvp.data());

	glBindVertexArray(vao);
	GLuint vertexLocation = myShader->GetAttribute("vertex");
	glBindAttribLocation(myShader->GetProgramId(), vertexLocation, "vertex");
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

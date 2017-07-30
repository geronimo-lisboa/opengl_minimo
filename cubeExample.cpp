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
	ss << "layout (location = 2) in vec2 texCoord;" << endl;
	ss << "layout (location = 3) in vec3 normal;" << endl;
	ss << "uniform mat4 modelMat;" << endl;
	ss << "uniform mat4 cameraMat;" << endl;
	ss << "out vec3 vertexColor;" << endl;
	ss << "out vec2 vertexTexCoord;" << endl;
	ss << "out vec3 fragNormal;" << endl;
	ss << "out vec3 fragVert;" << endl;
	ss << "void main(){" << endl;
	ss << "  gl_Position = cameraMat * modelMat * vec4(vertex, 1.0);" << endl;
	ss << "  vertexColor = color;" << endl;
	ss << "  vertexTexCoord = texCoord;" << endl;
	ss << "  fragNormal = normal;" << endl;
	ss << "  fragVert = vertex;" << endl;
	ss << "}";
	return ss;
}
//https://www.tomdalling.com/blog/modern-opengl/06-diffuse-point-lighting/
stringstream defineFragmentShader()
{
	stringstream ss;
	ss << "#version 400" << endl;
	ss << "uniform mat4 modelMat;" << endl;
	ss << "uniform sampler2D textureSampler;" << endl;
	ss << "uniform bool isUsandoTextura;" << endl;
	ss << "uniform bool isUsandoIluminacao;" << endl;
	ss << "uniform vec3 lightLocation;" << endl;
	ss << "in vec3 vertexColor;" << endl;
	ss << "in vec2 vertexTexCoord;" << endl;
	ss << "in vec3 fragNormal;" << endl;
	ss << "in vec3 fragVert;" << endl;
	ss << "out vec4 finalColor;" << endl;
	ss << "void main(){" << endl;
	ss << "  if (!isUsandoTextura) {" << endl;
	ss << "    finalColor = vec4(vertexColor, 1.0);" << endl;
	ss << "  }else{" << endl;
	ss << "    finalColor = texture(textureSampler, vertexTexCoord);" << endl;
	ss << "  }" << endl;
	ss << "  if (isUsandoIluminacao){" << endl;
	ss << "    mat3 normalMatrix = transpose(inverse(mat3(modelMat)));" << endl;
	ss << "    vec3 transformedNormal = normalize(normalMatrix * fragNormal);" << endl;//As normais estão em modelspace, isso aqui as leva pra world space
	ss << "    vec3 fragPosition = vec3(modelMat * vec4(fragVert,1));" << endl;//A posição do fragmento atual em worldspace, necessária para calcular o angulo entre a normal do fragmento e a luz.
	ss << "    vec3 surfaceToLight = lightLocation - fragPosition;" << endl;//Vetor do pixel (em wc) pra luz.
	ss << "    float brightness = dot(transformedNormal, surfaceToLight)/ (length(surfaceToLight) * length(transformedNormal));" << endl;// O brilho é o cosseno do angulo entre a normal da superficie e o vetor do fragmento pra luz
	ss << "    finalColor = vec4(finalColor[0]*brightness, finalColor[1]*brightness, finalColor[2]*brightness, 1.0);" << endl;
	ss << "  }" << endl;
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
	//-----------------1
	v.push_back(0); v.push_back(0); v.push_back(1);
	v.push_back(0); v.push_back(0); v.push_back(1);
	v.push_back(0); v.push_back(0); v.push_back(1);

	v.push_back(0); v.push_back(0); v.push_back(1);
	v.push_back(0); v.push_back(0); v.push_back(1);
	v.push_back(0); v.push_back(0); v.push_back(1);
	//-----------------2
	v.push_back(0); v.push_back(0); v.push_back(-1);
	v.push_back(0); v.push_back(0); v.push_back(-1);
	v.push_back(0); v.push_back(0); v.push_back(-1);

	v.push_back(0); v.push_back(0); v.push_back(-1);
	v.push_back(0); v.push_back(0); v.push_back(-1);
	v.push_back(0); v.push_back(0); v.push_back(-1);
	//-----------------3
	v.push_back(0); v.push_back(-1); v.push_back(0);
	v.push_back(0); v.push_back(-1); v.push_back(0);
	v.push_back(0); v.push_back(-1); v.push_back(0);

	v.push_back(0); v.push_back(-1); v.push_back(0);
	v.push_back(0); v.push_back(-1); v.push_back(0);
	v.push_back(0); v.push_back(-1); v.push_back(0);
	//-----------------4
	v.push_back(0); v.push_back(1); v.push_back(0);
	v.push_back(0); v.push_back(1); v.push_back(0);
	v.push_back(0); v.push_back(1); v.push_back(0);

	v.push_back(0); v.push_back(1); v.push_back(0);
	v.push_back(0); v.push_back(1); v.push_back(0);
	v.push_back(0); v.push_back(1); v.push_back(0);
	//-----------------5
	v.push_back(1); v.push_back(0); v.push_back(0);
	v.push_back(1); v.push_back(0); v.push_back(0);
	v.push_back(1); v.push_back(0); v.push_back(0);

	v.push_back(1); v.push_back(0); v.push_back(0);
	v.push_back(1); v.push_back(0); v.push_back(0);
	v.push_back(1); v.push_back(0); v.push_back(0);
	//-----------------6
	v.push_back(-1); v.push_back(0); v.push_back(0);
	v.push_back(-1); v.push_back(0); v.push_back(0);
	v.push_back(-1); v.push_back(0); v.push_back(0);

	v.push_back(-1); v.push_back(0); v.push_back(0);
	v.push_back(-1); v.push_back(0); v.push_back(0);
	v.push_back(-1); v.push_back(0); v.push_back(0);
	return v;
}

vector<GLfloat> defineTexCoords()
{
	vector<GLfloat> texCoords;
	texCoords.push_back(0.0f); texCoords.push_back(0.0f);
	texCoords.push_back(0.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(0.0f);
	texCoords.push_back(0.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(0.0f);

	texCoords.push_back(0.0f); texCoords.push_back(0.0f);
	texCoords.push_back(0.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(0.0f);
	texCoords.push_back(0.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(0.0f);

	texCoords.push_back(0.0f); texCoords.push_back(0.0f);
	texCoords.push_back(0.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(0.0f);
	texCoords.push_back(0.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(0.0f);

	texCoords.push_back(0.0f); texCoords.push_back(0.0f);
	texCoords.push_back(0.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(0.0f);
	texCoords.push_back(0.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(0.0f);

	texCoords.push_back(0.0f); texCoords.push_back(0.0f);
	texCoords.push_back(0.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(0.0f);
	texCoords.push_back(0.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(0.0f);

	texCoords.push_back(0.0f); texCoords.push_back(0.0f);
	texCoords.push_back(0.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(0.0f);
	texCoords.push_back(0.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(0.0f);
	return texCoords;
}
CubeExample::CubeExample()
{
	usandoTextura = false;
	usandoIluminacao = true;
	//O que tem que ser feito aqui:
	//1)Criar o shader do volume renderer
	myShader = make_shared<Shader>(defineVertexShader(), defineFragmentShader());
	//2)Criar o cubo de suporte
	vertexes = defineVertexes();
	vertexesVbo = CreateBuffer<float>(GL_ARRAY_BUFFER, vertexes);
	colors = defineColors();
	colorsVbo = CreateBuffer<float>(GL_ARRAY_BUFFER, colors);
	texCoords = defineTexCoords();
	tcVbo = CreateBuffer<GLfloat>(GL_ARRAY_BUFFER, texCoords);
	normalCoords = defineNormals();
	normalVbo = CreateBuffer<GLfloat>(GL_ARRAY_BUFFER, normalCoords);
	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	myShader->UseProgram();
	GLuint vpLocation = myShader->GetAttribute("vertex");
	GLuint colorLocation = myShader->GetAttribute("color");
	GLuint texCoordLocation = myShader->GetAttribute("texCoord");
	GLuint normalLocation = myShader->GetAttribute("normal");
	glEnableVertexAttribArray(vpLocation);
	glEnableVertexAttribArray(colorLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glEnableVertexAttribArray(normalLocation);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexesVbo);
	glVertexAttribPointer(vpLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, colorsVbo);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, tcVbo);
	glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	//Valores iniciais das matrizes
}

void CubeExample::Render(shared_ptr<Camera> camera)
{
	Matrix4f cameraMat = camera->GetCameraMatrix();
	//O que tem que ser feito aqui:
	//1)Exibir o cubo de suporte na tela
	//2)Rotacioná-lo para eu ver se está tudo ok.
	myShader->UseProgram();
	GLuint cameraMatLocation = myShader->GetUniform("cameraMat");
	GLuint modelMatLocation = myShader->GetUniform("modelMat");
	GLuint isUsandoTexturaLocation = myShader->GetUniform("isUsandoTextura");
	GLuint textureSamplerLocation = myShader->GetUniform("textureSampler");
	GLuint isUsandoIluminacaoLocation = myShader->GetUniform("isUsandoIluminacao");
	GLuint lightLocationLocation = myShader->GetUniform("lightLocation");
	glUniformMatrix4fv(cameraMatLocation, 1, GL_FALSE, cameraMat.data());
	glUniformMatrix4fv(modelMatLocation, 1, GL_FALSE, mModelMatrix.data());
	glUniform1i(isUsandoTexturaLocation, usandoTextura);
	glUniform1i(isUsandoIluminacaoLocation, usandoIluminacao);
	glUniform3f(lightLocationLocation, 1.0, 5.0, -5.0);
	//Ativação da textura no shader
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureSamplerLocation, /*GL_TEXTURE*/0);
	glBindTexture(GL_TEXTURE_2D, textura);

	glBindVertexArray(vao);
	GLuint vertexLocation = myShader->GetAttribute("vertex");
	glBindAttribLocation(myShader->GetProgramId(), vertexLocation, "vertex");
	GLuint colorLocation = myShader->GetAttribute("color");
	glBindAttribLocation(myShader->GetProgramId(), colorLocation, "color");
	GLuint texCoordLocation = myShader->GetAttribute("texCoord");
	glBindAttribLocation(myShader->GetProgramId(), texCoordLocation, "texCoord");
	GLuint normalLocation = myShader->GetAttribute("normal");
	glBindAttribLocation(myShader->GetProgramId(), normalLocation, "normal");

	glDrawArrays(GL_TRIANGLES, 0, vertexes.size() / 3);
	std::cout << "aa" << endl;
}

void CubeExample::SetTextura(ImageType::Pointer tex)
{
	usandoTextura = true;
	textura = 0;
	glGenTextures(1, &textura);
	glBindTexture(GL_TEXTURE_2D, textura);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, tex->GetLargestPossibleRegion().GetSize()[0],
		tex->GetLargestPossibleRegion().GetSize()[1], 0, GL_RED, GL_FLOAT, tex->GetBufferPointer());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}
CubeExample::~CubeExample()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vertexesVbo);
}



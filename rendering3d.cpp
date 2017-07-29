#include "rendering3d.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <exception>
using namespace std;

void teste_opengl()
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::stringstream ss;
		ss << "erro de opengl " << err;
		throw std::exception(ss.str().c_str());
	}
}



template <typename T>const GLuint Object3d::CreateBuffer(GLenum target, std::vector<T> &vec)
{
	GLuint resultBuffer = 0;
	glGenBuffers(1, &resultBuffer);
	glBindBuffer(target, resultBuffer);
	glBufferData(target, vec.size() * sizeof(vec.at(0)), vec.data(), GL_STATIC_DRAW);
	return resultBuffer;
}

Object3dTexture2d::Object3dTexture2d(std::string vsfile, std::string fsfile, itk::Image<float, 2>::Pointer imagem) :
Object3d(vsfile, fsfile)
{
	this->image = imagem;
	//Criação da textura
	texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, imagem->GetLargestPossibleRegion().GetSize()[0],
		imagem->GetLargestPossibleRegion().GetSize()[1], 0, GL_RED, GL_FLOAT, imagem->GetBufferPointer());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


	vertexes.push_back(-1.0f); vertexes.push_back(-1.0f); vertexes.push_back(0.0f);
	vertexes.push_back(1.0f); vertexes.push_back(-1.0f); vertexes.push_back(0.0f);
	vertexes.push_back(-1.0f); vertexes.push_back(1.0f); vertexes.push_back(0.0f);
	vertexes.push_back(1.0f); vertexes.push_back(1.0f); vertexes.push_back(0.0f);

	colors.push_back(1.0f); colors.push_back(0.0f); colors.push_back(0.0f);
	colors.push_back(0.0f); colors.push_back(1.0f); colors.push_back(0.0f);
	colors.push_back(0.0f); colors.push_back(0.0f); colors.push_back(1.0f);
	colors.push_back(0.0f); colors.push_back(0.1f); colors.push_back(0.0f);

	texCoords.push_back(0.0f); texCoords.push_back(0.0f);
	texCoords.push_back(0.0f); texCoords.push_back(1.0f);
	texCoords.push_back(1.0f); texCoords.push_back(0.0f);
	texCoords.push_back(1.0f); texCoords.push_back(1.0f);

	vertexesVbo = CreateBuffer<float>(GL_ARRAY_BUFFER, vertexes);
	colorsVbo = CreateBuffer<float>(GL_ARRAY_BUFFER, colors);
	texVbo = CreateBuffer<float>(GL_ARRAY_BUFFER, texCoords);

	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	shader.UseProgram();
	//eu SEI qual é o nome das coisas no shader e esses nomes vem de lá
	GLuint vpLocation = shader.GetAttribute("vp");
	GLuint vcLocation = shader.GetAttribute("vc");
	GLuint uvLocation = shader.GetAttribute("uv");
	glEnableVertexAttribArray(vpLocation);
	glEnableVertexAttribArray(vcLocation);
	glEnableVertexAttribArray(uvLocation);
	glUseProgram(0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexesVbo);
	glVertexAttribPointer(vpLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, colorsVbo);
	glVertexAttribPointer(vcLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, texVbo);
	glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, 0, NULL);

}

void Object3dTexture2d::Render()
{
	shader.UseProgram();
	glBindVertexArray(vao);
	GLuint vpLocation = shader.GetAttribute("vp");
	GLuint vcLocation = shader.GetAttribute("vc");
	GLuint uvLocation = shader.GetAttribute("uv");
	GLuint textureSamplerLocation = shader.GetUniform("myTextureSampler");
	GLuint useTextureLocation = shader.GetUniform("useTexture");

	glUniform1i(useTextureLocation, true);//Flag de controle no shader

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureSamplerLocation, 0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBindAttribLocation(shader.GetProgramId(), vpLocation, "vp");
	glBindAttribLocation(shader.GetProgramId(), vcLocation, "vc");
	glBindAttribLocation(shader.GetProgramId(), uvLocation, "uv");
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	teste_opengl();
}

Object3dTexture3d::Object3dTexture3d(std::string vsfile, std::string fsfile, itk::Image<float, 3>::Pointer imagem) :
Object3d(vsfile, fsfile)
{
	this->image = imagem;
	//cria a textura

	texture = 0;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_3D, texture);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexImage3D(GL_TEXTURE_3D, 0,
		GL_R16F,//INTERNAL FORMAT
		imagem->GetLargestPossibleRegion().GetSize()[0],
		imagem->GetLargestPossibleRegion().GetSize()[1],
		imagem->GetLargestPossibleRegion().GetSize()[2], 0,
		 GL_RED,//FORMAT
		GL_FLOAT, //TYPE
		image->GetBufferPointer());
	//cria os buffers
	vertexes.push_back(-1.0f); vertexes.push_back(-1.0f); vertexes.push_back(0.0f);
	vertexes.push_back(1.0f); vertexes.push_back(-1.0f); vertexes.push_back(0.0f);
	vertexes.push_back(-1.0f); vertexes.push_back(1.0f); vertexes.push_back(0.0f);
	vertexes.push_back(1.0f); vertexes.push_back(1.0f); vertexes.push_back(0.0f);

	colors.push_back(1.0f); colors.push_back(0.0f); colors.push_back(0.0f);
	colors.push_back(0.0f); colors.push_back(1.0f); colors.push_back(0.0f);
	colors.push_back(0.0f); colors.push_back(0.0f); colors.push_back(1.0f);
	colors.push_back(0.0f); colors.push_back(0.1f); colors.push_back(0.0f);

	texCoords.push_back(0.0f); texCoords.push_back(0.0f);texCoords.push_back(0.5f);
	texCoords.push_back(0.0f); texCoords.push_back(1.0f);texCoords.push_back(0.5f);
	texCoords.push_back(1.0f); texCoords.push_back(0.0f);texCoords.push_back(0.5f);
	texCoords.push_back(1.0f); texCoords.push_back(1.0f);texCoords.push_back(0.5f);

	vertexesVbo = CreateBuffer<float>(GL_ARRAY_BUFFER, vertexes);
	colorsVbo = CreateBuffer<float>(GL_ARRAY_BUFFER, colors);
	texVbo = CreateBuffer<float>(GL_ARRAY_BUFFER, texCoords);

	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	shader.UseProgram();
	//eu SEI qual é o nome das coisas no shader e esses nomes vem de lá
	GLuint vpLocation = shader.GetAttribute("vp");
	GLuint vcLocation = shader.GetAttribute("vc");
	GLuint uvLocation = shader.GetAttribute("uv");
	glEnableVertexAttribArray(vpLocation);
	glEnableVertexAttribArray(vcLocation);
	glEnableVertexAttribArray(uvLocation);
	glUseProgram(0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexesVbo);
	glVertexAttribPointer(vpLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, colorsVbo);
	glVertexAttribPointer(vcLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, texVbo);
	glVertexAttribPointer(uvLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);

}
void Object3dTexture3d::Render()
{
	shader.UseProgram();
	glBindVertexArray(vao);
	GLuint vpLocation = shader.GetAttribute("vp");
	GLuint vcLocation = shader.GetAttribute("vc");
	GLuint uvLocation = shader.GetAttribute("uv");
	GLuint textureSamplerLocation = shader.GetUniform("myTextureSampler");
	GLuint useTextureLocation = shader.GetUniform("useTexture");

	glUniform1i(useTextureLocation, true);//Flag de controle no shader

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureSamplerLocation, 0);
	glBindTexture(GL_TEXTURE_3D, texture);

	glBindAttribLocation(shader.GetProgramId(), vpLocation, "vp");
	glBindAttribLocation(shader.GetProgramId(), vcLocation, "vc");
	glBindAttribLocation(shader.GetProgramId(), uvLocation, "uv");
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	teste_opengl();
}
static float posNaTex = 0.5;

void Object3dTexture3d::Avancar()
{
	posNaTex = posNaTex + 0.01;
	texCoords.clear();
	texCoords.push_back(0.0f); texCoords.push_back(0.0f); texCoords.push_back(posNaTex);
	texCoords.push_back(0.0f); texCoords.push_back(1.0f); texCoords.push_back(posNaTex);
	texCoords.push_back(1.0f); texCoords.push_back(0.0f); texCoords.push_back(posNaTex);
	texCoords.push_back(1.0f); texCoords.push_back(1.0f); texCoords.push_back(posNaTex);
	glBindBuffer(GL_ARRAY_BUFFER, texVbo);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(texCoords.at(0)), texCoords.data(), GL_STATIC_DRAW);

}
void Object3dTexture3d::Recuar()
{
	posNaTex = posNaTex - 0.01;
	texCoords.clear();
	texCoords.push_back(0.0f); texCoords.push_back(0.0f); texCoords.push_back(posNaTex);
	texCoords.push_back(0.0f); texCoords.push_back(1.0f); texCoords.push_back(posNaTex);
	texCoords.push_back(1.0f); texCoords.push_back(0.0f); texCoords.push_back(posNaTex);
	texCoords.push_back(1.0f); texCoords.push_back(1.0f); texCoords.push_back(posNaTex);
	glBindBuffer(GL_ARRAY_BUFFER, texVbo);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(texCoords.at(0)), texCoords.data(), GL_STATIC_DRAW);

}
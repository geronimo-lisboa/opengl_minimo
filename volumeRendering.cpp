#include "volumeRendering.h"
#include <sstream>
#include "rendering3d.h"

using namespace std;

stringstream defineVertexBuffer()
{
	stringstream ss;
	ss << "#version 400" << endl;
	ss << "layout(location = 0) in vec3 vertex;" << endl;
	return ss;
}

MyVolumeRenderer::MyVolumeRenderer()
{
	//O que tem que ser feito aqui:
	//1)Criar o shader do volume renderer
	//2)Criar o cubo de suporte
}
MyVolumeRenderer::~MyVolumeRenderer()
{
	//O que deve ser feito aqui:
	//Deletar shader.
	//Deletar buffers.
}
void MyVolumeRenderer::Render()
{
	//O que tem que ser feito aqui:
	//1)Exibir o cubo de suporte na tela
	//2)Rotacioná-lo para eu ver se está tudo ok.
}
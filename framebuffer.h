#pragma once
#include <gl/glew.h>
namespace my3d
{
	//O objeto do framebuffer � um quad com a mvp = identidade, onde a cena � desenhada
	class Framebuffer
	{
	private:
		//Relativo ao framebuffer
		GLuint fb, fbColor, fbDepth;
		GLenum *DrawBuffers;
		int screenWidth, screenHeight;
	public:
		Framebuffer(int screenWidth, int screenHeight);
		void UseFramebuffer();
	};
}
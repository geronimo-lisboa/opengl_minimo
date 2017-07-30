#include "framebuffer.h"

namespace  my3d
{
	Framebuffer::Framebuffer(int screenWidth, int screenHeight)
	{
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		//Cria o framebuffer, sua textura e seu depth buffer.
		glGenFramebuffers(1, &fb);
		glBindFramebuffer(GL_FRAMEBUFFER, fb);
		glGenTextures(1, &fbColor);
		glBindTexture(GL_TEXTURE_2D, fbColor);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->screenWidth, this->screenHeight, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, 0);//A textura estará vazia pq receberá o dado do framebuffer.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenRenderbuffers(1, &fbDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, fbDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, this->screenWidth, this->screenHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbDepth);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, fbColor, 0);
	}

	void Framebuffer::UseFramebuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fb);
		glViewport(0, 0, this->screenWidth, this->screenHeight);
	}
}

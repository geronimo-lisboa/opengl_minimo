#include <GL/glew.h>
#include <Windows.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "itkTypes.h"
#include "misc.h"
#include <memory>
#include <vector>
#include <itkMinimumMaximumImageFilter.h>
#include <map>
#include "rendering3d.h"
#include "loadDicom.h"
#include "cubeExample.h"
#include "camera.h"
#include "framebuffer.h"
const std::string imagePath = GetExecutablePath();
const int screenWidth = 300;
const int screenHeight = 300;

shared_ptr<Camera> camera = nullptr;
shared_ptr<Object3d> obj = nullptr;
shared_ptr<CubeExample> volume = nullptr;
shared_ptr<Framebuffer> framebuffer = nullptr;

int main(int argc, char** argv)
{
	try
	{
		//1) Carga da imagem
		ImageLoaderType::Pointer imageLoader = ImageLoaderType::New();
		imageLoader->SetFileName(imagePath + "phantom.png");
		imageLoader->Update();
		ImageType::Pointer originalImage = imageLoader->GetOutput();

		//2)Cria��o da janela/contexto/blablabla da glfw.
		GLFWwindow* window;
		//Seta o callback de erro.
		glfwSetErrorCallback([](int error, const char* description){
			std::stringstream ss;
			ss << "Erro: " << error << " : " << description;
			throw std::exception(ss.str().c_str());
		});
		//Se falhou em iniciar, morre
		if (!glfwInit()) { throw std::exception("Falha no glfwInit(). Tem algo MUITO errado acontecendo"); }
		//OpenGL 3.2
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		window = glfwCreateWindow(screenWidth, screenHeight, "BOA NOITE CARALHO", NULL, NULL);//A cria��o da janela � aqui
		if (!window)//Se falhou em criar a janela, morre.
		{
			glfwTerminate();
			throw std::exception("N�o foi possivel criar a janela");
		}

		glfwMakeContextCurrent(window);
		// start GLEW extension handler
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			/* A fonte de erro mais comum � tentar iniciar a glew sem ter um contexto opengl funcionando,
			sem ter a tela ativa.*/
			std::stringstream ss;
			ss << "Erro de glew: " << glewGetErrorString(err);
			throw std::exception(ss.str().c_str());
		}
		//O makeCurrent() da glfw
		glfwSwapInterval(1);
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);


		bool isInitialized = false;
		float ang = 0;
		while (!glfwWindowShouldClose(window))
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			if (!isInitialized)
			{
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LESS);
				//obj = make_shared<Object3dTexture2d>(imagePath + "vertexShader.glsl", imagePath + "fragmentShader.glsl", originalImage); --TESTE PRA CARREGAR UMA TEXTURA
				//obj = make_shared<Object3dTexture3d>(imagePath + "vertexShader3d.glsl", imagePath + "fragmentShader3d.glsl", tomography);--TESTE DA TEXTURA 3D
				volume = make_shared<CubeExample>();
				volume->SetTextura(originalImage);
				//Seta o callback de tecla;
				glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
				{
					if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)//Esc = encerra o programa.
						glfwSetWindowShouldClose(window, GLFW_TRUE);
					if (key == GLFW_KEY_W && action == GLFW_PRESS)
					{
						Object3dTexture3d* myObj = dynamic_cast<Object3dTexture3d*>(obj.get());
						myObj->Avancar();
					}
					if (key == GLFW_KEY_S && action == GLFW_PRESS)
					{
						Object3dTexture3d* myObj = dynamic_cast<Object3dTexture3d*>(obj.get());
						myObj->Recuar();
					}
				});

				array<float, 3> eye = { {0.0, 2.0, -3.0} };
				array<float, 3> focus = { {0.0, 0.0, 0.0} };
				array<float, 3> vup = { {0.0, 1.0, 0.0} };
				camera = make_shared<Camera>(eye, focus, vup, 45, 1, 0.01, 100);
				camera->screenHeight = screenHeight;
				camera->screenWidth = screenWidth;
				framebuffer = make_shared<Framebuffer>(camera->screenWidth, camera->screenHeight);
				isInitialized = true;
			}
			else
			{
				volume->RotateAround(1, ang);
				ang = ang+1;

				volume->Render(camera);
			}
			glfwPollEvents();
			glfwSwapBuffers(window);
		}
		//4) Fim do loop principal
		//Limpa tudo e morre.
		glfwDestroyWindow(window);
		glfwTerminate();
		exit(EXIT_SUCCESS);

	}
	catch (std::exception &ex)
	{
		std::cout << "Erro : " << ex.what() << endl;
		std::cout << "Pressione qqer tecla para continuar...";
		getchar();
		exit(EXIT_FAILURE);
	}

}
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
#include "volumeRendering.h"
const std::string imagePath = GetExecutablePath();
const int screenWidth = 300;
const int screenHeight = 300;

shared_ptr<Object3d> obj = nullptr;
shared_ptr<MyVolumeRenderer> volume = nullptr;

int main(int argc, char** argv)
{
	try
	{
		//1) Carga da imagem
		ImageLoaderType::Pointer imageLoader = ImageLoaderType::New();
		imageLoader->SetFileName(imagePath + "phantom.png");
		imageLoader->Update();
		ImageType::Pointer originalImage = imageLoader->GetOutput();
		Float3dImageType::Pointer tomography = loadDicom("C:\\Users\\geronimo\\dicom\\Marching Man");
		itk::MinimumMaximumImageFilter<Float3dImageType>::Pointer minmax = itk::MinimumMaximumImageFilter<Float3dImageType>::New();
		minmax->SetInput(tomography);
		minmax->Update();
		std::cout << "max = " << minmax->GetMaximum() << std::endl;
		std::cout << "min = " << minmax->GetMinimum() << std::endl;

		//2)Criação da janela/contexto/blablabla da glfw.
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
		window = glfwCreateWindow(screenWidth, screenHeight, "BOA NOITE CARALHO", NULL, NULL);//A criação da janela é aqui
		if (!window)//Se falhou em criar a janela, morre.
		{
			glfwTerminate();
			throw std::exception("Não foi possivel criar a janela");
		}

		glfwMakeContextCurrent(window);
		// start GLEW extension handler
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			/* A fonte de erro mais comum é tentar iniciar a glew sem ter um contexto opengl funcionando,
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
				volume = make_shared<MyVolumeRenderer>();
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
				isInitialized = true;
			}
			else
			{
				volume->RotateAround(1, ang);
				ang = ang+1;
				//obj->Render(); -- Passei pro teste do volume renderer...
				volume->Render();
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
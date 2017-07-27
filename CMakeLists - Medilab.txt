cmake_minimum_required(VERSION 2.8)
set(CMAKE_SUPPRESS_REGENERATION true)
project(opengl_minimo)

#set(VTK_DIR "C:/lib/VTK-7.1.1/VTK-7.1.1")
set(ITK_DIR "C:/itk_nova/ITK_build")
# Find VTK
#find_package(VTK REQUIRED)
#include(${VTK_USE_FILE})
# Find ITK
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

file(GLOB my_source_files "*.h" "*.cpp" "*.cxx")
add_executable(opengl_minimo ${my_source_files} )  
#bota vtk e ITK
target_link_libraries(opengl_minimo   ${ITK_LIBRARIES})
#Adds OpenGL
find_package(OpenGL)
target_link_libraries(opengl_minimo ${OPENGL_LIBRARIES})
#Adds GLEW
include_directories(C:/libs/glew-2.0.0/include)
target_link_libraries(opengl_minimo C:/libs/glew-2.0.0/lib/Release/Win32/glew32.lib)
#Adds GLFW
include_directories(C:/libs/glfw/include)
target_link_libraries(opengl_minimo debug C:/libs/glfw_bin/src/Debug/glfw3.lib)
target_link_libraries(opengl_minimo optimized C:/libs/glfw_bin/src/Release/glfw3.lib)
#Adds EIGEN3
SET (EIGEN3_INCLUDE_DIR "C:/Program Files (x86)/Eigen3/include")
include_directories(${EIGEN3_INCLUDE_DIR})


#suprime o warning chato dos infernos que a ITK gera por usar funções inseguras da api do windows
add_definitions(-D_SCL_SECURE_NO_WARNINGS)




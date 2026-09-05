#include <iostream>

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <shader.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Cube.hpp>

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR , 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR , 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE , GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800 , 600 , "14_multiple_cubes" , nullptr , nullptr);
    if(window == nullptr){
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    glViewport(0,0,800,600);

    Cube cube("../assets/container.png","../shader/shader.vs","../shader/shader.fs");

    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cube.drawRotate({1.0f , 1.0f , 1.0f} , {0.0f , 0.0f , -10.0F} , 0.0f);
        cube.drawRotate({1.0f , 1.0f , 1.0f} , {1.0f , 1.0f , -5.0F} , 0.0f);
        cube.drawRotate({1.0f , 1.0f , 1.0f} , {-1.0f , -1.0f , -5.0F} , 0.0f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Cube.hpp>
#include <Camera.hpp>


float lastX = 400.0f;
float lastY = 300.0f;
bool firstMouse = true;

Camera camera {};

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
        return;
    }

    float xOffset = static_cast<float>(xpos - lastX);
    float yOffset = static_cast<float>(ypos - lastY);

    lastX = xpos;
    lastY = ypos;

    camera.processMouse(xOffset, yOffset);
}

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "20_3D_room", nullptr, nullptr);
    if (window == nullptr)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, 800, 600);

    Cube cube("../assets/wall.jpg" , "../shader/cube_shader.vs" , "../shader/cube_shader.fs");

    float lastFrame = 0.0f;
    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        float currentFrame = glfwGetTime();
        float dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.m_dt = dt;

        camera.processKeyboard(window);

        glm::mat4 view = camera.getViewMatrix();
        cube.m_view = view;

        // front wall
        cube.draw(glm::vec3(1.0f , 3.0f , 0.2f));
        cube.draw(glm::vec3(1.0f , 1.0f , 0.2f) , {1.0f , 1.0f , 0.0f});
        cube.draw(glm::vec3(1.0f , 1.0f , 0.2f) , {2.0f , 1.0f , 0.0f});
        cube.draw(glm::vec3(1.0f , 3.0f , 0.2f) , {3.0f , 0.0f , 0.0f});

        // right wall
        cube.draw(glm::vec3(1.0f , 3.0f , 0.2f) , {3.5f , 0.0f , -3.5f} , 90.0f);
        cube.draw(glm::vec3(1.0f , 3.0f , 0.2f) , {3.5f , 0.0f , -2.5f} , 90.0f);
        cube.draw(glm::vec3(1.0f , 3.0f , 0.2f) , {3.5f , 0.0f , -1.5f} , 90.0f);
        cube.draw(glm::vec3(1.0f , 3.0f , 0.2f) , {3.5f , 0.0f , -0.5f} , 90.0f);

        // left wall
        cube.draw(glm::vec3(1.0f , 3.0f , 0.2f) , {-0.5f , 0.0f , -3.5f} , 90.0f);
        cube.draw(glm::vec3(1.0f , 3.0f , 0.2f) , {-0.5f , 0.0f , -2.5f} , 90.0f);
        cube.draw(glm::vec3(1.0f , 3.0f , 0.2f) , {-0.5f , 0.0f , -1.5f} , 90.0f);
        cube.draw(glm::vec3(1.0f , 3.0f , 0.2f) , {-0.5f , 0.0f , -0.5f} , 90.0f);

        // back wall
        cube.draw(glm::vec3(1.0f , 3.0f , 0.2f) , {0.0f , 0.0f , -4.0f});
        cube.draw(glm::vec3(1.0f , 3.0f , 0.2f) , {1.0f , 0.0f , -4.0f});
        cube.draw(glm::vec3(1.0f , 3.0f , 0.2f) , {2.0f , 0.0f , -4.0f});
        cube.draw(glm::vec3(1.0f , 3.0f , 0.2f) , {3.0f , 0.0f , -4.0f});

        // floor
        cube.draw(glm::vec3(4.0f , 0.1f , 4.0f), {1.5f , -1.5f , -2.0f});

        // ceil
        cube.draw(glm::vec3(4.0f , 0.1f , 4.0f), {1.5f , 1.5f , -2.0f});

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

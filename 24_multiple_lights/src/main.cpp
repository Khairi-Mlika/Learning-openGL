#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <shader.h>
#include <stb_image.h>

#include <camera.hpp>
#include <cube.hpp>

struct LightPoint{
    glm::vec3 lightPos;
    glm::vec3 lightColor;
};

float lastX = 400.0f;
float lastY = 300.0f;
bool firstMouse = true;

Camera camera{};

void mouseCallback(GLFWwindow *window, double xpos, double ypos)
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

    GLFWwindow *window = glfwCreateWindow(800, 600, "23_moving_light", nullptr, nullptr);
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
    glEnable(GL_DEPTH_TEST);

    LightPoint lightPoints[3];

    lightPoints[0].lightColor = glm::vec3(1.0f , 0.0f , 0.0f);
    lightPoints[0].lightPos = glm::vec3(0.0f , 1.0f , -3.0f);

    lightPoints[1].lightColor = glm::vec3(0.0f , 1.0f , 0.0f);
    lightPoints[1].lightPos = glm::vec3(-1.0f , 1.0f , 3.0f);

    lightPoints[2].lightColor = glm::vec3(0.0f , 0.0f , 1.0f);
    lightPoints[2].lightPos = glm::vec3(1.0f , 1.0f , 3.0f);

    Cube container("../assets/container.png","../shader/light_shader.vs","../shader/light_shader.fs");

    container.m_shaderProgram.use();

    container.m_shaderProgram.setVec3("lightPoints[0].lightColor" , lightPoints[0].lightColor);
    container.m_shaderProgram.setVec3("lightPoints[0].lightPos" , lightPoints[0].lightPos);

    container.m_shaderProgram.setVec3("lightPoints[1].lightColor" , lightPoints[1].lightColor);
    container.m_shaderProgram.setVec3("lightPoints[1].lightPos" , lightPoints[1].lightPos);

    container.m_shaderProgram.setVec3("lightPoints[2].lightColor" , lightPoints[2].lightColor);
    container.m_shaderProgram.setVec3("lightPoints[2].lightPos" , lightPoints[2].lightPos);

    Cube redLight("../assets/container.png","../shader/light_cube_shader.vs","../shader/light_cube_shader.fs");

    redLight.m_shaderProgram.use();
    redLight.m_shaderProgram.setVec3("Color",lightPoints[0].lightColor);

    Cube greenLight("../assets/container.png","../shader/light_cube_shader.vs","../shader/light_cube_shader.fs");

    greenLight.m_shaderProgram.use();
    greenLight.m_shaderProgram.setVec3("Color",lightPoints[1].lightColor);

    Cube blueLight("../assets/container.png","../shader/light_cube_shader.vs","../shader/light_cube_shader.fs");

    blueLight.m_shaderProgram.use();
    blueLight.m_shaderProgram.setVec3("Color",lightPoints[2].lightColor);

    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        camera.m_dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.processKeyboard(window);

        glm::mat4 view = camera.getViewMatrix();

        container.m_view = view;

        redLight.m_view = view;
        greenLight.m_view = view;
        blueLight.m_view = view;

        container.draw();

        redLight.draw(glm::vec3(.25f) , lightPoints[0].lightPos , 0.0f);
        greenLight.draw(glm::vec3(.25f) , lightPoints[1].lightPos , 0.0f);
        blueLight.draw(glm::vec3(.25f) , lightPoints[2].lightPos , 0.0f);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
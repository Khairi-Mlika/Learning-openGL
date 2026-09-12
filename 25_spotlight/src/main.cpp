#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <shader.h>
#include <stb_image.h>

#include <camera.hpp>
#include <cube.hpp>

struct LightPoint
{
    glm::vec3 lightColor = glm::vec3(1.0f);
    glm::vec3 lightPos = glm::vec3(0.0f, 5.2f, 0.0f);
    glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
    float cutoff = glm::cos(glm::radians(12.5f));
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

    GLFWwindow *window = glfwCreateWindow(800, 600, "25_spotlight", nullptr, nullptr);
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

    LightPoint lightPoint{};

    Cube lightCube("../assets/container.png", "../shader/light_cube.vs", "../shader/light_cube.fs");

    lightCube.m_shaderProgram.use();
    lightCube.m_shaderProgram.setVec3("lightColor", lightPoint.lightColor);

    Cube container("../assets/container.png", "../shader/container.vs", "../shader/container.fs");

    container.m_shaderProgram.use();
    container.m_shaderProgram.setVec3("lightPoint.lightColor", lightPoint.lightColor);
    container.m_shaderProgram.setVec3("lightPoint.lightPos", lightPoint.lightPos);
    container.m_shaderProgram.setVec3("lightPoint.direction", lightPoint.direction);
    container.m_shaderProgram.setFloat("lightPoint.cutoff", lightPoint.cutoff);

    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        camera.m_dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.processKeyboard(window);

        glm::mat4 view = camera.getViewMatrix();

        lightCube.m_view = view;
        lightCube.draw(glm::vec3(0.25f), lightPoint.lightPos, 0.0f);

        container.m_view = view;
        container.m_shaderProgram.use();
        container.m_shaderProgram.setVec3("viewPos", camera.m_cameraPos);
        container.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
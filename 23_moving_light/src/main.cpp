#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <shader.h>
#include <stb_image.h>

#include <camera.hpp>
#include <cube.hpp>

glm::vec3 lightColor = glm::vec3(1.0f);

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

    Cube container("../assets/container.png", "../shader/container_shader.vs", "../shader/container_shader.fs");
    Cube light("../assets/container.png", "../shader/light_shader.vs", "../shader/light_shader.fs");

    container.m_shaderProgram.use();
    container.m_shaderProgram.setVec3("lightColor", lightColor);

    float radius = 3.0f;
    float speed = 1.0f;

    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        camera.m_dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.processKeyboard(window);

        glm::mat4 view = camera.getViewMatrix();

        float angle = glfwGetTime() * speed;
        glm::vec3 lightPos = glm::vec3(
            cos(angle) * radius,
            1.0f,
            sin(angle) * radius);

        container.m_view = view;
        container.m_shaderProgram.use();
        container.m_shaderProgram.setVec3("lightPos", lightPos);
        container.m_shaderProgram.setVec3("viewPos", camera.m_cameraPos);
        container.draw();

        light.m_view = view;
        light.draw(glm::vec3(0.25f), lightPos, 0.0f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
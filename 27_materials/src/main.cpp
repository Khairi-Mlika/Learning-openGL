#include <iostream>

#include <vector>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <shader.h>
#include <stb_image.h>

#include <camera.hpp>
#include <cube.hpp>

struct Material
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct LightPoint
{
    glm::vec3 color = glm::vec3(1.0f);
    glm::vec3 position = glm::vec3(-1.0f, 1.2f, 0.0f);
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

    std::vector<Material> materials =
        {
            // Red plastic
            {
                glm::vec3(0.30f, 0.00f, 0.00f), // ambient
                glm::vec3(0.60f, 0.00f, 0.00f), // diffuse
                glm::vec3(0.80f, 0.60f, 0.60f), // specular
                32.0f                           // shininess
            },

            // Brass
            {
                glm::vec3(0.329f, 0.224f, 0.027f),
                glm::vec3(0.780f, 0.569f, 0.114f),
                glm::vec3(0.992f, 0.941f, 0.808f),
                27.8f},

            // Bronze
            {
                glm::vec3(0.213f, 0.128f, 0.054f),
                glm::vec3(0.714f, 0.428f, 0.181f),
                glm::vec3(0.394f, 0.272f, 0.167f),
                25.6f},

            // Polished bronze
            {
                glm::vec3(0.250f, 0.148f, 0.065f),
                glm::vec3(0.400f, 0.237f, 0.104f),
                glm::vec3(0.775f, 0.459f, 0.201f),
                76.8f},

            // Chrome
            {
                glm::vec3(0.250f, 0.250f, 0.250f),
                glm::vec3(0.400f, 0.400f, 0.400f),
                glm::vec3(0.775f, 0.775f, 0.775f),
                76.8f},

            // Copper
            {
                glm::vec3(0.191f, 0.074f, 0.023f),
                glm::vec3(0.704f, 0.270f, 0.083f),
                glm::vec3(0.257f, 0.138f, 0.086f),
                12.8f},

            // Gold
            {
                glm::vec3(0.247f, 0.199f, 0.075f),
                glm::vec3(0.751f, 0.606f, 0.226f),
                glm::vec3(0.628f, 0.556f, 0.366f),
                51.2f},

            // Emerald
            {
                glm::vec3(0.022f, 0.175f, 0.022f),
                glm::vec3(0.076f, 0.614f, 0.076f),
                glm::vec3(0.633f, 0.728f, 0.633f),
                76.8f},

            // Pearl
            {
                glm::vec3(0.250f, 0.207f, 0.207f),
                glm::vec3(1.000f, 0.829f, 0.829f),
                glm::vec3(0.297f, 0.297f, 0.297f),
                11.3f},

            // Ruby
            {
                glm::vec3(0.175f, 0.012f, 0.012f),
                glm::vec3(0.615f, 0.041f, 0.041f),
                glm::vec3(0.728f, 0.627f, 0.627f),
                76.8f}};

    LightPoint light{};

    Cube cube("../assets/container.png", "../shader/material_shader.vs", "../shader/material_shader.fs");

    cube.m_shaderProgram.use();

    cube.m_shaderProgram.setVec3("light.position", light.position);
    cube.m_shaderProgram.setVec3("light.color", light.color);

    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        camera.m_dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.processKeyboard(window);

        cube.m_view = camera.getViewMatrix();

        cube.m_shaderProgram.use();
        cube.m_shaderProgram.setVec3("viewPos", camera.m_cameraPos);

        for (size_t i = 0; i < materials.size(); i++)
        {
            cube.m_shaderProgram.setVec3("material.ambient", materials[i].ambient);
            cube.m_shaderProgram.setVec3("material.diffuse", materials[i].diffuse);
            cube.m_shaderProgram.setVec3("material.specular", materials[i].specular);
            cube.m_shaderProgram.setFloat("material.shininess", materials[i].shininess);

            cube.draw(glm::vec3(1.0f), glm::vec3((float)i * 2.5f, 0.0f, 0.0f), 0.0f);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
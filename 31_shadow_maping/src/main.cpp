#include <iostream>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <shader.h>
#include <stb_image.h>

#include <camera.hpp>
#include <cube.hpp>

float SRC_WIDTH = 800.0f;
float SRC_HEIGHT = 600.0f;

float SHADOW_WIDTH = SRC_WIDTH;
float SHADOW_HEIGHT = SRC_HEIGHT;

float lastX = 400.0f;
float lastY = 300.0f;
bool firstMouse = true;

Camera camera{};

struct LightSrouce
{
    glm::vec3 color;
    glm::vec3 position;
    glm::vec3 target;
};

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

    GLFWwindow *window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "31_shadow_maping", nullptr, nullptr);
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
    glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    unsigned int depthMap;

    glGenTextures(1, &depthMap);

    glBindTexture(GL_TEXTURE_2D, depthMap);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_DEPTH_COMPONENT,
        SHADOW_WIDTH,
        SHADOW_HEIGHT,
        0,
        GL_DEPTH_COMPONENT,
        GL_FLOAT,
        nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Clamp

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    // Border color

    float borderColor[] =
        {
            1.0f,
            1.0f,
            1.0f,
            1.0f};

    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        depthMap,
        0);

    glReadBuffer(GL_NONE);
    glDrawBuffer(GL_NONE);

    // light

    LightSrouce light{glm::vec3(1.0f), glm::vec3(-2.0f, 4.0F, -1.0f), glm::vec3(0.0f)};

    // cube

    Cube lightCube("../assets/container.png", "../shader/light_shader.vs", "../shader/light_shader.fs");
    Cube shadowCube("../assets/container.png", "../shader/shadow.vs", "../shader/shadow.fs");

    Cube lightFloor("../assets/grass.jpg", "../shader/light_shader.vs", "../shader/light_shader.fs");
    Cube shadowFloor("../assets/grass.jpg", "../shader/shadow.vs", "../shader/shadow.fs");

    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        // Time

        float currentFrame = glfwGetTime();
        camera.m_dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // process input

        camera.processKeyboard(window);

        // light srouce pov
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);

        glm::mat4 lightModel = glm::mat4(1.0f);
        glm::mat4 lightView = glm::lookAt(light.position, light.target, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0F, 10.0f, 1.0f, 20.0f);
        glm::mat4 lightSpace = lightProjection * lightView;

        shadowCube.m_view = lightView;
        shadowCube.m_projection = lightProjection;

        shadowFloor.m_view = lightView;
        shadowFloor.m_projection = lightProjection;

        shadowCube.m_shaderProgram.use();
        shadowCube.m_shaderProgram.setMat4("lightSpace", lightSpace);
        shadowFloor.m_shaderProgram.use();
        shadowFloor.m_shaderProgram.setMat4("lightSpace", lightSpace);

        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        shadowCube.draw();
        shadowFloor.draw(glm::vec3(10.0f, 5.0f, 10.0f), glm::vec3(0.0f, -1.0f, 0.0f), 0.0f);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // camera pov

        glViewport(0, 0, SRC_WIDTH, SRC_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightCube.m_view = camera.getViewMatrix();

        lightCube.m_shaderProgram.use();
        lightCube.m_shaderProgram.setVec3("light.color", light.color);
        lightCube.m_shaderProgram.setVec3("light.direction", glm::normalize(light.target - light.position));
        lightCube.m_shaderProgram.setVec3("viewPos", camera.m_cameraPos);

        lightFloor.m_view = camera.getViewMatrix();

        lightFloor.m_shaderProgram.use();
        lightFloor.m_shaderProgram.setVec3("light.color", light.color);
        lightFloor.m_shaderProgram.setVec3("light.direction", glm::normalize(light.target - light.position));
        lightFloor.m_shaderProgram.setVec3("viewPos", camera.m_cameraPos);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);

        lightCube.m_shaderProgram.setInt("shadowMap", 1);
        lightFloor.m_shaderProgram.setInt("shadowMap", 1);

        lightCube.draw();
        lightFloor.draw(glm::vec3(10.0f, 0.0f, 10.0f), glm::vec3(0.0f, -0.5f, 0.0f), 0.0f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>
#include <shader.h>

#include <camera.hpp>
#include <cube.hpp>

glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
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

    GLFWwindow *window = glfwCreateWindow(800, 600, "22_phong_lighting", nullptr, nullptr);
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

    Cube cube("../assets/container.png", "../shader/cube_shader.vs", "../shader/cube_shader.fs");

    unsigned int texture1;

    glGenTextures(1, &texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    int width, height, nbr;
    unsigned char *data = stbi_load("../assets/container2_specular.png", &width, &height, &nbr, 0);

    auto type = (nbr == 3) ? GL_RGB : GL_RGBA;

    if (data)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            type, width,
            height,
            0,
            type,
            GL_UNSIGNED_BYTE,
            data);

        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);

    cube.m_shaderProgram.use();

    cube.m_shaderProgram.setVec3("lightPos", lightPos);
    cube.m_shaderProgram.setVec3("lightColor", lightColor);

    cube.m_shaderProgram.setInt("texture1", 1);

    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        camera.m_dt = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.processKeyboard(window);

        cube.m_view = camera.getViewMatrix();

        cube.m_shaderProgram.setVec3("viewPos", camera.m_cameraPos);

        cube.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
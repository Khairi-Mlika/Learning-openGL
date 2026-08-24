#include <iostream>

#include <cmath>

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

const char *vertexCode = R"(
    #version 330

    layout(location = 0) in vec2 aPos;
    layout(location = 1) in vec3 aColor;

    uniform float offset;

    out vec3 Color;

    void main(){
        gl_Position = vec4(aPos.x + offset , aPos.y , 0.0 , 1.0);
        Color = aColor;
    }
)";

const char *fragmentCode = R"(
    #version 330

    in vec3 Color;
    out vec4 FragColor;

    void main(){
        FragColor = vec4(Color , 1.0);
    }
)";

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "05_animated_triangle", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "window creation error" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "glad init error" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, 800, 600);

    // clang-format off
    float vertexInfo[] = {
        // position     // color
         0.0f,  0.5f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,   0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,   0.0f, 0.0f, 1.0f,
    };
    // clang-format on
    unsigned int VAO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexInfo), vertexInfo, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(2 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexCode, nullptr);
    glShaderSource(fragmentShader, 1, &fragmentCode, nullptr);

    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    unsigned int offsetLocation = glGetUniformLocation(shaderProgram, "offset");

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        float offset = std::sin(glfwGetTime()) * 0.5;

        glUseProgram(shaderProgram);

        glUniform1f(offsetLocation, offset);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
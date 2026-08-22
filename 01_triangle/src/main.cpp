#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char *vertexCode = R"(
    #version 330

    layout (location = 0) in vec2 aPos;

    void main(){

        gl_Position = vec4(aPos , 0.0 , 1.0);

    }
)";

const char *fragmentCode = R"(
    #version 330

    out vec4 FragColor;

    void main(){

        FragColor = vec4(1.0 , 0.0 , 0.0 , 1.0);

    }
)";

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "01_triangle", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "window creation error" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "glad loading error" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }
    glViewport(0, 0, 800, 600);

    // clang-format off
    float vertices[] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f,
    };
    // clang-format on

    unsigned int VAO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertexShader, 1, &vertexCode, nullptr);
    glShaderSource(fragmentShader, 1, &fragmentCode, nullptr);

    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    unsigned int shader = glCreateProgram();

    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);

    glLinkProgram(shader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
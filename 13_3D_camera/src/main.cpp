#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <shader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtC/type_ptr.hpp>

#include <stb_image.h>

class Camera
{
public:
    glm::vec3 m_cameraPos{0.0f, 0.0f, 3.0f};

    glm::vec3 m_cameraFront{0.0f, 0.0f, -1.0f};
    glm::vec3 m_cameraUp{0.0f, 1.0f, 0.0f};
    glm::vec3 m_cameraRight{1.0f, 0.0f, 0.0f};

    float m_mouseSense{0.1f};

    float m_yaw{-90.0f};
    float m_pitch{0.0f};

    float m_speed{3.0f};

    float m_dt{0.1f};

    Camera()
    {
    }

    Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up, glm::vec3 right, float yaw, float pitch)
        : m_cameraPos(pos),
          m_cameraFront(front),
          m_cameraUp(up),
          m_cameraRight(right),
          m_yaw(yaw),
          m_pitch(pitch)
    {
    }

    glm::mat4 getViewMatrix()
    {
        return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
    }

    void updateCamera()
    {
        glm::vec3 front;

        front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        front.y = sin(glm::radians(m_pitch));
        front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

        m_cameraFront = glm::normalize(front);
        m_cameraRight = glm::normalize(glm::cross(m_cameraFront, m_cameraUp));
    }

    void moveForward()
    {
        m_cameraPos += m_cameraFront * m_speed * m_dt;
    }

    void moveBackward()
    {
        m_cameraPos -= m_cameraFront * m_speed * m_dt;
    }

    void moveRight()
    {
        m_cameraPos += m_cameraRight * m_speed * m_dt;
    }

    void moveLeft()
    {
        m_cameraPos -= m_cameraRight * m_speed * m_dt;
    }

    void processKeyboard(GLFWwindow *window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(window, true);
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            moveForward();
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            moveRight();
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            moveBackward();
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            moveLeft();
        }
        // this makes the camera fps like (binded to the ground)
        // change it later to match the terrain height
        m_cameraPos.y = 0;
    }

    void processMouse(float xoffset, float yoffset, GLboolean pitchLock = true)
    {
        xoffset *= m_mouseSense;
        yoffset *= m_mouseSense * 0.3;

        m_yaw += xoffset;
        m_pitch -= yoffset;

        if (pitchLock)
        {
            if (m_pitch > 89.0f)
            {
                m_pitch = 89.0f;
            }
            if (m_pitch < -89.0f)
            {
                m_pitch = -89.0f;
            }
        }

        updateCamera();
    }
};

class Cube
{
public:
    Shader m_shaderProgram;
    unsigned int m_VAO;

    Cube(const std::string &texture_path, const std::string &vertex_path, const std::string &fragment_path)
    {
        // clang-format off
        float vertexInfo[] = {
            // ============================================================
            // FRONT (+Z)
            // ============================================================
            // position              // UV
            -0.5f,  0.5f,  0.5f,     0.0f, 1.0f, // top-left
             0.5f,  0.5f,  0.5f,     1.0f, 1.0f, // top-right
            -0.5f, -0.5f,  0.5f,     0.0f, 0.0f, // bottom-left
             0.5f, -0.5f,  0.5f,     1.0f, 0.0f, // bottom-right

            // ============================================================
            // BACK (-Z)
            // ============================================================
            -0.5f,  0.5f, -0.5f,     1.0f, 1.0f, // top-left
             0.5f,  0.5f, -0.5f,     0.0f, 1.0f, // top-right
            -0.5f, -0.5f, -0.5f,     1.0f, 0.0f, // bottom-left
             0.5f, -0.5f, -0.5f,     0.0f, 0.0f, // bottom-right

            // ============================================================
            // TOP (+Y)
            // ============================================================
            -0.5f,  0.5f,  0.5f,     0.0f, 0.0f, // front-left
             0.5f,  0.5f,  0.5f,     1.0f, 0.0f, // front-right
            -0.5f,  0.5f, -0.5f,     0.0f, 1.0f, // back-left
             0.5f,  0.5f, -0.5f,     1.0f, 1.0f, // back-right

            // ============================================================
            // BOTTOM (-Y)
            // ============================================================
            -0.5f, -0.5f,  0.5f,     0.0f, 1.0f, // front-left
             0.5f, -0.5f,  0.5f,     1.0f, 1.0f, // front-right
            -0.5f, -0.5f, -0.5f,     0.0f, 0.0f, // back-left
             0.5f, -0.5f, -0.5f,     1.0f, 0.0f, // back-right

            // ============================================================
            // RIGHT (+X)
            // ============================================================
            0.5f,  0.5f,  0.5f,     0.0f, 1.0f, // top-front
            0.5f, -0.5f,  0.5f,     0.0f, 0.0f, // bottom-front
            0.5f,  0.5f, -0.5f,     1.0f, 1.0f, // top-back
            0.5f, -0.5f, -0.5f,     1.0f, 0.0f, // bottom-back

            // ============================================================
            // LEFT (-X)
            // ============================================================
            -0.5f,  0.5f, -0.5f,     0.0f, 1.0f, // top-back
            -0.5f, -0.5f, -0.5f,     0.0f, 0.0f, // bottom-back
            -0.5f,  0.5f,  0.5f,     1.0f, 1.0f, // top-front
            -0.5f, -0.5f,  0.5f,     1.0f, 0.0f  // bottom-front
        };

        unsigned int indices[] = {
            // FRONT
            0, 1, 3,
            3, 2, 0,

            // BACK
            4, 6, 7,
            7, 5, 4,

            // TOP
            8, 10, 11,
            11, 9, 8,

            // BOTTOM
            12, 13, 15,
            15, 14, 12,

            // RIGHT
            16, 17, 19,
            19, 18, 16,

            // LEFT
            20, 22, 23,
            23, 21, 20
        };

        //  clang-format on

        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        unsigned int VBO, EBO;

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER , sizeof(vertexInfo) , vertexInfo , GL_STATIC_DRAW);

        glGenBuffers(1 ,&EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER , sizeof(indices) , indices , GL_STATIC_DRAW);

        glVertexAttribPointer(0 , 3 , GL_FLOAT , GL_FALSE , 5 * sizeof(float) , (void*)0);
        glVertexAttribPointer(1 , 2 , GL_FLOAT , GL_FALSE , 5 * sizeof(float) , (void*)(3 * sizeof(float)));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        unsigned int texture0;
        
        glGenTextures(1 , &texture0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D , texture0);

        stbi_set_flip_vertically_on_load(true);

        int width,height,nbr;
        unsigned char* data = stbi_load(texture_path.data() , &width , &height , &nbr , 0);

        auto type = (nbr == 3) ? GL_RGB : GL_RGBA;

        if(data){
            glPixelStorei(GL_UNPACK_ALIGNMENT , 1);

            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                type,width,
                height,
                0,
                type,
                GL_UNSIGNED_BYTE ,
                data
            );

            glGenerateMipmap(GL_TEXTURE_2D);
        }
        stbi_image_free(data);

        m_shaderProgram = Shader(vertex_path.data() , fragment_path.data());
        m_shaderProgram.setInt("texture0" , 0);
    }

    ~Cube()
    {
    }

    // this is a static draw function
    void draw(Camera& camera ,glm::vec3 size = {1.0f , 1.0f , 1.0f} , glm::vec3 position = {0.0f , 0.0f , 0.0f} , float rotation = 0.0f)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model , position);
        model = glm::rotate(model , glm::radians(rotation) , glm::vec3(0.0f , 0.0f , 1.0f));
        model = glm::scale(model , size);
        
        glm::mat4 projection = glm::perspective(glm::radians(45.0f) , (float)(800.0/600.0) , 0.1f , 100.0f);

        glm::mat4 view = camera.getViewMatrix();

        m_shaderProgram.use();

        m_shaderProgram.setMat4("model" , model);
        m_shaderProgram.setMat4("projection" , projection);
        m_shaderProgram.setMat4("view" , view);

        glEnable(GL_DEPTH_TEST);
        

        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES , 36 , GL_UNSIGNED_INT , 0);
    }

    // this is the rotating cube function
    void drawRotate(Camera& camera, glm::vec3 size = {1.0f , 1.0f , 1.0f} , glm::vec3 position = {0.0f , 0.0f , 0.0f} , float rotation = 0.0f){
                glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model , position);
        model = glm::rotate(model , glm::radians(rotation) , glm::vec3(0.0f , 0.0f , 1.0f));
        model = glm::rotate(model , glm::radians((float)glfwGetTime())*20 , glm::vec3(0.0f , 1.0f , 0.0f));
        model = glm::scale(model , size);
        
        glm::mat4 projection = glm::perspective(glm::radians(45.0f) , (float)(800.0/600.0) , 1.0f , 100.0f);

        glm::mat4 view = camera.getViewMatrix();

        m_shaderProgram.use();

        m_shaderProgram.setMat4("model" , model);
        m_shaderProgram.setMat4("projection" , projection);
        m_shaderProgram.setMat4("view" , view);

        glEnable(GL_DEPTH_TEST);

        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES , 36 , GL_UNSIGNED_INT , 0);
    }
};

float lastX = 400.0f;
float lastY = 300.0f;
bool firstMouse = true;

Camera* cameraPtr = nullptr;

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

    cameraPtr->processMouse(xOffset, yOffset);
}

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "13_3D_camera", nullptr, nullptr);
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


    Cube cube("../assets/container.png", "../shader/shader.vs","../shader/shader.fs");

    Camera camera {};
    cameraPtr = &camera;

    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        float dt = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        cameraPtr->m_dt = dt;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        camera.processKeyboard(window);

        cube.draw(camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    delete(cameraPtr);

    return 0;
}
class Camera
{
public:
    glm::vec3 m_cameraPos{0.0f, 0.0f, 3.0f};

    glm::vec3 m_cameraFront{0.0f, 0.0f, -1.0f};
    glm::vec3 m_cameraUp{0.0f, 1.0f, 0.0f};
    glm::vec3 m_cameraRight{1.0f, 0.0f, 0.0f};

    float m_mouseSense{0.05f};

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
        yoffset *= m_mouseSense;

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
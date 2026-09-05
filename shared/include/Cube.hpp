class Cube
{
public:
    Shader m_shaderProgram;

    unsigned int m_VAO;
    unsigned int m_texture;

    glm::mat4 m_projection = glm::perspective(glm::radians(45.0f), (float)(800.0 / 600.0), 0.1f, 100.0f);
    glm::mat4 m_view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

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
            
        glGenTextures(1 , &m_texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D , m_texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_set_flip_vertically_on_load(true);

        int width,height,nbr;
        unsigned char* data = stbi_load(texture_path.data() , &width , &height , &nbr , 0);

        auto type = (nbr == 3) ? GL_RGB : GL_RGBA;

        if(data)
        {
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
    void draw(glm::vec3 size = {1.0f , 1.0f , 1.0f} , glm::vec3 position = {0.0f , 0.0f , 0.0f} , float rotation = 0.0f)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model , position);
        model = glm::rotate(model , glm::radians(rotation) , glm::vec3(0.0f , 1.0f , 0.0f));
        model = glm::scale(model , size);
            
        glm::mat4 projection = m_projection;
        glm::mat4 view = m_view;

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D , m_texture);

        m_shaderProgram.use();

        m_shaderProgram.setMat4("model" , model);
        m_shaderProgram.setMat4("projection" , projection);
        m_shaderProgram.setMat4("view" , view);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES , 36 , GL_UNSIGNED_INT , 0);
    }

    void drawRotate(glm::vec3 size = {1.0f , 1.0f , 1.0f} , glm::vec3 position = {0.0f , 0.0f , 0.0f} , float rotation = 0.0f)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model , position);
        model = glm::rotate(model , glm::radians(rotation) , glm::vec3(0.0f , 0.0f , 1.0f));
        model = glm::rotate(model , glm::radians((float)glfwGetTime())*20 , glm::vec3(0.0f , 1.0f , 0.0f));
        model = glm::scale(model , size);
        
        glm::mat4 projection = glm::perspective(glm::radians(45.0f) , (float)(800.0/600.0) , 1.0f , 100.0f);

        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),glm::vec3(0.0f, 0.0f, 0.0f),glm::vec3(0.0f, 1.0f, 0.0f));

        m_shaderProgram.use();

        m_shaderProgram.setMat4("model" , model);
        m_shaderProgram.setMat4("projection" , projection);
        m_shaderProgram.setMat4("view" , view);

        glEnable(GL_DEPTH_TEST);

        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES , 36 , GL_UNSIGNED_INT , 0);
    }
};

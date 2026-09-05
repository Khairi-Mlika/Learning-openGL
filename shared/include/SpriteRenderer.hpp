class SpriteRenderer
{
public:
    Shader m_shaderProgram;
    std::string m_texturePath;
    unsigned int m_VAO;

    SpriteRenderer(std::string texture_path, std::string vertexSh_path, std::string fragmentSh_path)
    {
        unsigned int VBO, EBO;

        // clang-format off

        float vertexInfo[] = {
            // position     // UV
            -0.5f,  0.5f,   0.0f, 1.0f,// top left
             0.5f,  0.5f,   1.0f, 1.0f,// top right
            -0.5f, -0.5f,   0.0f, 0.0f,// bottom left
             0.5f, -0.5f,   1.0f, 0.0f,// bottom right
        };

        unsigned int indices[] = {
            0, 1, 2,
            1, 2, 3,
        };

        // clang-format on

        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexInfo), vertexInfo, GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        unsigned int texture;

        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        stbi_set_flip_vertically_on_load(true);

        int width, height, nbr;
        unsigned char *data = stbi_load(texture_path.data(), &width, &height, &nbr, 0);

        auto type = (nbr == 3) ? GL_RGB : GL_RGBA;

        if (data)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                type,
                width,
                height,
                0,
                type,
                GL_UNSIGNED_BYTE,
                data);

            glGenerateMipmap(GL_TEXTURE_2D);
        }
        stbi_image_free(data);

        m_shaderProgram = Shader(vertexSh_path.data(), fragmentSh_path.data());

        m_shaderProgram.setInt("texture0", 0);
    }

    ~SpriteRenderer()
    {
    }

    void draw(glm::vec2 size, glm::vec2 position, float rotation)
    {
        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, glm::vec3(position, 0.0f));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(size, 1.0f));

        glm::mat4 projection = glm::ortho(
            -400.0f, 400.0f,
            -300.0F, 300.0f,
            -1.0f, 1.0f);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_shaderProgram.use();

        m_shaderProgram.setMat4("model", model);
        m_shaderProgram.setMat4("projection", projection);

        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
};
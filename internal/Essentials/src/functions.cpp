#define STB_IMAGE_IMPLEMENTATION
#include "camera.hpp"
#include "etime.hpp"

using namespace std;

#define ENDING 4
#define SPEED 5.0f

bool wireframe = false;
bool mouse_enabled = false;

std::ostream &operator<<(std::ostream &os, const glm::vec4 &vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
    return os;
}

std::ostream &operator<<(std::ostream &os, const glm::vec3 &vec) {
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}

glm::vec3 operator+(const glm::vec3 &base, const glm::vec4 &other) {
    glm::vec3 n = base;
    glm::vec4 no = other / other.w;
    n.x += no.x;
    n.y += no.y;
    n.z += no.z;
    return n;
}

glm::vec3 operator-(const glm::vec3 &base, const glm::vec4 &other) {
    glm::vec3 n = base;
    glm::vec4 no = other / other.w;
    n.x -= no.x;
    n.y -= no.y;
    n.z -= no.z;
    return n;
}

glm::vec3 operator*(const glm::mat4 &mat, const glm::vec3 &vec) {
    auto v4 = glm::vec4(vec, 1);
    auto mv = mat * v4;
    return mv / mv.w;
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
        cout << "process input..." << endl;
    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        // glfwSetWindowShouldClose(window, true);
        mouse_enabled = !mouse_enabled;
        glfwSetInputMode(window, GLFW_CURSOR, (!mouse_enabled) ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
        // cout << "escape was pressed mouse : " << mouse_enabled << endl;
    } else if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        wireframe = !wireframe;
        if (wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    } else if (key == GLFW_KEY_W && action == GLFW_REPEAT) {
        Camera::mainCamera.localTranslate(glm::vec3(0.0, 1.0, 0.0) * SPEED * ETime::deltaTime);
        // Camera::mainCamera.position += glm::vec3(0.0, 1.0, 0.0) * SPEED;
        // cout << Camera::mainCamera.position.x << ", " << Camera::mainCamera.position.y << ", "
        //      << Camera::mainCamera.position.z << endl;
        // Camera::mainCamera.recalculate();
    } else if (key == GLFW_KEY_S && action == GLFW_REPEAT) {
        Camera::mainCamera.localTranslate(glm::vec3(0.0, -1.0, 0.0) * SPEED * ETime::deltaTime);
        // Camera::mainCamera.position += glm::vec3(0.0, -1.0, 0.0) * SPEED;
        // Camera::mainCamera.recalculate();
    } else if (key == GLFW_KEY_D && action == GLFW_REPEAT) {
        Camera::mainCamera.localTranslate(glm::vec3(1.0, 0.0, 0.0) * SPEED * ETime::deltaTime);
        // Camera::mainCamera.position += glm::vec3(1.0, 0.0, 0.0) * SPEED;
        // Camera::mainCamera.recalculate();
    } else if (key == GLFW_KEY_A && action == GLFW_REPEAT) {
        Camera::mainCamera.localTranslate(glm::vec3(-1.0, 0.0, 0.0) * SPEED * ETime::deltaTime);
        // Camera::mainCamera.position += glm::vec3(-1.0, 0.0, 0.0) * SPEED;
        // Camera::mainCamera.recalculate();
    } else if (key == GLFW_KEY_E && action == GLFW_REPEAT) {
        Camera::mainCamera.rollRotate(SPEED * 90 * ETime::deltaTime);
    } else if (key == GLFW_KEY_UP && action == GLFW_REPEAT) {
        Camera::mainCamera.globalTranslate(glm::vec3(0.0, 1.0, 0.0) * SPEED * ETime::deltaTime);
        // Camera::mainCamera.position += glm::vec3(0.0, 1.0, 0.0) * SPEED;
        // cout << Camera::mainCamera.position.x << ", " << Camera::mainCamera.position.y << ", "
        //      << Camera::mainCamera.position.z << endl;
        // Camera::mainCamera.recalculate();
    } else if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT) {
        Camera::mainCamera.globalTranslate(glm::vec3(0.0, -1.0, 0.0) * SPEED * ETime::deltaTime);
        // Camera::mainCamera.position += glm::vec3(0.0, -1.0, 0.0) * SPEED;
        // Camera::mainCamera.recalculate();
    } else if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT) {
        Camera::mainCamera.globalTranslate(glm::vec3(1.0, 0.0, 0.0) * SPEED * ETime::deltaTime);
        // Camera::mainCamera.position += glm::vec3(1.0, 0.0, 0.0) * SPEED;
        // Camera::mainCamera.recalculate();
    } else if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT) {
        Camera::mainCamera.globalTranslate(glm::vec3(-1.0, 0.0, 0.0) * SPEED * ETime::deltaTime);
        // Camera::mainCamera.position += glm::vec3(-1.0, 0.0, 0.0) * SPEED;
        // Camera::mainCamera.recalculate();
    } else if (key == GLFW_KEY_Q && action == GLFW_REPEAT) {
        Camera::mainCamera.rollRotate(-SPEED * 90 * ETime::deltaTime);
    } else if (key == GLFW_KEY_Y && action == GLFW_REPEAT) {
        Camera::mainCamera.scale(glm::vec3(1.0, 1.0, 1.0) * (1 + SPEED * ETime::deltaTime));
    } else if (key == GLFW_KEY_H && action == GLFW_REPEAT) {
        Camera::mainCamera.scale(glm::vec3(1.0, 1.0, 1.0) * (1 - SPEED * ETime::deltaTime));
    } else if (key == GLFW_KEY_Z && action == GLFW_REPEAT) {
        Camera::mainCamera.localTranslate(glm::vec3(0.0, 0.0, 1.0) * SPEED * ETime::deltaTime);
    } else if (key == GLFW_KEY_X && action == GLFW_REPEAT) {
        Camera::mainCamera.localTranslate(glm::vec3(0.0, 0.0, -1.0) * SPEED * ETime::deltaTime);
        cout << Camera::mainCamera.getPos() << endl;
    } else if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        if (Camera::mainCamera.getProjectionMode() == Camera::Mode::orthographic) {
            Camera::mainCamera.setProjectionMode(Camera::Mode::perspective);
        } else {
            Camera::mainCamera.setProjectionMode(Camera::Mode::orthographic);
        }
    } else if (key == GLFW_KEY_F && action == GLFW_REPEAT) {
        Camera::mainCamera.lookAt(glm::vec3(0.0, 0.0, 0.0));
        cout << Camera::mainCamera.up();
        cout << "look at" << endl;
    }
}

bool initialized = false;
double prevX = -1, prevY = -1;
void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    if (initialized == false) {
        initialized = true;
        prevX = xpos;
        prevY = ypos;
        return;
    }

    double offsetX = prevX - xpos;
    double offsetY = prevY - ypos;

    prevX = xpos;
    prevY = ypos;
    if (abs(offsetX) < 0.1f) {
        return;
    }

    Camera::mainCamera.yawRotate(ETime::deltaTime * -offsetX);
    Camera::mainCamera.pitchRotate(ETime::deltaTime * -offsetY);
    // Camera::mainCamera.rollRotate(ETime::deltaTime, glm::vec3(-offsetY, -offsetX, 0.0));
    //  cout << offsetX << " : " << offsetY << endl;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

unsigned int load_shaders() {
    // create a simple vertex shader
    const char *vertexShaderSource = "#version 460 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "layout (location = 1) in vec3 aColor;\n"
                                     "out vec4 vertexColor;\n"
                                     "void main()\n"
                                     "{\n"
                                     "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                     "  vertexColor = vec4(aColor, 1.0);\n"
                                     "}\0";

    // define vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // bind code and compile
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // Fragment shader
    const char *fragmentShaderSource = "#version 460 core\n"
                                       "out vec4 FragColor;\n"
                                       "in vec4 vertexColor;\n"
                                       "uniform vec4 ourColor;\n"
                                       "void main() {\n"
                                       //    "    FragColor = ourColor;\n"
                                       "    FragColor = vertexColor;\n"
                                       "};\0";
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }

    // SHADER PROGRAM
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    }

    return shaderProgram;
}

unsigned int create_texture(const char *imagePath) {
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    // // // Border color
    // // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    // // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    // // float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
    // // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // // Setting interpolation options
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // // Mipmaps interpolation
    // // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
    if (data) {
        unsigned int len = strlen(imagePath);
        GLint format = GL_RGBA;
        if (strcmp(&imagePath[len - ENDING], ".jpg") == 0) {
            format = GL_RGB;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return texture;
}

unsigned int create_simplex() {
    float vertices[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f   // top left
    };
    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    // 2. copy our vertices array in a vertex buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 4. then set the vertex attributes pointers
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    // glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return VAO;
}

unsigned int create_cube() {
    unsigned int indices[36];
    unsigned int mapping[] = {0, 1, 2, 2, 3, 0};
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            indices[i * 6 + j] = 4 * i + mapping[j];
            // cout << i * 6 + j << " : " << mapping[j] << endl;
        }
    }

    // 0->0, 1->1, 2->2, 3->2, 4->3, 5->1

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f};

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    // Generate a vertex array object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // 1. bind Vertex Array Object
    glBindVertexArray(VAO);

    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // copy indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    return VAO;
}

unsigned int create_cube_bad() {

    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // 0
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  // 1
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // 2
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,   // 2
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // 3
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // 0

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    // 2. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    return VAO;
}

// #include "Node/transform.hpp"
// #include "property.hpp"

// Property<Engine::Node::Transform, glm::vec3> &operator+=(Property<Engine::Node::Transform, glm::vec3> &base, const glm::vec3 &other) {
//     base = (glm::vec3)base + other;
//     return base;
// }
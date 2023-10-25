#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// 错误回调函数
void errorCallback(int error, const char *description);
// 退出回调函数
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
// 编译顶点着色器
const char *vertexShaderSource = R"(
            #version 330 core
            layout (location = 0) in vec3 position;
            layout (location = 1) in vec3 color;
            out vec3 fragColor;
            void main() {
                gl_Position = vec4(position, 1.0);
                fragColor = color;
            }
        )";

// 编译片段着色器
const char *fragmentShaderSource = R"(
            #version 330 core
            in vec3 fragColor;
            out vec4 outColor;
            void main() {
                outColor = vec4(fragColor, 1.0);
            }
        )";
// 窗口尺寸
const GLuint width = 1920, height = 1080;
int main()
{
    ////////////////////////////////////////////////////////////固定模板
    ///////////////////////////////////////////////////////////////////
    if (!glfwInit())
    {
        return -1;
    }
    // 设置错误回调函数
    glfwSetErrorCallback(errorCallback);
    // 配置GLFW以使用OpenGL 3.3核心剖面
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow *window = glfwCreateWindow(width, height, "TestOpenGL", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // 设置退出回调函数
    glfwSetKeyCallback(window, key_callback);
    // 初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    ////////////////////////////////////////////////////////////固定模板
    ///////////////////////////////////////////////////////////////////
    // 设置顶点着色器
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    // 检查顶点着色器是否编译正常
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    // 设置片段着色器
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    // 检查片段着色器是否编译正常
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
    }
    // 创建着色器程序并链接
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // 检查着色器程序是否链接正常
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 设置顶点坐标和颜色
    GLfloat vertices[] = {
        0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f};
    GLuint indices[] = {
        // Note that we start from 0!
        0, 1, 2, // First Triangle
        0, 2, 3  // Second Triangle
    };
    // 创建顶点缓冲对象 (VBO)，顶点数组对象（VAO）,索引缓冲对象（EBO）
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // 绑定顶点数组VAO，然后绑定顶点缓冲VBO
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
    glEnableVertexAttribArray(0);
    // 设置颜色属性指针
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // 解除顶点数组VAO绑定
    glBindVertexArray(0);
    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // 事件检查
        glfwPollEvents();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // 设置视口参数
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        // 调用着色器
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // 渲染
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }
    // 解除链接资源（不再绘制）
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}

void errorCallback(int error, const char *description)
{
    std::cerr << "Error: " << description << std::endl;
}
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    // 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
    // 关闭应用程序
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// 错误回调函数
void errorCallback(int error, const char *description);
// 退出回调函数
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
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
    GLFWwindow *window = glfwCreateWindow(width, height, "", nullptr, nullptr);
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
        glfwSwapBuffers(window);
    }
    // 解除链接资源（不再绘制）
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
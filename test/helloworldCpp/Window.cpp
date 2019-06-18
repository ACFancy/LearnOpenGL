//
//  Window.cpp
//  helloworldCpp
//
//  Created by User on 2019/6/13.
//  Copyright © 2019 User. All rights reserved.
//

#include "Window.hpp"
#pragma mark - Public Methods
#pragma mark-- Design Methods
Window::Window(GLuint width, GLuint height, GLchar *title): m_width(width), m_height(height), m_title(title)
{
    m_window = createWindow();
    if (NULL == m_window || Ret_failure == initGlad()) {
        return;
    }
}

#pragma mark -- Destruct Methods
Window::~Window()
{
    clearWindow();
    m_window = NULL;
    m_title = NULL;
}

#pragma mark -- Render
void Window::render()
{
    while (!glfwWindowShouldClose(m_window)) {
        /** 处理用户输入 */
        processInput(m_window);
        
        // 渲染执行
        /** 设置清除缓存时窗体背景颜色 glClearColor函数是一个状态设置函数 */
        glClearColor(0.2, 0.3, 0.3, 1.0f);
        
        /** 开始清除 glClear函数则是一个状态使用的函数*/
        glClear(GL_COLOR_BUFFER_BIT);
        
        // 检查并调用事件，交互缓冲
        /** 交互缓存*/
        glfwSwapBuffers(m_window);
        
        /** 检查是否有事件, 更新窗体状态*/
        glfwPollEvents();
    }
    
    clearWindow();
}

#pragma mark - Private Methods
#pragma mark-- Create
GLFWwindow *Window::createWindow()
{
    /** 初始化 GLFW*/
    if (!glfwInit()) {
        std::cout << "GLFW init failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    /** 告诉 GLFW 使用的OpenGL 版本是3.3*/
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    /** 告诉 GLFW 使用的是 Core-OpenGL 模式*/
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /** apple兼容*/
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
    
    /** 创建GLFWwindow 实例*/
    GLFWwindow *window = glfwCreateWindow(m_width, m_height, m_title, NULL, NULL);
    if (NULL == window) {
        std::cout << "Create window failed" << std::endl;
        clearWindow();
        return NULL;
    }
    
    /** 通知 GLFW 将窗口的上下文设置为当前线程的上下文*/
    glfwMakeContextCurrent(window);
    // 渲染循环初始化之前注册这些回调函数
    /** 注册 GLFW 窗体大小的回调函数*/
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCB);
    /** 注册 GLFW 错误回调*/
    glfwSetErrorCallback(errorCallback);
    
    return window;
}

#pragma mark -- init CLAD
RetStatus Window::initGlad()
{
    /**
     GLAD 是用来管理 OpenGL 的函数指针的，所以在调用任何 OpenGL的函数之前需要先初始化
     GLAD。（GLFW给我们的是glfwGetProcAddress, 它根据我们编译的系统定义了正确的函数）
     */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "GLAD init Failed" << std::endl;
        clearWindow();
        return Ret_failure;
    }
    return Ret_Success;
}

#pragma mark -- Handle User Events
void Window::processInput(GLFWwindow *window)
{
    if (NULL == window) {
        std::cout << "window == NULL invalid" << std::endl;
        return;
    }
    /** 判断用户是否按下 esc 按键，或者是关闭窗体*/
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        /* 关闭窗体*/
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    } else if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_0)) {
        std::cout << "0 Pressed" << std::endl;
    }
}

#pragma mark -- Clear Window
void Window::clearWindow()
{
    glfwTerminate();
}

#pragma mark - callback
#pragma mark -- error
void Window::errorCallback(GLint errorCode, const GLchar *description)
{
    std::cout << "errorCode:" << errorCode << ", description:" << description << std::endl;
}

#pragma mark -- Frame
void Window::frameBufferSizeCB(GLFWwindow *window, GLint width, GLint height)
{
    if (NULL == window) {
        std::cout << "Window is NULL" << std::endl;
        return;
    }
    
    std::cout << "width:" << width << ",height:" << height << std::endl;
    /**
     告诉 OpenGL 渲染 GLFW 窗体的位置，大小（确保渲染大小和窗体大小一致，
     也可用比窗体小，在视网膜显示屏是甚至比窗体大）
     */
    glViewport(0, 0, width, height);
}

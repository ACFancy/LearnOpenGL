//
//  main.m
//  helloWindow
//
//  Created by User on 2019/5/31.
//  Copyright © 2019 User. All rights reserved.
//

#include <glad.h>
//#include <GL/glew.h>
#include <glfw3.h>
#include <iostream>

void Render(void)
{
    // 开始渲染
    // 设置帧缓存清除时窗体背景颜色，避免帧切换时还出现上一帧内容
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void errorCallback(GLint errorCode, const GLchar *description)
{
    std::cout << "errorCode:" << errorCode << ", descrpition:" << description << std::endl;
}

void frameBufferSizeCB(GLFWwindow *window, GLint width, GLint height)
{
    if (NULL == window) {
        
        return;
    }
    
    glViewport(0, 0, width, height);
}

int main(int argc, const char * argv[]) {
    if (!glfwInit()) {
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //如果使用的是Mac OS X系统，你还需要加下面这行代码到你的初始化代码中这些配置才能起作用
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow *window = glfwCreateWindow(800, 600, "Open New Window", NULL, NULL);
    if (NULL == window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
//    if(!glewInit())
//    {
//        return -1;
//    }
    // 指定OpenGL上下文
    glfwMakeContextCurrent(window);
    
    // 注册相关回调函数
    // 窗体大小改变回调
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCB);
    
    // 错误回调
    glfwSetErrorCallback(errorCallback);
    
    // 使用OpenGL 相关API之前，必须先初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    while(!glfwWindowShouldClose(window)){
        Render();
        glfwSwapBuffers(window);
        //在渲染循环中处理相关输入：键盘、鼠标
        glfwPollEvents();
    }
    glfwTerminate();
    exit(EXIT_SUCCESS);
    return 0;
}

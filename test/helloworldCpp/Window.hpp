//
//  Window.hpp
//  test
//
//  Created by User on 2019/6/13.
//  Copyright © 2019 User. All rights reserved.
//

#ifndef Window_h
#define Window_h

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef enum __retStatus {
    Ret_failure,
    Ret_Success
}RetStatus;

class Window
{
private:
    
    GLFWwindow *m_window;
    const GLuint m_width;
    const GLuint m_height;
    const GLchar *m_title;
    
    /**
     创建 GLFW 实例

     @return GLFW 实例
     */
    GLFWwindow *createWindow();
    
    /**
     初始化 GLAD

     @return RetStatus
     */
    RetStatus initGlad();
    
    /**
     处理用户输入回调

     @param window GLFW 窗体实例
     */
    void processInput(GLFWwindow *window);
    
    /**
     清理资源
     */
    void clearWindow();
    
    /**
     GLFW 错误回调

     @param errorCode 错误代码
     @param description 错误描述
     */
    static void errorCallback(GLint errorCode, const GLchar *description);
    
    /**
     GLFW 窗体大小回调

     @param window GLFW 窗体实例
     @param width 窗体宽度
     @param height 窗体高度
     */
    static void frameBufferSizeCB(GLFWwindow *window, GLint width, GLint height);
    
public:
    Window(GLuint width, GLuint height, GLchar *title);
    
    ~Window();
    
    /**
     渲染
     */
    void render();
};


#endif /* Window_h */

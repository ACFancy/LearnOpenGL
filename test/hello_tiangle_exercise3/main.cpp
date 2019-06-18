//
//  main.cpp
//  hello_tiangle_exercise3
//
//  Created by User on 2019/6/18.
//  Copyright © 2019 User. All rights reserved.
//

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
using namespace std;

static const GLint kSCRWidth = 800;
static const GLint kSCRHeight = 600;
static const GLchar *kWindowTitle = "hello_triangle_exercise3";
static const GLuint kErrorInfoLen = 512;

static const GLchar *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

static const GLchar *fragmentShaderSource1 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()"
"{"
" FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"}\0";

static const GLchar *fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()"
"{"
" FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\0";

void frameChangeSizeCallBack(GLFWwindow *, GLint, GLint);

void errorFunCallBack(GLint, const GLchar *);

void processInput(GLFWwindow *);

void render(GLFWwindow *, GLint *, GLuint *);

int main(int argc, const char * argv[]) {

    //glfw初始化
    if (!glfwInit()) {
        cout << "glfw init failed" << endl;
        exit(EXIT_SUCCESS);
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    //创建窗口
    GLFWwindow *window = glfwCreateWindow(kSCRWidth, kSCRHeight, kWindowTitle, NULL, NULL);
    if (NULL == window) {
        cout << "window create failed" << endl;
        glfwTerminate();
        return -1;
    }
    //设置窗口为当前上下文窗口
    glfwMakeContextCurrent(window);
    
    //设置窗口回调监听
    glfwSetFramebufferSizeCallback(window, frameChangeSizeCallBack);
    glfwSetErrorCallback(errorFunCallBack);
    
    //glad加载Open GL
    if (!gladLoadGL()) {
        cout << "glad load Open GL Failed" << endl;
        window = NULL;
        glfwTerminate();
        return -1;
    }
    
    //着色器创建，编译和检查是否编译成功
    GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    GLint success;
    GLchar errorInfo[kErrorInfoLen];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, kErrorInfoLen, NULL, errorInfo);
        cout << "vertex shader compile failed:" << errorInfo << endl;
    }
    
    GLint fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
    glCompileShader(fragmentShader1);
    glGetShaderiv(fragmentShader1, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader1, kErrorInfoLen, NULL, errorInfo);
        cout << "fragment shader 1 compile failed:" << errorInfo << endl;
    }
    
    GLint fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader2);
    glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader2, kErrorInfoLen, NULL, errorInfo);
        cout << "fragment shader 2 compile failed:" << errorInfo << endl;
    }
    
    //程序创建，连接和检查是否连接成功
    GLint shaderPrograme[2];
    shaderPrograme[0] = glCreateProgram();
    glAttachShader(shaderPrograme[0], vertexShader);
    glAttachShader(shaderPrograme[0], fragmentShader1);
    
    glLinkProgram(shaderPrograme[0]);
    glGetProgramiv(shaderPrograme[0], GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderPrograme[0], kErrorInfoLen, NULL, errorInfo);
        cout << "programe 0 link failed:" << errorInfo << endl;
    }
    
    shaderPrograme[1] = glCreateProgram();
    glAttachShader(shaderPrograme[1], vertexShader);
    glAttachShader(shaderPrograme[1], fragmentShader2);
    glLinkProgram(shaderPrograme[1]);
    glGetProgramiv(shaderPrograme[1], GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderPrograme[1], kErrorInfoLen, NULL, errorInfo);
        cout << "program 1 link failed:" << errorInfo << endl;
    }
    
    //删除着色器
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader1);
    glDeleteShader(fragmentShader2);
    
    //顶点数据设置
    GLfloat firstTriangle[] = {
        -0.9f, -0.5f, 0.0f,
        -0.45f, -0.5f, 0.0f,
        -0.6f, 0.5f, 0.0f
    };
    
    GLfloat secondTriangle[] = {
        0.9f, -0.5f, 0.0f,
        0.45f, -0.5f, 0.0f,
        0.6f, 0.5f, 0.0f
    };
    
    //顶点缓存和顶点属性赋值
    GLuint VAOs[2], VBOs[2];
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);
    
    glBindVertexArray(VAOs[0]);
    
    //顶点数组数据绑定和开启
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);
    
    //再次绑定顶点缓存和顶点数组，置空 便于之后解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    glBindVertexArray(VAOs[1]);
    //顶点数组数据绑定和开启
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);
    
    //再次绑定顶点缓存和顶点数组，置空 便于之后解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    //渲染
    render(window, shaderPrograme, VAOs);
    
    //顶点缓存和顶点数组销毁
    glDeleteBuffers(2, VBOs);
    glDeleteVertexArrays(2, VAOs);
    
    //销毁窗口退出
    window = NULL;
    glfwTerminate();
    
    return 0;
}


void frameChangeSizeCallBack(GLFWwindow *window, GLint width, GLint height)
{
    if (NULL == window) {
        return;
    }
    glViewport(0, 0, width, height);
}

void errorFunCallBack(GLint errorCode, const GLchar *errorInfo)
{
    cout << "ErrorCode: " << errorCode << ", ErrorInfo:" << errorInfo << endl;
}

void processInput(GLFWwindow *window)
{
    if (NULL == window) {
        return;
    }
    if (GLFW_PRESS == glfwGetKey(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void render(GLFWwindow *window, GLint *programs, GLuint *VAOs)
{
    if (NULL == window) {
        return;
    }
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        
        //clear Cache
        glClearColor(0.2f, 0.5f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //draw triangle
        glUseProgram(*(programs + 0));
        glBindVertexArray(*(VAOs + 0));
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glUseProgram(*(programs + 1));
        glBindVertexArray(*(VAOs + 1));
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        //swap window buffer and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

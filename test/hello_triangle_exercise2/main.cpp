//
//  main.cpp
//  hello_triangle_exercise2
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
static const GLchar *kWindowTitle = "hello_triangle_Exercise2";
static const GLuint kErrorInfoLen = 512;

static const GLchar *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

static const GLchar *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
" FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"}\0";

void frameChangeSizeCallBack(GLFWwindow *, GLint, GLint);

void errorfunCallBack(GLint, const GLchar *);

void processInput(GLFWwindow *window);

void render(GLFWwindow *window, GLint, GLuint *);

int main(int argc, const char * argv[]) {
    
    //初始化glfw
    if (!glfwInit()) {
        cout << "Init Failed" << endl;
        exit(EXIT_SUCCESS);
        return -1;
    }
    
    //OpenGL版本设置
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
        cout << "create window failed" << endl;
        glfwTerminate();
        return -1;
    }
    //将创建的窗口设置为当前上下文的窗口
    glfwMakeContextCurrent(window);
    //回调监听
    glfwSetFramebufferSizeCallback(window, frameChangeSizeCallBack);
    glfwSetErrorCallback(errorfunCallBack);
    //glad OpenGL 加载
    if (!gladLoadGL()) {
        cout << "Glad load Open GL Failed" << endl;
        window = NULL;
        glfwTerminate();
        return -1;
    }
    
    //shader 创建,编译和查询是否编译成功
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
    
    GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, kErrorInfoLen, NULL, errorInfo);
        cout << "fragment shader compile failed:" << errorInfo << endl;
    }
    
    //program 创建、连接和查询是否连接成功
    GLint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    //删除创建的shader
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    //顶点属性数据设置
    GLfloat firstTriangle[] = {
      -0.9, -0.5, 0.0,
      -0.45, -0.5, 0.0,
      -0.6, 0.5, 0.0
    };
    
    GLfloat secondTriangle[] = {
        0.9, -0.5, 0.0,
        0.45,-0.5, 0.0,
        0.6, 0.5, 0.0
    };
    
    //创建顶点buffer缓存
    GLuint VAO[2], VBO[2];
    
    //第一个三角形
    glGenVertexArrays(1, &VAO[0]);
    glGenBuffers(1, &VBO[0]);
    
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    
    //顶点属性赋值和开启顶点数组
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);
    
    //进行安全解绑的前置操作
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    //第二个三角形
    glGenVertexArrays(1, &VAO[1]);
    glGenBuffers(1, &VBO[1]);
    
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    
    //顶点属性赋值和开启顶点数组
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);
    
    //进行安全解绑的前置操作
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    //渲染
    render(window, shaderProgram, VAO);
    
    //销毁缓存
    glDeleteBuffers(1, &VBO[0]);
    glDeleteBuffers(1, &VBO[1]);
    glDeleteVertexArrays(1, &VAO[0]);
    glDeleteVertexArrays(1, &VAO[1]);
    
    //销毁窗口
    glfwTerminate();
    window = NULL;
    return 0;
}


void frameChangeSizeCallBack(GLFWwindow *window, GLint width, GLint height)
{
    if (NULL == window) {
        return;
    }
    glViewport(0, 0, width, height);
}

void errorfunCallBack(GLint errorCode, const GLchar *errorInfo)
{
    cout << "ErrorCode:" << errorCode << "ErrorInfo:" << errorInfo << endl;
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

void render(GLFWwindow *window, GLint program, GLuint *VAO)
{
    if (NULL == window) {
        return;
    }
    
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        
        //clear cache
        glClearColor(0.2f, 0.5f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //draw triangle
        glUseProgram(program);
        glBindVertexArray(*(VAO + 0));
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(*(VAO + 1));
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        //swap window buffer  and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

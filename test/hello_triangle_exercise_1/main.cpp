//
//  main.cpp
//  hello_triangle_exercise_1
//
//  Created by User on 2019/6/18.
//  Copyright © 2019 User. All rights reserved.
//

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

static const GLint SCR_WIDTH = 800;
static const GLint SCR_HEIGHT = 600;
static const GLchar *WINDOW_TITLE = "Triangle_Exercise1";
static const GLuint kErrorInfoLen = 512;

static const GLchar *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

static const GLchar *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{"
" FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);"
"}\0";

void framebuffersizeCallback(GLFWwindow*,int,int);

void errorfunCallBack(int,const char*);

void processInput(GLFWwindow *window);

void render(GLFWwindow *, GLint, GLint);

int main(int argc, const char * argv[]) {
    
    if (!glfwInit()) {
        glfwTerminate();
        exit(EXIT_SUCCESS);
        return -1;
    }
    
    //版本兼容
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    //创建窗口
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (NULL == window) {
        glfwTerminate();
        std::cout << "Create Window Failed" << std::endl;
        return -1;
    }
    
    //将创建的窗口设置为当前上下文的窗口
    glfwMakeContextCurrent(window);
    //设置相关回调监听
    glfwSetFramebufferSizeCallback(window, framebuffersizeCallback);
    glfwSetErrorCallback(errorfunCallBack);
    
    // glad 加载OpenGL相关库
    if (!gladLoadGL()) {
        std::cout << "Glad Load GL Failed" << std::endl;
        glfwTerminate();
        window = NULL;
        return -1;
    }
    
    //着色器编译相关
    GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    GLint success;
    GLchar errorInfo[kErrorInfoLen];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, kErrorInfoLen, NULL, errorInfo);
        std::cout << "vertex Shader Compile Failed:" << errorInfo << std::endl;
    }
    
    GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, kErrorInfoLen, NULL, errorInfo);
        std::cout << "fragment shader Compile Failed:" << errorInfo << std::endl;
    }
    
    //程序link相关
    GLint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, kErrorInfoLen, NULL, errorInfo);
        std::cout << "program link failed:" << errorInfo << std::endl;
    }
    
    //着色器销毁相关
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    //顶点缓冲相关
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    //先绑定VAO
    glBindVertexArray(VAO);
    
    //绑定到缓存，顶点属性赋值相关
    GLfloat vertexs[] = {
      -1.0f,  -0.5f, 0.0f,
      -0.5f,   0.5f, 0.0f,
          0,  -0.5f, 0.0f,
       1.0f,  -0.5f, 0.0f,
       0.5f,   0.5f, 0.0f
    };
    
    GLuint indices[] = {
        0, 1, 2,
        2, 3, 4
    };
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    //顶点属性赋值起始位置索引，大小，数据类型，是否转换为标准坐标，偏移量，顶点属性起始位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
    //开启顶点数组从0开始
    glEnableVertexAttribArray(0);
    
    //绑定注册的VBO、EBO和VAO,之后可以安全的解绑
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    //渲染
    render(window, shaderProgram, VAO);
    
    //顶点缓存销毁相关
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    //销毁
    glfwTerminate();
    window = NULL;
    return 0;
}


void framebuffersizeCallback(GLFWwindow *window, int width, int height)
{
    if (NULL == window) {
        return;
    }
    glViewport(0, 0, width, height);
}

void errorfunCallBack(int errorCode,const char *errorInfo)
{
    std::cout << "ErrorCode: " << errorCode << ", ErrorInfo:" << errorInfo << std::endl;
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

void render(GLFWwindow *window, GLint program, GLint VAO)
{
    if (NULL == window) {
        return;
    }
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        
        //清除缓存
        glClearColor(0.2f, 0.5f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //使用程序
        glUseProgram(program);
        //绑定
        glBindVertexArray(VAO);
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawArrays(GL_TRIANGLES, 2, 3);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

//
//  main.cpp
//  hello_triangle_1
//
//  Created by User on 2019/6/18.
//  Copyright © 2019 User. All rights reserved.
//

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const GLint SCR_WIDTH = 800;
const GLint SCR_HEIGHT = 600;
static const char *GL_TITLE = "Triangle";
static const GLuint errorInfoLen = 512;

void framebuffersizefunCallBack(GLFWwindow*,int,int);

void errorfunCallBack(int,const char*);

void render(GLFWwindow *, GLuint, GLuint);

void processInput(GLFWwindow *);

const GLchar *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
"}\0";
const GLchar *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"}\0";

int main(int argc, const char * argv[]) {
    
    if (!glfwInit()) {
        glfwTerminate();
        exit(EXIT_SUCCESS);
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, GL_TITLE, NULL, NULL);
    if (NULL == window) {
        std::cout << "Create Window failed" << std::endl;
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGL()) {
        glfwTerminate();
        return -1;
    }
    
    glfwSetFramebufferSizeCallback(window, framebuffersizefunCallBack);
    glfwSetErrorCallback(errorfunCallBack);
    
    if (!gladLoadGL()) {
        glfwTerminate();
        window = NULL;
        std::cout << "glad Load GL Failed" << std::endl;
        return -1;
    }
    
    std::cout << "Support OpenGL version:" << (GLchar *)glGetString(GL_VERSION) << std::endl;
    
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f,  -0.5f, 0.0f,
        0.0f,   0.5f, 0.0f,
    };
    
    //buffer
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //bind the vertex Array Object first, then bind and set vertex buffers, and the configure vertex attributes
    glBindVertexArray(VAO);
    //bind buffer to VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    //Shader
    //Vertex Shader
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    GLchar errorInfo[errorInfoLen];
    if (!success) {
        glGetShaderInfoLog(vertexShader, errorInfoLen, NULL, errorInfo);
        std::cout << "Vertex Shader Compile Failed:" << errorInfo << std::endl;
    }
    
    //Fragment Shader
    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, errorInfoLen, NULL, errorInfo);
        std::cout << "Fragment Shader Compile Failed:" << errorInfo << std::endl;
    }
    
    //Program 着色器程序
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    //装配着色器到Program上
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    //将编译后边的着色器的目标文件进行程序链接
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, errorInfoLen, NULL, errorInfo);
        std::cout << "Program Link Failed:" << errorInfo << std::endl;
    }
    //删除掉用完的着色器
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    //链接顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);
    
    //渲染
    render(window, shaderProgram, VAO);
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    
    glfwTerminate();
    window = NULL;
    return 0;
    
    return 0;
}

void framebuffersizefunCallBack(GLFWwindow *window,GLint width, GLint height)
{
    if (NULL == window) {
        return;
    }
    glViewport(0, 0, width, height);
}

void errorfunCallBack(GLint errorCode,const GLchar *errorInfo)
{
    std::cout << "errorCode:" << errorCode << ", errorInfo:" << errorInfo << std::endl;
}

void render(GLFWwindow *window, GLuint shaderProgram, GLuint VAO)
{
    if (NULL == window) {
        return;
    }
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        //Clear last frame buffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        //Draw Triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        //swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
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

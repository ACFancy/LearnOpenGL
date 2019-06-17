//
//  main.m
//  hello_triangle
//
//  Created by User on 2019/6/14.
//  Copyright © 2019 User. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
using namespace std;

const GLuint SCR_WIDTH = 800;
const GLuint SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

//const char *vertexShaderSource = ""
//"attribute vec3 aPos;\n"
//"void main()\n"
//"{\n"
//"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//"}\0";
//const char *fragmentShaderSource = ""
//"vec4 FragColor;\n"
//"void main()\n"
//"{\n"
//"   FragColor = vec4(1.0, 0.5, 0.2, 1.0);\n"
//"}\n\0";


void frameBuffersizeCallBack(GLFWwindow *window, GLint width, GLint height)
{
    if (NULL == window) {
        return;
    }
    glViewport(0, 0, width, height);
}

void errorCallBack(GLint errorCode, const GLchar *errorInfo)
{
    cout << "errorCode:" << errorCode << ", errorInfo" << errorInfo << endl;
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

void render(GLFWwindow *window, int shaderProgram, unsigned int VAO)
{
    if (NULL == window) {
        return;
    }
    
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        
        glClearColor(0.2, 0.3, 0.1, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    
        //draw our first triangle
        glUseProgram(shaderProgram);
        // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
//        glBindVertexArray(0); // no need to unbind it every time
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int main(int argc, const char * argv[]) {
    @autoreleasepool {
    
        if (glfwInit() != 1) {
            return -1;
        }
        glfwInitHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwInitHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        
        GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Triangle", NULL, NULL);
        if (NULL == window) {
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, frameBuffersizeCallBack);
        glfwSetErrorCallback(errorCallBack);
        
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            window = NULL;
            glfwTerminate();
            return -1;
        }
        
        printf("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));
        printf("Supported OpenGL version is %s.\n", (char *)glGetString(GL_VERSION));
        
        // build and compile our shader program
        // vertex shader
        int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        // check for shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            cout << "vertex shader infoLog:" << infoLog << endl;
        }

        //framgment shader
        int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        // check for shader compile errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            cout << "fragment shader infoLog:" << infoLog << endl;
        }

        // link shaders
        int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        // check for linking errors
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            cout << "program infolog:" << infoLog << endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        //set up vertex data (and buffers) and configure vertex attributes
        float vertices[] = {
            0.5f,   0.5f, 0.0f, //top right
            0.5f,  -0.5f, 0.0f, // bottom right
           -0.5f,  -0.5f, 0.0f, /// bottom left
           -0.5f,   0.5f, 0.0f, //top left
        };
        unsigned int indices[] = {
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
        };

        GLuint VBO, VAO, EBO;
        glGenVertexArrays(1, &VAO);

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        //bind the Vertex Array Object first, then bind and set vertex buffer(s), and the configure attribute(s)
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        //note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        //remeber: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // you can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs(or VBOs) when it's not directly necessary.
        glBindVertexArray(0);
        
        //uncomment this call to draw in wireframe polygons
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        
        render(window, shaderProgram, VAO);
        glfwTerminate();
        window = NULL;
    }
    return 0;
}

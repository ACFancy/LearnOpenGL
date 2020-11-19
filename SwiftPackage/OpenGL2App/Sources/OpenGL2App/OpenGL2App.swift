import Foundation
import Cglfw
import Cglew
import OpenGL.GL3

// Shader source code
fileprivate var vShaderCode = """
#version 330 core\n
layout (location = 0) in vec3 vertex;\n
void main() {\n
    gl_Position = vec4(vertex, 1.0);\n
}\0
"""

fileprivate var fShaderCode = """
#version 330 core\n
out lowp vec4 FragColor;\n
void main() {\n
    FragColor = vec4(1.0, 0.1, 0.0, 1.0);\n
}\0
"""

public class OpenGL2App {
    // MARK: - Properties
    private var VBO: UInt32 = 0
    private var VAO: UInt32 = 0
    private var shader: Shader!
    private var window: OpaquePointer?

    public init(title: String, width: Int32, height: Int32) {
        if glfwInit() == 0 {
            glfwTerminate()
            exit(EXIT_FAILURE)
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3)
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE)
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE)
        let data = (title as NSString).utf8String
        window = glfwCreateWindow(width, height, data!, nil, nil)
        if window == nil {
            glfwTerminate()
            exit(EXIT_FAILURE)
        }
        glfwMakeContextCurrent(window)
        glfwSwapInterval(1)
        setup()
    }

    public func run() {
        while (glfwWindowShouldClose(window!) == 0) {
            // Clear the colorbuffer
            glClearColor(0.2, 0.3, 0.3, 1.0)
            glClear(GLbitfield(GL_COLOR_BUFFER_BIT))
            shader.use()
            glBindVertexArray(VAO)
            glDrawArrays(GLenum(GL_TRIANGLES), 0, 3)
            glBindVertexArray(0)
            glfwSwapBuffers(window!)
            glfwPollEvents()
        }
        glfwDestroyWindow(window!)
        glfwTerminate()
        window = nil
        exit(EXIT_SUCCESS)
    }

    func setup() {
       var vertices: [Float] = [
           -0.5, -0.5, 0.0, // left
           0.5, -0.5, 0.0, // right
           0.0,  0.5, 0.0  // top
       ]

       shader = Shader(vShaderCode: vShaderCode, fShaderCode: fShaderCode)
       shader.compileShader()

       glGenVertexArrays(1, &VAO)
       glBindVertexArray(VAO)

       glGenBuffers(1, &VBO)
       glBindBuffer(GLenum(GL_ARRAY_BUFFER), VBO)
       glBufferData(GLenum(GL_ARRAY_BUFFER), vertices.count * MemoryLayout<Float>.size, &vertices, GLenum(GL_STATIC_DRAW))
       glEnableVertexAttribArray(0)

       glVertexAttribPointer(0, 3, GLenum(GL_FLOAT), GLboolean(GL_FALSE), GLsizei(3 * MemoryLayout<Float>.size), nil)
       glBindVertexArray(0)
       glBindBuffer(GLenum(GL_ARRAY_BUFFER), 0)
     }
}

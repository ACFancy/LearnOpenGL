//
//  File.swift
//  
//
//  Created by Lee Danatech on 2020/11/19.
//

import Foundation
import Cglfw
import Cglew

public final class Shader {
    // MARK: - Properties
    let vShaderCode: String
    let fShaderCode: String

    let program: UInt32

    public init(vShaderCode: String, fShaderCode: String) {
        self.vShaderCode = vShaderCode
        self.fShaderCode = fShaderCode
        program = glCreateProgram()
    }

    deinit {
        glDeleteProgram(program)
    }

    public func use() {
        glUseProgram(program)
    }

    public func compileShader() {
        let vertexShader = compileShader(sourceCode: vShaderCode, type: GL_VERTEX_SHADER)
        let fragmentShader = compileShader(sourceCode: fShaderCode, type: GL_FRAGMENT_SHADER)
        glAttachShader(program, vertexShader)
        glAttachShader(program, fragmentShader)
        glLinkProgram(program)

        glDeleteShader(vertexShader)
        glDeleteShader(fragmentShader)
    }

    public func compileShader(sourceCode: String, type: Int32) -> UInt32 {
        let shader = glCreateShader(GLenum(type))
        var data = sourceCode.toUnsafePointer()
        let codePtr = withUnsafePointer(to: &data) { (point) -> UnsafePointer<UnsafePointer<Int8>?> in
            return point
        }
        glShaderSource(shader, 1, codePtr, nil)
        glCompileShader(shader)
        data?.deallocate()
        var success: GLint = 0
        glGetShaderiv(shader, GLenum(GL_COMPILE_STATUS), &success)
        if success == 0 {
            let info = UnsafeMutablePointer<Int8>.allocate(capacity: 255)
            glGetShaderInfoLog(shader, 255, nil, info)
            let shaderType = type == GL_VERTEX_SHADER ? "Vertex" : "Fragment"
            debugPrint("Error: Compile \(shaderType) shader error", String(cString: info))
            info.deallocate()
        }
        return shader
    }
}

extension String {
    func toUnsafePointer() -> UnsafePointer<Int8>? {
        guard let data = data(using: .utf8) else {
            return nil
        }

        let buffer = UnsafeMutablePointer<UInt8>.allocate(capacity: data.count)
        let stream = OutputStream(toBuffer: buffer, capacity: data.count)
        stream.open()
        let value = data.withUnsafeBytes {
            $0.baseAddress?.assumingMemoryBound(to: UInt8.self)
        }
        guard let val = value else {
            return nil
        }
        stream.write(val, maxLength: data.count)
        stream.close()
        return UnsafeRawPointer(buffer).bindMemory(to: Int8.self, capacity: data.count)
    }
}

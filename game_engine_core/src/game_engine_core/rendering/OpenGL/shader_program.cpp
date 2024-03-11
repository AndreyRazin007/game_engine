#include "game_engine_core/rendering/OpenGL/shader_program.hpp"

#include "game_engine_core/log.hpp"

#include "glad/glad.h"
#include "glm/gtc/type_ptr.hpp"

namespace game_engine {
    bool create_shader(const char *source, const GLenum shaderType, GLuint &shaderId) {
        shaderId = glCreateShader(shaderType);
        glShaderSource(shaderId, 1, &source, nullptr);
        glCompileShader(shaderId);

        GLint success;
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE) {
            char infoLog[1024];
            glGetShaderInfoLog(shaderId, 1024, nullptr, infoLog);

            LOG_CRITICAL("Shader compilation error:\n{}", infoLog);

            return false;
        }

        return true;
    }

    ShaderProgram::ShaderProgram(const char *vertexShaderSrc, const char *fragmentShaderSrc) {
        GLuint vertexShaderId = 0;
        if (!create_shader(vertexShaderSrc, GL_VERTEX_SHADER, vertexShaderId)) {
            LOG_CRITICAL("VERTEX SHADER: compile-time error!");
            glDeleteShader(vertexShaderId);

            return;
        }

        GLuint fragmentShaderId = 0;
        if (!create_shader(fragmentShaderSrc, GL_FRAGMENT_SHADER, fragmentShaderId)) {
            LOG_CRITICAL("FRAGMENT SHADER: compile-time error!");
            glDeleteShader(vertexShaderId);
            glDeleteShader(fragmentShaderId);

            return;
        }

        m_id = glCreateProgram();
        glAttachShader(m_id, vertexShaderId);
        glAttachShader(m_id, fragmentShaderId);
        glLinkProgram(m_id);

        GLint success;
        glGetProgramiv(m_id, GL_LINK_STATUS, &success);
        if (success == GL_FALSE) {
            GLchar info_log[1024];
            glGetProgramInfoLog(m_id, 1024, nullptr, info_log);
            LOG_CRITICAL("SHADER PROGRAM: Link-time error:\n{0}", info_log);
            glDeleteProgram(m_id);
            m_id = 0;
            glDeleteShader(vertexShaderId);
            glDeleteShader(fragmentShaderId);

            return;
        } else {
            m_isCompiled = true;
        }

        glDetachShader(m_id, vertexShaderId);
        glDetachShader(m_id, fragmentShaderId);
        glDeleteShader(vertexShaderId);
        glDeleteShader(fragmentShaderId);
    }

    ShaderProgram::~ShaderProgram() {
        glDeleteProgram(m_id);
    }

    void ShaderProgram::bind() const {
        glUseProgram(m_id);
    }

    void ShaderProgram::unbind() {
        glUseProgram(0);
    }

    ShaderProgram &ShaderProgram::operator=(ShaderProgram &&shaderProgram) {
        glDeleteProgram(m_id);
        m_id = shaderProgram.m_id;
        m_isCompiled = shaderProgram.m_isCompiled;

        shaderProgram.m_id = 0;
        shaderProgram.m_isCompiled = false;

        return *this;
    }

    ShaderProgram::ShaderProgram(ShaderProgram &&shaderProgram) {
        m_id = shaderProgram.m_id;
        m_isCompiled = shaderProgram.m_isCompiled;

        shaderProgram.m_id = 0;
        shaderProgram.m_isCompiled = false;
    }

    void ShaderProgram::setMatrix_4(const char *name, const glm::mat4 &matrix) const {
        glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void ShaderProgram::setInt(const char *name, const int value) const {
        glUniform1i(glGetUniformLocation(m_id, name), value);
    }
}

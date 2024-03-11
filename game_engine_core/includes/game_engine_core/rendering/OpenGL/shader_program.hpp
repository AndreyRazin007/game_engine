#pragma once

#include "glm/mat4x4.hpp"

namespace game_engine {
    class ShaderProgram {
    public:
        ShaderProgram(const char *vertexShaderSrc, const char *fragmentShaderSrc);
        ShaderProgram(ShaderProgram&&);
        ShaderProgram &operator=(ShaderProgram&&);
        ~ShaderProgram();

        ShaderProgram() = delete;
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram &operator=(const ShaderProgram&) = delete;

        void bind() const;
        static void unbind();

        bool isCompiled() const { return m_isCompiled; }
        void setMatrix_4(const char *name, const glm::mat4 &matrix) const;
        void setInt(const char *name, const int value) const;

    private:
        bool m_isCompiled = false;
        unsigned int m_id = 0;
    };
}

#include "game_engine_core/rendering/OpenGL/vertex_buffer.hpp"
#include "game_engine_core/log.hpp"

#include "glad/glad.h"

namespace game_engine {
    constexpr unsigned int shaderDataTypeToComponentsCount(const ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float:
            case ShaderDataType::Int:
                return 1;

            case ShaderDataType::Float2:
            case ShaderDataType::Int2:
                return 2;

            case ShaderDataType::Float3:
            case ShaderDataType::Int3:
                return 3;

            case ShaderDataType::Float4:
            case ShaderDataType::Int4:
                return 4;
        }

        LOG_ERROR("shaderDataTypeToComponentType: unknown ShaderDataType!");

        return 0;
    }

    constexpr size_t shaderDataTypeSize(const ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
                return sizeof(GLfloat) * shaderDataTypeToComponentsCount(type);

            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
                return sizeof(GLint) * shaderDataTypeToComponentsCount(type);
        }

        LOG_ERROR("shaderDataTypeSize: unknown ShaderDataType!");

        return 0;
    }

    constexpr unsigned int shaderDataTypeToComponentType(const ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
                return GL_FLOAT;

            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
                return GL_INT;
        }

        LOG_ERROR("shaderDataTypeToComponentType: unknown ShaderDataType!");

        return GL_FLOAT;
    }

    constexpr GLenum usageToGLenum(const VertexBuffer::TypeDrawUsage usage) {
        switch (usage) {
            case VertexBuffer::TypeDrawUsage::Static: return GL_STATIC_DRAW;
            case VertexBuffer::TypeDrawUsage::Dynamic: return GL_DYNAMIC_DRAW;
            case VertexBuffer::TypeDrawUsage::Stream: return GL_STREAM_DRAW;
        }

        LOG_ERROR("Unknown VertexBuffer usage");

        return GL_STREAM_DRAW;
    }

    BufferElement::BufferElement(const ShaderDataType type)
        : m_type{type}, m_componentType{shaderDataTypeToComponentType(type)},
          m_componentsCount{shaderDataTypeToComponentsCount(type)},
          m_size{shaderDataTypeSize(type)},
          m_offset{0} {}

    VertexBuffer::VertexBuffer(const void *data, const size_t size,
                                BufferLayout bufferLayout, const TypeDrawUsage usage)
        : m_bufferLayout{std::move(bufferLayout)} {
        glGenBuffers(1, &m_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, size, data, usageToGLenum(usage));
    }

    VertexBuffer::~VertexBuffer() {
        glDeleteBuffers(1, &m_id);
    }

    VertexBuffer &VertexBuffer::operator=(VertexBuffer &&vertexBuffer) noexcept {
        m_id = vertexBuffer.m_id;
        vertexBuffer.m_id = 0;

        return *this;
    }

    VertexBuffer::VertexBuffer(VertexBuffer &&vertexBuffer) noexcept
        : m_id{vertexBuffer.m_id},
          m_bufferLayout{std::move(vertexBuffer.m_bufferLayout)} {
        vertexBuffer.m_id = 0;
    }

    void VertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void VertexBuffer::unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

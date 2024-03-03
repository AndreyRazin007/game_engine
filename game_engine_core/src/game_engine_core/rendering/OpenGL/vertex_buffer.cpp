#include "game_engine_core/rendering/OpenGL/vertex_buffer.hpp"
#include "game_engine_core/log.hpp"

#include "glad/glad.h"

namespace game_engine {
    constexpr GLenum usageToGLenum(const VertexBuffer::TypeDrawUsage usage) {
        switch (usage) {
            case VertexBuffer::TypeDrawUsage::Static: return GL_STATIC_DRAW;
            case VertexBuffer::TypeDrawUsage::Dynamic: return GL_DYNAMIC_DRAW;
            case VertexBuffer::TypeDrawUsage::Stream: return GL_STREAM_DRAW;
        }

        LOG_ERROR("Unknown VertexBuffer usage");

        return GL_STREAM_DRAW;
    }

    VertexBuffer::VertexBuffer(const void *data, const size_t size, const TypeDrawUsage usage) {
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

    VertexBuffer::VertexBuffer(VertexBuffer &&vertexBuffer) noexcept {
        m_id = vertexBuffer.m_id;
        vertexBuffer.m_id = 0;
    }

    void VertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void VertexBuffer::unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

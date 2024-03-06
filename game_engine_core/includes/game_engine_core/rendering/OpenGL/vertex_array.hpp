#pragma once

#include "game_engine_core/rendering/OpenGL/vertex_buffer.hpp"
#include "game_engine_core/rendering/OpenGL/index_buffer.hpp"

namespace game_engine {
    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray &operator=(const VertexArray&) = delete;

        VertexArray &operator=(VertexArray&&) noexcept;
        VertexArray(VertexArray&&) noexcept;

        void addVertexBuffer(const VertexBuffer &vertexBuffer);
        void setIndexBuffer(const IndexBuffer &indexBuffer);
        void bind() const;
        static void unbind();
        size_t getIndicesCount() const { return m_indicesCount; }

    private:
        unsigned int m_id = 0;
        unsigned int m_elementsCount = 0;
        size_t m_indicesCount = 0;
    };
}

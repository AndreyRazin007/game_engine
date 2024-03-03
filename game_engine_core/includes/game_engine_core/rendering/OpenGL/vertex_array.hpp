#pragma once

#include "game_engine_core/rendering/OpenGL/vertex_buffer.hpp"

namespace game_engine {
    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&) = delete;
        VertexArray &operator=(const VertexArray&) = delete;

        VertexArray &operator=(VertexArray&&) noexcept;
        VertexArray(VertexArray&&) noexcept;

        void addBuffer(const VertexBuffer &vertexBuffer);
        void bind() const;
        static void unbind();

    private:
        unsigned int m_id = 0;
        unsigned int m_elementsCount = 0;
    };
}

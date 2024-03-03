#pragma once

#include <iostream>

namespace game_engine {
    class VertexBuffer {
    public:
        enum class TypeDrawUsage {
            Static,
            Dynamic,
            Stream
        };

        VertexBuffer(const void *data, const size_t size,
                     const TypeDrawUsage usage = VertexBuffer::TypeDrawUsage::Static);
        ~VertexBuffer();

        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer &operator=(const VertexBuffer&) = delete;

        VertexBuffer &operator=(VertexBuffer &&vertexBuffer) noexcept;
        VertexBuffer(VertexBuffer &&vertexBuffer) noexcept;

        void bind() const;
        static void unbind();

    private:
        unsigned int m_id = 0;
    };
}

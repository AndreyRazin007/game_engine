#pragma once

#include <vector>

namespace game_engine {
    enum class ShaderDataType {
        Float,
        Float2,
        Float3,
        Float4,
        Int,
        Int2,
        Int3,
        Int4,
    };

    struct BufferElement {
        ShaderDataType m_type;
        uint32_t m_componentType;
        size_t m_componentsCount;
        size_t m_size;
        size_t m_offset;

        BufferElement(const ShaderDataType type);
    };

    class BufferLayout {
    public:
        BufferLayout(std::initializer_list<BufferElement> elements)
            : m_elements{std::move(elements)} {
            size_t offset = 0;
            m_stride = 0;

            for (auto &element : m_elements) {
                element.m_offset = offset;
                offset += element.m_size;
                m_stride += element.m_size;
            }
        }

        const std::vector<BufferElement> &getElements() const { return m_elements; }
        size_t getStride() const { return m_stride; }

    private:
        std::vector<BufferElement> m_elements;
        size_t m_stride = 0;
    };

    class VertexBuffer {
    public:
        enum class TypeDrawUsage {
            Static,
            Dynamic,
            Stream
        };

        VertexBuffer(const void *data, const size_t size, BufferLayout bufferLayout,
                     const TypeDrawUsage usage = VertexBuffer::TypeDrawUsage::Static);
        ~VertexBuffer();

        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer &operator=(const VertexBuffer&) = delete;

        VertexBuffer &operator=(VertexBuffer &&vertexBuffer) noexcept;
        VertexBuffer(VertexBuffer &&vertexBuffer) noexcept;

        void bind() const;
        static void unbind();

        const BufferLayout &getLayout() const { return m_bufferLayout; }

    private:
        unsigned int m_id = 0;
        BufferLayout m_bufferLayout;
    };
}

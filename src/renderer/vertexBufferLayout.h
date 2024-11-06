#ifndef VERTEXBUFFERLAYOUT_H
#define VERTEXBUFFERLAYOUT_H
#include <vector>
#include <glad/glad.h>
#include <stdexcept>

#include "renderer.h"


struct VertexBufferLayoutElement {
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type) {
        switch (type) {
            case GL_FLOAT: return 4;
                break;
            case GL_UNSIGNED_INT: return 4;
                break;
            case GL_UNSIGNED_BYTE: return 1;
                break;
            default: return 0;
        }
        ASSERT(false);
    }
};

class VertexBufferLayout {
public:
    VertexBufferLayout() : m_stride(0) {
    }

    template<typename T>
    void Push(unsigned int count) {
        throw std::runtime_error("Push not implemented for this type");
    }


    [[nodiscard]] inline const std::vector<VertexBufferLayoutElement> &GetElements() const {
        return m_elements;
    }

    [[nodiscard]] inline unsigned int GetStride() const { return m_stride; }

private:
    std::vector<VertexBufferLayoutElement> m_elements;
    unsigned int m_stride;
};

template<>
inline void VertexBufferLayout::Push<float>(unsigned int count) {
    m_elements.push_back(VertexBufferLayoutElement{GL_FLOAT, count, GL_FALSE});
    m_stride += VertexBufferLayoutElement::GetSizeOfType(GL_FLOAT);
}

template<>
inline void VertexBufferLayout::Push<unsigned int>(unsigned int count) {
    m_elements.push_back(VertexBufferLayoutElement{GL_UNSIGNED_INT, count, GL_FALSE});
    m_stride += VertexBufferLayoutElement::GetSizeOfType(GL_UNSIGNED_INT);
}

template<>
inline void VertexBufferLayout::Push<unsigned char>(unsigned int count) {
    m_elements.push_back(VertexBufferLayoutElement{GL_UNSIGNED_BYTE, count, GL_TRUE});
    m_stride += VertexBufferLayoutElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}


#endif //VERTEXBUFFERLAYOUT_H

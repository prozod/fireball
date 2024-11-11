#include "vertexArray.h"
#include "renderer.h"

VertexArray::VertexArray() {
    GLCall(glGenVertexArrays(1, &m_rendererID));
}

VertexArray::~VertexArray() {
    GLCall(glDeleteVertexArrays(1, &m_rendererID));
}

void VertexArray::AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout) const {
    // need to be bound before calling glEnableVertexAttribArray, otherwise glEnableVertexAttribArray wont have a valid VAO to modify/operate on.
    Bind();
    vb.Bind();
    const auto &elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++) {
        // stride needs to be carefully calculated else itll screw up textures
        // nr. elem * sizeoftype = stride, 4elem*sizeof(float)=16bytes per vertex
        // std::cout << "STRIDE: " << layout.GetStride() << " | SIZEOF FLOAT: " << sizeof(float) << std::endl;
        const auto &element = elements[i];
        GLCall(glEnableVertexAttribArray(i)); // enable vertex at certain index
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized,
            2 * layout.GetStride(), reinterpret_cast<void*>(offset))); // set/define layout of our vertex attrib data
        offset += element.count * VertexBufferLayoutElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const {
    GLCall(glBindVertexArray(m_rendererID));
}

void VertexArray::Unbind() {
    GLCall(glBindVertexArray(0));
}



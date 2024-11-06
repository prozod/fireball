#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H
#include "vertexBuffer.h"
#include "vertexBufferLayout.h"


class VertexArray {
public:
    VertexArray();

    ~VertexArray();

    void AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);

    void Bind() const;

    void Unbind();

private:
    unsigned int m_rendererID{};
};


#endif //VERTEXARRAY_H

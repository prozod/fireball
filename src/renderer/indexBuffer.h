#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H
#include "renderer.h"


class IndexBuffer {
public:
    IndexBuffer(const unsigned int *data, unsigned int count);

    ~IndexBuffer();

    void Bind() const;

    void Unbind() const;

    [[nodiscard]] inline unsigned int GetCount() const { return m_count; }

private:
    unsigned int m_rendererID{};
    unsigned int m_count{};
};


#endif //INDEXBUFFER_H

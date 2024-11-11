#ifndef TEXTURE_H
#define TEXTURE_H
#include "../renderer/renderer.h"

class Texture {
private:
    unsigned int m_rendererId{};
    std::string m_filepath;
    unsigned char *m_localbuffer;
    int m_width, m_height, m_bitsperpixel;

public:
    explicit Texture(const std::string &filepath);

    ~Texture();

    void Bind(unsigned int slot = 0) const;

    void Unbind() const;

    inline int GetWidth() const { return m_width; }
    inline int GetHeight() const { return m_height; }
};


#endif //TEXTURE_H

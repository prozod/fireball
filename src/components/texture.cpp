#include "texture.h"
#include "stb_image.h"

Texture::Texture(const std::string &path): m_filepath(path), m_localbuffer(nullptr), m_width(0), m_height(0),
                                           m_bitsperpixel(0) {
    stbi_set_flip_vertically_on_load(1);
    m_localbuffer = stbi_load(path.c_str(), &m_width, &m_height, &m_bitsperpixel, 4);
    GLCall(glGenTextures(1, &m_rendererId));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererId));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localbuffer));
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));

    if (m_localbuffer) {
        stbi_image_free(m_localbuffer);
    }
    if (!m_localbuffer) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return;
    }
}

Texture::~Texture() {
    GLCall(glDeleteTextures(1, &m_rendererId));
}

void Texture::Bind(unsigned int slot) const {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_rendererId));
}

void Texture::Unbind() const {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

#pragma once

#include <glad/glad.h>
#include <string>

struct Texture
{
private:
    bool m_IsInverted;
    uint32_t m_RendererId;
    std::string m_FilePath;
    unsigned char *m_LocalBuffer;
    int m_Width, m_Height, m_BPP;
public:
    Texture(const std::string &path, bool isInverted);
    ~Texture();

    void Bind(uint32_t slot = 0) const;
    void Unbind() const;

    inline int getWidth() const {return m_Width;}
    inline int getHeight() const {return m_Height;}

};

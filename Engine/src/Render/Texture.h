#pragma once

#include <cstdint>
#include <string>

struct Texture
{
public:
    Texture(const std::string &path, bool isInverted, uint32_t slot = 0);
    Texture(uint32_t width, uint32_t height, uint32_t slot = 0);
    Texture(const Texture& other) = default;
    ~Texture();

    void Bind();
    void Unbind() const;

    inline int GetWidth() const {return m_Width;}
    inline int GetHeight() const {return m_Height;}

    inline uint32_t GetID() const { return m_RendererId; }
    inline uint32_t GetSlot() const { return m_Slot; }

    void Resize(uint32_t width, uint32_t height);
private:
    bool m_IsInverted;
    uint32_t m_RendererId;
    std::string m_FilePath;
    unsigned char *m_LocalBuffer;
    int m_Width, m_Height, m_BPP;
    uint32_t m_Slot = 0;
};

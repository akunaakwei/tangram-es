#pragma once

#include "gl.h"
#include "scene/spriteAtlas.h"

#include <vector>
#include <memory>
#include <string>

namespace Tangram {

class RenderState;

enum class TextureMinFilter : GLenum {
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR,
    NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
};

enum class TextureMagFilter : GLenum {
    NEAREST = GL_NEAREST,
    LINEAR = GL_LINEAR,
};

enum class TextureWrap : GLenum {
    CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
    REPEAT = GL_REPEAT,
};

enum class PixelFormat : GLenum {
    ALPHA = GL_ALPHA,
    LUMINANCE = GL_LUMINANCE,
    LUMINANCE_ALPHA = GL_LUMINANCE_ALPHA,
    RGB = GL_RGB,
    RGBA = GL_RGBA,
};

struct TextureOptions {
    TextureMinFilter minFilter = TextureMinFilter::LINEAR;
    TextureMagFilter magFilter = TextureMagFilter::LINEAR;
    TextureWrap wrapS = TextureWrap::CLAMP_TO_EDGE;
    TextureWrap wrapT = TextureWrap::CLAMP_TO_EDGE;
    PixelFormat pixelFormat = PixelFormat::RGBA;
    float displayScale = 1.f; // 0.5 for a "@2x" image.
    bool generateMipmaps = false;
};

class Texture {

public:

    explicit Texture(TextureOptions _options);

    Texture(const uint8_t* data, size_t length, TextureOptions _options);

    Texture(const Texture& _other) = delete;
    Texture& operator=(const Texture& _other) = delete;
    Texture(Texture&& _other) = delete;
    Texture& operator=(Texture&& _other) = delete;

    virtual ~Texture();

    bool loadImageFromMemory(const uint8_t* data, size_t length);

    /* Sets texture pixel data */
    bool movePixelData(int _width, int _height, int _bytesPerPixel, GLubyte* _data, size_t _length);
    bool setPixelData(int _width, int _height, int _bytesPerPixel, const GLubyte* _data, size_t _length);

    void setRowsDirty(int start, int count);

    void setSpriteAtlas(std::unique_ptr<SpriteAtlas> sprites);

    /* Perform texture updates, should be called at least once and after adding data or resizing */
    virtual void update(RenderState& rs, GLuint _textureSlot);

    /* Resize the texture */
    void resize(int width, int height);

    void bind(RenderState& rs, GLuint _unit);

    /* Width and Height texture getters */
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

    GLuint getGlHandle() const { return m_glHandle; }

    float getDisplayScale() const { return m_options.displayScale; }

    const auto& getSpriteAtlas() const { return m_spriteAtlas; }

    /* Checks whether the texture has valid data and has been successfully uploaded to GPU */
    bool isValid() const;

    size_t getBufferSize() const { return m_bufferSize; }

    size_t getBytesPerPixel() const;

protected:

    struct DirtyRowRange {
        int min;
        int max;
    };

    void generate(RenderState& rs, GLuint _textureUnit);

    bool sanityCheck(size_t _width, size_t _height, size_t _bytesPerPixel, size_t _length) const;

    TextureOptions m_options;

    std::unique_ptr<GLubyte> m_buffer;
    size_t m_bufferSize;
    size_t m_bytesPerPixel;

    std::vector<DirtyRowRange> m_dirtyRows;

    GLuint m_glHandle = 0;

    bool m_shouldResize = false;
    // Dipose buffer after texture upload
    bool m_disposeBuffer = true;

    int m_width = 0;
    int m_height = 0;

    RenderState* m_rs = nullptr;

private:

    std::unique_ptr<SpriteAtlas> m_spriteAtlas;

};

} // namespace Tangram

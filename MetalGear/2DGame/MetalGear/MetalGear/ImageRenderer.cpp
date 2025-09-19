#include "ImageRenderer.h"
#include "Game.h"
#include <iostream>

ImageRenderer::ImageRenderer()
{
    texProgram = nullptr;
    vao = 0;
    vbo = 0;
    initialized = false;
}

ImageRenderer::~ImageRenderer()
{
    free();
}

void ImageRenderer::init(ShaderProgram* shaderProgram)
{
    if (initialized) return;

    texProgram = shaderProgram;
    projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);

    createQuad();
    initialized = true;

    std::cout << "ImageRenderer initialized successfully" << std::endl;
}

void ImageRenderer::createQuad()
{
    // Crear un quad básico que se actualizará dinámicamente
    float vertices[24] = {
        0.f, 0.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 1.f, 1.f,
        0.f, 0.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 1.f,
        0.f, 0.f, 0.f, 1.f
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_DYNAMIC_DRAW);

    posLocation = texProgram->bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
    texCoordLocation = texProgram->bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

bool ImageRenderer::loadImage(const std::string& name, const std::string& filename)
{
    if (loadedImages.find(name) != loadedImages.end()) {
        std::cout << "Image '" << name << "' already loaded" << std::endl;
        return true;
    }

    Texture texture;
    if (!texture.loadFromFile(filename, TEXTURE_PIXEL_FORMAT_RGBA)) {
        std::cout << "Error loading image: " << filename << std::endl;
        return false;
    }

    texture.setWrapS(GL_CLAMP_TO_EDGE);
    texture.setWrapT(GL_CLAMP_TO_EDGE);
    texture.setMinFilter(GL_LINEAR);
    texture.setMagFilter(GL_LINEAR);

    loadedImages[name] = std::move(texture);

    std::cout << "Image '" << name << "' loaded successfully. Size: "
        << loadedImages[name].width() << "x" << loadedImages[name].height() << std::endl;

    return true;
}

void ImageRenderer::updateQuadVertices(const glm::vec2& position, const glm::vec2& size, const ImageRegion& region)
{
    // Intercambiar top y bottom para corregir la inversión
    float vertices[24] = {
        position.x, position.y, region.left, region.top,                       // Bottom-left
        position.x + size.x, position.y, region.right, region.top,            // Bottom-right  
        position.x + size.x, position.y + size.y, region.right, region.bottom, // Top-right
        position.x, position.y, region.left, region.top,                       // Bottom-left
        position.x + size.x, position.y + size.y, region.right, region.bottom, // Top-right
        position.x, position.y + size.y, region.left, region.bottom            // Top-left
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 24 * sizeof(float), vertices);
}

void ImageRenderer::renderImage(const std::string& imageName, const glm::vec2& position, const glm::vec2& size)
{
    ImageRegion fullImage; // Usa toda la imagen (0,0) a (1,1)
    renderImageRegion(imageName, position, size, fullImage);
}

void ImageRenderer::renderImageRegion(const std::string& imageName,
    const glm::vec2& position,
    const glm::vec2& size,
    const ImageRegion& region)
{
    if (!initialized) {
        std::cout << "ImageRenderer not initialized!" << std::endl;
        return;
    }

    auto it = loadedImages.find(imageName);
    if (it == loadedImages.end()) {
        std::cout << "Image '" << imageName << "' not found!" << std::endl;
        return;
    }

    // Actualizar vértices del quad
    updateQuadVertices(position, size, region);

    // Configurar shader
    texProgram->use();
    texProgram->setUniformMatrix4f("projection", projection);
    texProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

    glm::mat4 modelview = glm::mat4(1.0f);
    texProgram->setUniformMatrix4f("modelview", modelview);
    texProgram->setUniform2f("texCoordDispl", 0.f, 0.f);

    // Renderizar
    glEnable(GL_TEXTURE_2D);
    it->second.use();
    glBindVertexArray(vao);
    glEnableVertexAttribArray(posLocation);
    glEnableVertexAttribArray(texCoordLocation);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisable(GL_TEXTURE_2D);
}

void ImageRenderer::renderFullscreen(const std::string& imageName)
{
    renderImage(imageName, glm::vec2(0.0f, 0.0f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT));
}

void ImageRenderer::renderFullscreenRegion(const std::string& imageName, const ImageRegion& region)
{
    renderImageRegion(imageName, glm::vec2(0.0f, 0.0f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT), region);
}

bool ImageRenderer::isImageLoaded(const std::string& imageName) const
{
    return loadedImages.find(imageName) != loadedImages.end();
}

void ImageRenderer::free()
{
    if (vbo != 0) {
        glDeleteBuffers(1, &vbo);
        vbo = 0;
    }
    if (vao != 0) {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }

    loadedImages.clear();
    initialized = false;
}
#ifndef _IMAGE_RENDERER_INCLUDE
#define _IMAGE_RENDERER_INCLUDE

#include "Texture.h"
#include "ShaderProgram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <map>
#include <string>

// Estructura para definir una región de la imagen (para spritesheets)
struct ImageRegion
{
    float left, right, top, bottom;

    ImageRegion() : left(0.0f), right(1.0f), top(0.0f), bottom(1.0f) {}
    ImageRegion(float l, float r, float t, float b) : left(l), right(r), top(t), bottom(b) {}
};

class ImageRenderer
{
public:
    static ImageRenderer& instance()
    {
        static ImageRenderer renderer;
        return renderer;
    }

    // Inicializar el renderer con el shader program
    void init(ShaderProgram* shaderProgram);

    // Cargar una imagen y guardarla en cache
    bool loadImage(const std::string& name, const std::string& filename);

    // Renderizar imagen completa
    void renderImage(const std::string& imageName,
        const glm::vec2& position,
        const glm::vec2& size);

    // Renderizar región específica de la imagen
    void renderImageRegion(const std::string& imageName,
        const glm::vec2& position,
        const glm::vec2& size,
        const ImageRegion& region);

    // Renderizar imagen con escalado automático para llenar la pantalla
    void renderFullscreen(const std::string& imageName);

    // Renderizar región de imagen a pantalla completa
    void renderFullscreenRegion(const std::string& imageName, const ImageRegion& region);

    // Liberar recursos
    void free();

    // Verificar si una imagen está cargada
    bool isImageLoaded(const std::string& imageName) const;

private:
    ImageRenderer();
    ~ImageRenderer();

    // No permitir copias
    ImageRenderer(const ImageRenderer&) = delete;
    ImageRenderer& operator=(const ImageRenderer&) = delete;

    void createQuad();
    void updateQuadVertices(const glm::vec2& position, const glm::vec2& size, const ImageRegion& region);

private:
    ShaderProgram* texProgram;
    std::map<std::string, Texture> loadedImages;
    GLuint vao;
    GLuint vbo;
    GLint posLocation, texCoordLocation;
    glm::mat4 projection;
    bool initialized;
};

#endif // _IMAGE_RENDERER_INCLUDE#pragma once

#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_resize.h"
#include "../globals.hpp"

std::map<std::string, Texture> Texture::textures = {};

Texture Texture::getFromUrl(const std::string &url, bool rgba) {
    static Texture emptyTexture = Texture::genEmptyTexture();
    if (textures.find(url) != textures.end()) {
        Texture &texture = textures[url];
        if (texture.loaded) {
            return texture;
        } else {
            if (texture.gotContent) {
                texture.loadTextureContent();
                return texture;
            }
            return emptyTexture;
        }
    }

    auto &tex = Texture::textures[url];
    tex.rgba = rgba;

    std::thread t([url]() {
        Texture::loadTexture(url);
    });
    t.detach();
    return emptyTexture;
}

void Texture::loadTexture(const std::string url) {
    int width, height, nrChannels;
    Texture &texture = Texture::textures[url];

    auto response = cpr::Get(cpr::Url{url});
    if (response.status_code != 200) {
        return;
    }

    auto data = stbi_load_from_memory(
        (unsigned char *)response.text.data(), response.text.size(), &width, &height, &nrChannels, 0);

    texture.data = (unsigned char *)malloc(width * height * nrChannels);
    stbir_resize_uint8(data, width, height, 0, texture.data, 64, 64, 0, nrChannels);

    stbi_image_free(data);

    if (texture.data == nullptr) {
        return;
    }
    texture.gotContent = true;
    texture.width = 64;
    texture.height = 64;
    return;
}

void Texture::loadTextureContent() {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, rgba ? GL_RGBA : GL_RGB, width, height, 0, rgba ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
    loaded = true;
    gotContent = false;
}
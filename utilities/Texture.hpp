#ifndef BPSCANNER_TEXTURE_HPP
#define BPSCANNER_TEXTURE_HPP

#include "GLFW/glfw3.h"
#include "cpr/cpr.h"
#include "map"
#include "string"
#include "vector"

class Texture {
   private:
    bool gotContent = false;
    unsigned char *data;
    bool loaded = false;
    bool rgba = false;
    int channels = 0;
   public:
    static std::map<std::string, Texture> textures;
    GLuint id;
    int width;
    int height;

    // load texture from url to use in imgui
    static Texture getFromUrl(const std::string &url, bool rgba = false);

   private:
    static void loadTexture(const std::string url);
    void loadTextureContent();

    static Texture genEmptyTexture() {
        Texture texture;
        texture.width = 1;
        texture.height = 1;

        glGenTextures(1, &texture.id);
        glBindTexture(GL_TEXTURE_2D, texture.id);

        unsigned char data[4] = {255, 255, 255, 0};

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        return texture;
    }
};

#endif  // BPSCANNER_TEXTURE_HPP
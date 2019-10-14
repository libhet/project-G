#ifndef TEXTURE_H
#define TEXTURE_H

#include <SOIL/SOIL.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Texture {
public:
    Texture() = default;
    Texture(const char* file_name) {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        unsigned char * image = SOIL_load_image(file_name, &width, &height, 0, SOIL_LOAD_AUTO);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);
    }    

    void Bind()     {glBindTexture(GL_TEXTURE_2D, texture);}
    void UnBind()   {glBindTexture(GL_TEXTURE_2D, 0);}

public:
    int width   = 0;
    int height  = 0;

    GLuint texture = 0;
};


#endif // TEXTURE_H
#pragma once
#ifndef DRAWABLE_CPP
#define DRAWABLE_CPP

#include <string>
#include <vector>

class Drawable
{
private:
    unsigned int VBO;
    unsigned int VAO;
    unsigned int program;
    std::vector<float> vertices;
    std::vector<int> indices;

public:
    Drawable(std::string filePath);
    ~Drawable();
};

#endif

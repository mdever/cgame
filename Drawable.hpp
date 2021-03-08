#pragma once
#ifndef DRAWABLE_CPP
#define DRAWABLE_CPP

#include <string>
#include <vector>

class Drawable
{
public:
    unsigned int VBO;
    unsigned int normalsVBO;
    unsigned int VAO;
    unsigned int EBO;
    unsigned int program;
    glm::vec3 position;
    glm::mat4 modelMatrix;
    std::vector<float> vertices;
    std::vector<float> verticesUnpacked;
    std::vector<float> vertexNormals;
    std::vector<float> vertexNormalsUnpacked;
    std::vector<unsigned int> vertexNormalIndicies;
    std::vector<unsigned int> indices;
    const char* vertexShaderSource;
    const char* fragmentShaderSource;
    unsigned int vertexShader;
    unsigned int fragmentShader;

public:
    Drawable(std::string filePath);
    ~Drawable();

    void setVertexShaderSource(const char* source);
    void setFragmentShaderSource(const char* source);
    bool compileVertexShader(char** error_log);
    bool compileFragmentShader(char** error_log);
    bool linkProgram(char** errorLog);
    bool setupBuffers(char** errorLog);
    void setPosition(glm::vec3 position);

    void draw();
};

#endif

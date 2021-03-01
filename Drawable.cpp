
#include <stdlib.h>

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/gl.h>

#include "Utils.hpp"
#include "Drawable.hpp"

Drawable::Drawable(std::string filePath)
    : VBO(0), VAO(0), EBO(0), program(0), vertexShaderSource(nullptr), fragmentShaderSource(nullptr), vertexShader(0), fragmentShader(0)
{
    std::ifstream in(filePath, std::ifstream::binary);
    if (!in) {
        WriteToConsole(L"Could not read file.\n");
    }

    std::string line;
    while (std::getline(in, line))
    {

        std::cout << line << std::endl;
        if (line[0] == 'v')
        {
            float x, y, z;
            int res = sscanf_s(line.c_str(), "%*s %f%f%f", &x, &y, &z);

            this->vertices.push_back(x);
            this->vertices.push_back(y);
            this->vertices.push_back(z);
        }
        else if (line[0] == 'f')
        {
            unsigned int v1, v2, v3;
            int res = sscanf_s(line.c_str(), "%*s %u%u%u", &v1, &v2, &v3);

            this->indices.push_back(v1 - 1);
            this->indices.push_back(v2 - 1);
            this->indices.push_back(v3 - 1);

        }
    }

    for (int i = 0; i < this->indices.size(); i++)
    {
        int vertexIndex = this->indices.at(i);
        this->verticesUnpacked.push_back(this->vertices.at(vertexIndex * 3));
        this->verticesUnpacked.push_back(this->vertices.at(vertexIndex * 3 + 1));
        this->verticesUnpacked.push_back(this->vertices.at(vertexIndex * 3 + 2));

    }
}

Drawable::~Drawable() {}

void Drawable::setVertexShaderSource(const char* source)
{
    this->vertexShaderSource = source;
}

void Drawable::setFragmentShaderSource(const char* source)
{
    this->fragmentShaderSource = source;
}

bool Drawable::compileVertexShader(char** error_log)
{
    this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(this->vertexShader, 1, &this->vertexShaderSource, NULL);
    glCompileShader(this->vertexShader);

    int result;
    size_t bytesWritten;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        WriteToConsole(L"Error compiling Vertex Shader...\n");
        char *infoLog = new char[128];
        wchar_t wInfoLog[128];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        mbstowcs_s(&bytesWritten, wInfoLog, infoLog, strlen(infoLog));
        WriteToConsole(wInfoLog);
        *error_log = infoLog;
        return false;
    }
    return true;
}

bool Drawable::compileFragmentShader(char** error_log)
{
    this->fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(this->vertexShader, 1, &this->fragmentShaderSource, NULL);
    glCompileShader(this->fragmentShader);

    int result;
    size_t bytesWritten;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        WriteToConsole(L"Error compiling Fragment Shader...\n");
        char* infoLog = new char[128];
        wchar_t wInfoLog[128];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        mbstowcs_s(&bytesWritten, wInfoLog, infoLog, strlen(infoLog));
        WriteToConsole(wInfoLog);
        *error_log = infoLog;
        return false;
    }
    return true;
}

bool Drawable::linkProgram(char** error_log)
{
    this->program = glCreateProgram();
    glAttachShader(this->program, this->vertexShader);
    glAttachShader(this->program, this->fragmentShader);
    glLinkProgram(this->program);

    int result;
    glGetProgramiv(this->program, GL_LINK_STATUS, &result);
    if (!result)
    {
        char* infoLog = new char[512];
        wchar_t wInfoLog[512];
        size_t bytesWritten;
        glGetProgramInfoLog(this->program, 512, NULL, infoLog);

        mbstowcs_s(&bytesWritten, wInfoLog, infoLog, strlen(infoLog));
        WriteToConsole(L"Error Linking Shader Program...\n");
        WriteToConsole(wInfoLog);

        *error_log = infoLog;
        return false;
    }

    return true;
}

bool Drawable::setupBuffers(char** errorMessage)
{
    glUseProgram(this->program);
    glGenBuffers(1, &this->VBO);
    glGenVertexArrays(1, &this->VAO);
    wchar_t msg[128];
    swprintf(msg, 128, L"VBO: %d, VAO: %d, EBO: %d\n", VBO, VAO, EBO);
    WriteToConsole(msg);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->verticesUnpacked.size() * sizeof(float), &this->verticesUnpacked[0], GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    return true;
}

void Drawable::draw()
{
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_LINES, 0, this->verticesUnpacked.size() / 3);
}

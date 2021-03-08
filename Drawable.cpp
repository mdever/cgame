
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
#include <glm\ext\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#define _CRT_SECURE_NO_WARNINGS

Drawable::Drawable(std::string filePath)
    : VBO(0), VAO(0), EBO(0), program(0), vertexShaderSource(nullptr), fragmentShaderSource(nullptr), vertexShader(0), fragmentShader(0)
{
    this->position = glm::vec3(0.0f, 0.0f, 0.0f);
    this->modelMatrix = glm::translate(glm::mat4(1.0f), this->position);

    std::ifstream in(filePath, std::ifstream::binary);
    if (!in) {
        WriteToConsole(L"Could not read file.\n");
    }

    std::string line;
    while (std::getline(in, line))
    {

        std::cout << line << std::endl;
        if (line[0] == 'v' && line[1] == ' ')
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
            unsigned int vt1, vt2, vt3;
            unsigned int vn1, vn2, vn3;
            /*int res = sscanf_s(line.c_str(), "%*s %u%u%u", &v1, &v2, &v3);

            this->indices.push_back(v1 - 1);
            this->indices.push_back(v2 - 1);
            this->indices.push_back(v3 - 1);*/

            char* state1;
            size_t max;
            char* token;
            char cLine[256];
            strcpy_s(cLine, 256, line.c_str());
            token = strtok_s(cLine, " ", &state1);
            while (token != NULL) {
                std::string sLine(token);
                if (sLine[0] == 'f') {
                    token = strtok_s(NULL, " ", &state1);
                    continue;
                }
                

                if (sLine.find("/") == std::string::npos) {
                    unsigned int v;
                    int res = sscanf_s(sLine.c_str(), "%u", &v);

                    this->indices.push_back(v - 1);
                }
                else
                {
                    unsigned int v, vt, vn;
                    char working[32];
                    int workingLength = 0;
                    char* state2;
                    int i = 0;
                    for (i; i < strlen(token); i++) {
                        if (token[i] == '/' || token[i] == '\0') {
                            break;
                        }

                        working[workingLength++] = token[i];
                    }

                    working[workingLength + 1] = '\0';
                    v = atoi(working);
                    memset(working, 0, 32);
                    workingLength = 0;
                    for (i++; i < strlen(token); i++) {
                        if (token[i] == '/' || token[i] == '\0') {
                            break;
                        }

                        working[workingLength++] = token[i];
                    }
                    working[workingLength + 1] = '\0';
                    vt = atoi(working);
                    memset(working, 0, 32);
                    workingLength = 0;

                    for (i++; i < strlen(token); i++) {
                        if (token[i] == '/' || token[i] == '\0') {
                            break;
                        }

                        working[workingLength++] = token[i];
                    }
                    working[workingLength + 1] = '\0';
                    vn = atoi(working);
                    memset(working, 0, 32);
                    workingLength = 0;

                    this->indices.push_back(v - 1);
                    this->vertexNormalIndicies.push_back(vn - 1);
                }

                token = strtok_s(NULL, " ", &state1);
            }



        }
        else if (line[0] == 'v' && line[1] == 'n')
        {
            float vnx, vny, vnz;
            int res = sscanf_s(line.c_str(), "%*s %f%f%f", &vnx, &vny, &vnz);

            this->vertexNormals.push_back(vnx);
            this->vertexNormals.push_back(vnx);
            this->vertexNormals.push_back(vnx);
        }
    }

    for (int i = 0; i < this->indices.size(); i++)
    {
        int vertexIndex = this->indices.at(i);
        this->verticesUnpacked.push_back(this->vertices.at(vertexIndex * 3));
        this->verticesUnpacked.push_back(this->vertices.at(vertexIndex * 3 + 1));
        this->verticesUnpacked.push_back(this->vertices.at(vertexIndex * 3 + 2));
    }
    for (int i = 0; i < this->vertexNormalIndicies.size(); i++)
    {
        int vertexIndex = this->indices.at(i);
        this->vertexNormalsUnpacked.push_back(this->vertices.at(vertexIndex * 3));
        this->vertexNormalsUnpacked.push_back(this->vertices.at(vertexIndex * 3 + 1));
        this->vertexNormalsUnpacked.push_back(this->vertices.at(vertexIndex * 3 + 2));
    }

    printf("Done");
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
    glGenBuffers(1, &this->normalsVBO);
    glGenVertexArrays(1, &this->VAO);
    wchar_t msg[128];
    swprintf(msg, 128, L"VBO: %d, VAO: %d, EBO: %d\n", VBO, VAO, EBO);
    WriteToConsole(msg);

    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->verticesUnpacked.size() * sizeof(float), &this->verticesUnpacked[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(this->normalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertexNormalsUnpacked.size() * sizeof(float), &this->vertexNormalsUnpacked[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    return true;
}

void Drawable::setPosition(glm::vec3 position)
{
    this->position = position;
    this->modelMatrix = glm::translate(glm::mat4(), this->position);
}

void Drawable::draw()
{
    int modelMatrixLocation = glGetUniformLocation(this->program, "modelMatrix");
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(this->modelMatrix));
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, this->verticesUnpacked.size() / 3);
}

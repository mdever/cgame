
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>

#include "Utils.hpp"
#include "Drawable.hpp"

Drawable::Drawable(std::string filePath)
    : VBO(0), VAO(0), program(0)
{
    std::ifstream in(filePath, std::ifstream::binary);
    if (!in) {
        WriteToConsole(L"Could not read file.\n");
    }

    std::string line;
    std::wstring wLine;
    while (std::getline(in, line))
    {
        if (line[0] == 'v')
        {
            char prelude[8];
            float x, y, z;
            sscanf_s(line.c_str(), "%s %f %f %f", prelude, &x, &y, &z);
            wchar_t msg[128];
            swprintf(msg, 128, L"Read Vector: %f, %f, %f\n", x, y, z);
            WriteToConsole(msg);
        }
        else
        {

        }
    }
}

Drawable::~Drawable() { }

#include "Utils.hpp"
#include <windows.h>
#include <glm/glm.hpp>

extern HANDLE g_hConsole;

BOOL WriteToConsole(const wchar_t* msg)
{
    DWORD charsWritten;
    WriteConsole(g_hConsole, msg, wcslen(msg), &charsWritten, NULL);
    return TRUE;
}

BOOL WriteToConsole(const char* msg)
{
    DWORD charsWritten;
    wchar_t *wMsg = new wchar_t[strlen(msg)];
    size_t charsConverted;
    mbstowcs_s(&charsConverted, wMsg, strlen(msg), msg, strlen(msg));
    WriteConsole(g_hConsole, wMsg, wcslen(wMsg), &charsWritten, NULL);
    delete[] wMsg;
    return TRUE;
}

float magnitude(glm::vec3 v)
{
    return std::cbrt(v.x*v.x + v.y*v.y + v.z*v.z);
}
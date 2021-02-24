#include "Utils.hpp"
#include <windows.h>

extern HANDLE g_hConsole;

BOOL WriteToConsole(const wchar_t* msg)
{
    DWORD charsWritten;
    WriteConsole(g_hConsole, msg, wcslen(msg), &charsWritten, NULL);
    return TRUE;
}

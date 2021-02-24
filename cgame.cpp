#ifndef UNICODE
#define UNICODE
#endif

#define GLEW_STATIC

#include <windows.h>
#include <WindowsX.h>
#include <tchar.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GL/gl.h>


#include "Utils.hpp"
#include "Drawable.hpp"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HINSTANCE g_hInstance;
HANDLE g_hConsole;

/* OpenGL Triangle Initialization */
float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
};
unsigned int VBO;
unsigned int VAO;
unsigned int vertexShader;
unsigned int fragmentShader;
unsigned int shaderProgram;

Drawable* teapot;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"  FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

BOOL SetupShaders()
{
    int result;
    char infoLog[512];
    wchar_t wInfoLog[512];
    wchar_t wShaderSource[512];

    glViewport(0, 0, 800, 600);
    glEnable(GL_MULTISAMPLE);
    memset(wShaderSource, 0, 512);
    size_t bytesWritten;
    mbstowcs_s(&bytesWritten, wShaderSource, vertexShaderSource, strlen(vertexShaderSource));
    WriteToConsole(L"Vertex Shader Source: \n");
    WriteToConsole(wShaderSource);

    memset(wShaderSource, 0, 512);
    mbstowcs_s(&bytesWritten, wShaderSource, fragmentShaderSource, strlen(fragmentShaderSource));
    WriteToConsole(L"Fragment Shader Source: \n");
    WriteToConsole(wShaderSource);

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        WriteToConsole(L"Error compiling Vertex Shader...\n");
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        mbstowcs_s(&bytesWritten, wInfoLog, infoLog, strlen(infoLog));
        WriteToConsole(wInfoLog);
        return FALSE;
    }
    WriteToConsole(L"Compiled Vertex Shader successfully.\n");

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        WriteToConsole(L"Error compiling Fragment Shader...\n");
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        mbstowcs_s(&bytesWritten, wInfoLog, infoLog, strlen(infoLog));
        WriteToConsole(wInfoLog);
        return FALSE;
    }

    WriteToConsole(L"Compiled Fragment Shader successfully.\n");

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        mbstowcs_s(&bytesWritten, wInfoLog, infoLog, strlen(infoLog));
        WriteToConsole(L"Error Linking Shader Program...\n");
        WriteToConsole(wInfoLog);
        return FALSE;
    }

    WriteToConsole(L"Linked shader program successfully.\n");

    glUseProgram(shaderProgram);
    return TRUE;
}

BOOL SetupVertexAttribs()
{
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    wchar_t msg[128];
    swprintf(msg, 128, L"VBO: %d, VAO: %d\n", VBO, VAO);
    WriteToConsole(msg);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    return TRUE;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

    const wchar_t CLASS_NAME[] = L"Sample Window Class";
    int pf;
    HDC hDC;
    PIXELFORMATDESCRIPTOR pfd;
    HGLRC hRC;

    g_hInstance = hInstance;

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.style = CS_OWNDC;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Learn to Program Windows",
        WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL,
        NULL,
        hInstance,
        NULL);

    AllocConsole();
    g_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    {
        DWORD charsWritten;
        wchar_t msg[] = L"Testing Console...\n";
        WriteConsole(g_hConsole, msg, wcslen(msg), &charsWritten, NULL);
    }


    if (hwnd == NULL)
    {
        return NULL;
    }

    hDC = GetDC(hwnd);

    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.cColorBits = 32;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.iLayerType = PFD_MAIN_PLANE;

    pf = ChoosePixelFormat(hDC, &pfd);
    if (pf == NULL) {
        MessageBox(NULL, L"Could not find pixelformat", L"Error", MB_OK | MB_ICONEXCLAMATION);
        return -1;
    }

    SetPixelFormat(hDC, pf, &pfd);

    DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    hDC = GetDC(hwnd);

    WriteToConsole(L"Creating gl context\n");

    hRC = wglCreateContext(hDC);

    if (hRC == NULL) {
        MessageBox(NULL, L"Could not create GL context", L"Error", MB_OK | MB_ICONEXCLAMATION);
        return -1;
    }

    WriteToConsole(L"Making context current\n");

    wglMakeCurrent(hDC, hRC);


    WriteToConsole(L"Showing Window\n");

    ShowWindow(hwnd, nCmdShow);

    WriteToConsole(L"Initializing GLEW\n");
    GLenum status;
    if ((status = glewInit()) != GLEW_OK) {
        char* errorString = (char*)glewGetErrorString(status);
        wchar_t wErrorString[128];
        size_t bytesWritten;
        mbstowcs_s(&bytesWritten, wErrorString, errorString, strlen(errorString) + 1);
        MessageBox(NULL, wErrorString, L"Error", MB_OK | MB_ICONEXCLAMATION);
        return -1;
    }

    WriteToConsole(L"Setting up shaders\n");
    BOOL result = SetupShaders();
    if (!result) {
        MessageBox(NULL, L"Error setting up shaders", L"Error", MB_OK | MB_ICONEXCLAMATION);
        return -1;
    }

    WriteToConsole(L"Setting up Vertex Buffers\n");
    SetupVertexAttribs();

    WriteToConsole(L"Initializing teapot.obj");
    teapot = new Drawable("./teapot.obj");
    WriteToConsole(L"Done initializing teapot.obj");

    WriteToConsole(L"Beginning Message Loop\n");

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    wglMakeCurrent(NULL, NULL);
    ReleaseDC(hwnd, hDC);
    wglDeleteContext(hRC);
    DestroyWindow(hwnd);

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_CREATE:
        WriteToConsole(L"Received WM_CREATE message\n");
        {
            WriteToConsole(L"Received WM_CREATE message\n");
            wchar_t msg[128];
            memset(msg, 0, 128);
            CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
            swprintf(msg, 128, L"Width: %d, Height: %d\n", cs->cx, cs->cy);
            WriteToConsole(msg);
            glViewport(0, 0, cs->cx, cs->cy);
            glClear(GL_COLOR_BUFFER_BIT);
        }
        return 0;

    case WM_PAINT:
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        {
            HDC hDC = GetDC(hwnd);
            SwapBuffers(hDC);
        }
        glBindVertexArray(0);
        glUseProgram(0);

        {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
        }

        return 0;
    case WM_LBUTTONDOWN:
    {
        WriteToConsole(L"Received WM_LBUTTONDOWN message\n");
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);
        wchar_t msg[256];
        swprintf(msg, 128, L"User clicked: %d, %d\n", xPos, yPos);
        WriteToConsole(msg);
    }
    return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

}


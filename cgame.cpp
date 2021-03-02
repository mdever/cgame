#ifndef UNICODE
#define UNICODE
#endif

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <windows.h>
#include <WindowsX.h>
#include <tchar.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/gl.h>


#include "Utils.hpp"
#include "Drawable.hpp"
#include "Camera.hpp"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HINSTANCE g_hInstance;
HANDLE g_hConsole;
HGLRC g_hRC;
glm::mat4 g_viewMatrix;
glm::vec3 g_viewPosition = glm::vec3(0.0f, 1.0f, 10.0f);
glm::vec3 g_viewTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_viewUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 g_projectionMatrix;

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

Drawable* g_teapot;
Camera* g_camera;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 viewMatrix;\n"
"uniform mat4 projectionMatrix;\n"
"void main()\n"
"{\n"
"  gl_Position = projectionMatrix * viewMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
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

    WriteToConsole(L"Initializing Camera\n");
    g_viewMatrix = glm::lookAt(g_viewPosition, g_viewTarget, g_viewUp);

    g_projectionMatrix = glm::perspective(
        glm::radians(45.0f),
        4.0f / 3.0f,
        0.1f,
        1000.0f
    );

    int viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(g_viewMatrix));
    int projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(g_projectionMatrix));
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

    g_hRC = wglCreateContext(hDC);

    if (g_hRC == NULL) {
        MessageBox(NULL, L"Could not create GL context", L"Error", MB_OK | MB_ICONEXCLAMATION);
        return -1;
    }

    WriteToConsole(L"Making context current\n");

    wglMakeCurrent(hDC, g_hRC);


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

    WriteToConsole(L"Initializing teapot.obj\n");
    g_teapot = new Drawable("./teapot.obj");
    g_camera = new Camera(g_viewPosition, g_viewTarget, g_viewUp);

    Drawable* teapot = g_teapot;
    teapot->setVertexShaderSource(vertexShaderSource);
    teapot->setFragmentShaderSource(fragmentShaderSource);

    {
        bool success;
        char* errorMessage;
        success = teapot->compileVertexShader(&errorMessage);
        if (!success) {
            printf("Got an error compiling vertex shader:\n%s", errorMessage);
            delete errorMessage;
        }



        success = teapot->compileFragmentShader(&errorMessage);
        if (!success) {
            printf("Got an error compiling vertex shader:\n%s", errorMessage);
            delete errorMessage;
        }


        success = teapot->linkProgram(&errorMessage);
        if (!success) {
            printf("Got an error linking program:\n%s", errorMessage);
            delete errorMessage;
        }

        success = teapot->setupBuffers(&errorMessage);
        if (!success) {
            printf("Got an error setting up buffers:\n%s", errorMessage);
            delete errorMessage;
        }

    }

    WriteToConsole(L"Done initializing teapot.obj\n");

    WriteToConsole(L"Beginning Message Loop\n");

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    wglMakeCurrent(NULL, NULL);
    ReleaseDC(hwnd, hDC);
    wglDeleteContext(g_hRC);
    DestroyWindow(hwnd);

    return 0;
}

bool isDragging = false;
bool truckPedestalDrag = false;
POINT lastDragLocation = { };
POINT lastPedestalDragLocation = { };
const float CAMERA_SPACE_FACTOR = 1.0f;
const float MOUSE_WHEEL_FACTOR = 0.5f;
const float FLAT_MOVE_FACTOR = 0.02f;

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
            glClearColor(0.2f, 0.7f, 0.5f, 0.5f);
            glClear(GL_COLOR_BUFFER_BIT);
        }
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            wglMakeCurrent(hdc, g_hRC);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(shaderProgram);

            int viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
            glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(g_camera->viewMatrix));

            g_teapot->draw();

            SwapBuffers(hdc);
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

        POINT pt = { xPos, yPos };
        if (DragDetect(hwnd, pt))
        {
            WriteToConsole(L"Initiating click and drag\n");
            isDragging = true;
            lastDragLocation = pt;
        }
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        if (isDragging) {
            WriteToConsole(L"Expecting to move object here\n");
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            POINT latest{ x, y };
            int deltaX = latest.x - lastDragLocation.x;
            int deltaY = lastDragLocation.y - latest.y;
            float deltaXCamera = CAMERA_SPACE_FACTOR * deltaX;
            float deltaYCamera = CAMERA_SPACE_FACTOR * deltaY;
            wchar_t msg[256];
            swprintf(msg, 256, L"translating camera matrix radially %f, %f\n", deltaXCamera, deltaYCamera);
            WriteToConsole(msg);

            g_camera->moveRadiallyUp(deltaYCamera);
            g_camera->moveRadiallyRight(deltaXCamera);
            lastDragLocation = latest;
            SendMessage(hwnd, WM_PAINT, NULL, NULL);
        }

        if (truckPedestalDrag) {
            WriteToConsole(L"Expecting to flat move camera\n");
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            POINT latest = POINT{ x, y };
            int deltaX = latest.x - lastPedestalDragLocation.x;
            int deltaY = latest.y - lastPedestalDragLocation.y;
            float deltaXCamera = FLAT_MOVE_FACTOR * deltaX;
            float deltaYCamera = FLAT_MOVE_FACTOR * deltaY;
            wchar_t msg[256];
            swprintf(msg, 256, L"translating camera matrix %f, %f\n", deltaXCamera, deltaYCamera);
            WriteToConsole(msg);

            g_camera->truckRight(deltaXCamera);
            g_camera->pedestalUp(deltaYCamera);
            lastPedestalDragLocation = latest;
            SendMessage(hwnd, WM_PAINT, NULL, NULL);
        }

        return 0;
    }
    case WM_LBUTTONUP:
    {
        if (isDragging) {
            WriteToConsole(L"Drag stopped\n");
            isDragging = false;
            lastDragLocation = { };
        }

        return 0;
    }
    case WM_MOUSEWHEEL:
    {
        short wScrollWheelDirection = HIWORD(wParam);

        float scrollWheelDirection = static_cast<double>(wScrollWheelDirection) / 120.0f;

        wchar_t msg[128];
        swprintf(msg, 128, L"Preparing to zoom: %f\n", scrollWheelDirection);
        WriteToConsole(msg);

        g_camera->moveForward(scrollWheelDirection * MOUSE_WHEEL_FACTOR);
        
        SendMessage(hwnd, WM_PAINT, NULL, NULL);
        return 0;
    }
    case WM_MBUTTONDOWN:
    {
        POINT pt{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        truckPedestalDrag = true;
        WriteToConsole(L"About to activate flat movement\n");
        lastPedestalDragLocation = pt;

        return 0;
    }
    case WM_MBUTTONUP:
    {
        if (truckPedestalDrag) {
            truckPedestalDrag = false;
            lastPedestalDragLocation = { };
        }
    }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

}


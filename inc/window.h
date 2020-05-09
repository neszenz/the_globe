// based on 'https://github.com/HectorPeeters/opengl_premake_boilerplate'
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

struct WindowData
{
    const char *title;
    unsigned width, height;
    bool vsync;
};

class Window
{
public:
    Window(const char *title, unsigned width, unsigned height, bool vsync, int samples);
    ~Window();

    bool IsValid();

    void Update();

    bool ShouldClose();

    bool IsVsync();
    void SetVsync(bool vsync);

    GLFWwindow *GetGlfwWindow();
    WindowData *GetDataPointer();

    glm::ivec2 GetSize();

private:
    static int m_NumOfWindows;
    bool m_IsValid;

    GLFWwindow *m_Window;
    WindowData m_Data;
};

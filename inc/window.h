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
    Window(const char *title, unsigned width, unsigned height, bool vsync = true, int samples = 0);
    ~Window();

    void MakeContextCurrent() const;

    void Update();

    bool ShouldClose();

    bool IsVsync();
    void SetVsync(bool vsync);

    GLFWwindow *GetGlfwWindow();
    WindowData *GetDataPointer();

    glm::ivec2 GetSize() const;

private:
    static int m_NumOfWindows;

    GLFWwindow *m_Window;
    WindowData m_Data;
};

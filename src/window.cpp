// based on 'https://github.com/HectorPeeters/opengl_premake_boilerplate'
#include "window.h"

#include "gl_utils.h"
#include "icu.hpp" // glfw_key_callback

#include <stdio.h>

void error_callback(int error, const char* description)
{
    fprintf(stderr, "glfw_error_callback() - %s\n", description);
}
void framebuffer_size_callback(GLFWwindow* win, int w, int h)
{
    WindowData *data = (WindowData *)glfwGetWindowUserPointer(win);
    data->width = w;
    data->height = h;
    GL(glViewport(0, 0, w, h));

    //printf("Resized %d, %d\n", data->width, data->height);
}
void set_default_callbacks(GLFWwindow *window)
{
    glfwSetErrorCallback(error_callback);
    glfwSetWindowSizeCallback(window, framebuffer_size_callback);

    glfwSetKeyCallback(window, glfw_key_callback);
}

int Window::m_NumOfWindows = 0;

Window::Window(const char *title, unsigned width, unsigned height, bool vsync = true, int samples = 0)
{
    m_Data = WindowData{title, width, height, vsync};

    if (m_NumOfWindows == 0) {
        if (!glfwInit()) {
            fprintf(stderr, "Failed to init GLFW\n");
            m_IsValid = false;
            return;
        }
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, samples);

    m_Window = glfwCreateWindow(m_Data.width, m_Data.height, m_Data.title, NULL, NULL);
    if (m_Window == NULL) {
        fprintf(stderr, "Failed to create GLFW window");
        m_IsValid = false;
        return;
    }

    glfwSetWindowUserPointer(m_Window, &m_Data);
    set_default_callbacks(m_Window);

    glfwMakeContextCurrent(m_Window);
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to init GLEW\n");
        glfwDestroyWindow(m_Window);
        m_IsValid = false;
        return;
    }

    SetVsync(m_Data.vsync);

    m_NumOfWindows++;
    m_IsValid = true;
    return;
}

Window::~Window()
{
    glfwDestroyWindow(m_Window);
    m_NumOfWindows--;

    if (m_NumOfWindows == 0) {
        glfwTerminate();
    }
}

bool Window::IsValid()
{
    return m_IsValid;
}

void Window::MakeContextCurrent()
{
    glfwMakeContextCurrent(m_Window);
}

void Window::Update()
{
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

bool Window::ShouldClose()
{
    return glfwWindowShouldClose(m_Window);
}

bool Window::IsVsync()
{
    return m_Data.vsync;
}

void Window::SetVsync(bool vsync)
{
    m_Data.vsync = vsync;
    glfwSwapInterval(vsync ? 1 : 0);
}

GLFWwindow *Window::GetGlfwWindow()
{
    return m_Window;
}

WindowData *Window::GetDataPointer()
{
    return &m_Data;
}

glm::ivec2 Window::GetSize()
{
    return {m_Data.width, m_Data.height};
}

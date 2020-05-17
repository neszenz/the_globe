// based on 'https://github.com/HectorPeeters/opengl_premake_boilerplate'
#include "window.h"

#include "gl_utils.h"
#include "icu.hpp" // glfw_key_callback

#include <sstream>
#include <string>
#include <stdio.h>

#define PRINT_VERSIONS true

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

void print_versions()
{
    const char* glfw_version = glfwGetVersionString();
    const unsigned char* opengl_version = glGetString(GL_VERSION);
    const unsigned char* vendor = glGetString(GL_VENDOR);
    const unsigned char* renderer = glGetString(GL_RENDERER);
    const unsigned char* glsl = glGetString(GL_SHADING_LANGUAGE_VERSION);
    int numOfVertexAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numOfVertexAttributes);
    printf("using GLFW %s\n", glfw_version);
    printf("using OpenGL %s | %s | %s | GLSL %s | maxVertAttribs %i\n",
            opengl_version, vendor, renderer, glsl, numOfVertexAttributes);
}

int Window::m_NumOfWindows = 0;

Window::Window(const char *title, unsigned width, unsigned height, bool vsync, int samples)
{
    m_Data = WindowData{title, width, height, vsync};

    if (m_NumOfWindows == 0) {
        if (!glfwInit()) {
            fprintf(stderr, "Failed to init GLFW\n");
            throw std::runtime_error("Window() error");
        }
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, samples);

    m_Window = glfwCreateWindow(m_Data.width, m_Data.height, m_Data.title, NULL, NULL);
    if (m_Window == NULL) {
        fprintf(stderr, "Failed to create GLFW window");
        throw std::runtime_error("Window() error");
    }

    glfwSetWindowUserPointer(m_Window, &m_Data);
    set_default_callbacks(m_Window);

    glfwMakeContextCurrent(m_Window);
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to init GLEW\n");
        glfwDestroyWindow(m_Window);
        throw std::runtime_error("Window() error");
    }

    SetVsync(m_Data.vsync);

    if (PRINT_VERSIONS)
        print_versions();

    m_NumOfWindows++;
}

Window::~Window()
{
    glfwDestroyWindow(m_Window);
    m_NumOfWindows--;

    if (m_NumOfWindows == 0) {
        glfwTerminate();
    }
}

void Window::MakeContextCurrent() const
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

glm::ivec2 Window::GetSize() const
{
    return {m_Data.width, m_Data.height};
}

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <raymath.h>
#include <cassert>
#include <iostream>
#include <vector>
#include "Shader.h"

void APIENTRY DebugCallback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

struct App
{
    int object_index;
};

int main(void)
{
    App app{};

    assert(glfwInit() == GLFW_TRUE);

    // Request certain OpenGL features
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef NDEBUG
    // Don't make a debug context if in release mode
#else
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(1600, 900, "Graphics 1", nullptr, nullptr);
    assert(window != nullptr);

    glfwSetWindowUserPointer(window, &app);
    glfwSetKeyCallback(window, KeyCallback);

    glfwMakeContextCurrent(window);
    assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == true);

#ifdef NDEBUG
#else
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(DebugCallback, nullptr);
#endif

    const size_t square_count = 16;
    const size_t vertex_count = square_count * 8;
	Vector3 corners[4] = {
		{ -1.0f, -1.0f, 0.0f }, // bottom left
		{ 1.0f, -1.0f, 0.0f }, // bottom right
		{ 1.0f,  1.0f, 0.0f }, // top right
		{ -1.0f,  1.0f, 0.0f } // top left
	};
    Vector3 colors[square_count] = {
        { 1.0f, 1.0f, 1.0f }, // white
        { 1.0f, 0.0f, 0.0f }, // red
        { 0.0f, 1.0f, 0.0f }, // green
        { 0.0f, 0.0f, 1.0f }, // blue
        { 1.0f, 1.0f, 0.0f }, // yellow
        { 1.0f, 0.0f, 1.0f }, // magenta
        { 0.0f, 1.0f, 1.0f }, // cyan
        { 1.0f, 0.5f, 0.0f }  // orange
    };

    std::vector<Vector3> line_positions;
    std::vector<Vector3> line_colors;
    line_positions.resize(vertex_count);
    line_colors.resize(vertex_count);

    for (size_t s = 0; s < square_count; s++)
    {
        size_t i = s * 8; // square is 8 vertices (4 edges x 2 endpoints)

        // every edge needs both of its endpoints listed
        // the last pair wraps back to corner 0 to close the square
        for (size_t j = 0; j < 4; j++)
        {
            line_positions[i + j * 2 + 0] = corners[j];
            line_positions[i + j * 2 + 1] = corners[(j + 1) % 4];
        }

        // set the colour of the squares edges to the colour of the square
        for (size_t j = 0; j < 8; j++)
        {
            line_colors[i + j] = colors[s % 8];
        }

        // create the next square. its corners are the midpoints of the previous squares edges
        // put it into a temp array first so that every midpoint is computed from the original corners to not overwrite
        Vector3 midpoints[4];
        for (size_t j = 0; j < 4; j++)
        {
            midpoints[j] = Vector3Scale(Vector3Add(corners[j], corners[(j + 1) % 4]), 0.5f);
        }
        for (size_t j = 0; j < 4; j++)
        {
            corners[j] = midpoints[j];
        }
    }

/*    // 4 Horizontal lines
    for (size_t i = 0; i < vertex_count / 2; i+= 2)
    {
        t += 0.4f;
        line_positions[i + 0] = { -1.0f, t, 0.0f };
        line_positions[i + 1] = {  1.0f, t, 0.0f };
    }

    // 4 Vertical lines
    t = -1.0f;
    for (size_t i = vertex_count / 2; i < vertex_count; i += 2)
    {
        t += 0.4f;
        line_positions[i + 0] = { t, -1.0f, 0.0f };
        line_positions[i + 1] = { t,  1.0f, 0.0f };
    }

    
    line_colors.resize(vertex_count);
    for (size_t i = 0; i < vertex_count / 2; i++)
    {
        line_colors[i] = { 1.0f, 0.0f, 0.0f };
    }
    for (size_t i = vertex_count / 2; i < vertex_count; i++)
    {
        line_colors[i] = { 1.0f, 0.0f, 1.0f };
    }*/
        
    assert(vertex_count == line_positions.size() && vertex_count == line_colors.size());
    GLuint line_vao = GL_NONE;
    GLuint line_positions_vbo = GL_NONE;
    GLuint line_colors_vbo = GL_NONE;

    glGenVertexArrays(1, &line_vao);
    glGenBuffers(1, &line_positions_vbo);
    glGenBuffers(1, &line_colors_vbo);

    glBindVertexArray(line_vao);                            // "Record the following state of vertex buffers within this vertex array"

    glBindBuffer(GL_ARRAY_BUFFER, line_positions_vbo);      // The following array (vertex) buffer will be described (triangle_positions_vbo):
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);// Index 0, 3 components, float data, no implicit normalization, no stride, no offset
    glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(Vector3), line_positions.data(), GL_STATIC_DRAW); // Transfer memory of triangle_positions into currently bound vbo
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, line_colors_vbo);         // The following array (vertex) buffer will be described (triangle_colors_vbo):
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);// Index 1, 3 components, float data, no implicit normalization, no stride, no offset
    glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(Vector3), line_colors.data(), GL_STATIC_DRAW); // Transfer memory of triangle_colors into currently bound vbo
    glEnableVertexAttribArray(1);

    glBindVertexArray(GL_NONE);                                 // "Stop recording vertex buffer state"

    GLuint vs_shader = LoadShader(GL_VERTEX_SHADER, "./assets/shaders/a2_lines.vert");
    GLuint fs_shader = LoadShader(GL_FRAGMENT_SHADER, "./assets/shaders/a2_lines.frag");
    GLuint shader_program = LoadProgram(vs_shader, fs_shader);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(line_vao);

        // Core profile caps line width at 1.0 on most drivers; anything
        // larger raises GL_INVALID_VALUE every frame.
        glLineWidth(1.0f);
        glDrawArrays(GL_LINES, 0, vertex_count);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    App* app = (App*)glfwGetWindowUserPointer(window);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
    {
        // Bound our array to be [0..4]
        ++app->object_index %= 5;
    }
}

void APIENTRY DebugCallback(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}

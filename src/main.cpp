#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <cassert>
#include <iostream>

int main(void)
{
	assert(glfwInit() == GLFW_TRUE);

    // Request certian OpenGL features
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef NDEBUG
    // Don't make a debug context if in release mode
#else
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif


    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    assert(window != nullptr);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    // We forgot to initialize glad; our OpenGL extension-loader, so when we called glClear the funciton was not loaded
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

#ifdef NDEBUG
#else
	glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(DebugCallback, nullptr);
#endif

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void APIENTRY DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
		return;
	std::cout << "_______________" << message << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;
}
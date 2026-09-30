#include "Window.h"
#include "Shader.h"
#include "raymath.h"
#include <cstddef>

struct Vertex
{
    Vector2 pos;   // offset of 0 (4 bytes for pos.x + 4 bytes for pos.y = 8)
    Vector3 col;   // offset of 8 and then (4 bytes for col.r + 4 bytes for color.g + 4 bytes for color.b = 12)
};

static const Vertex vertices_white[3] =
{
    { { -0.6f, -0.4f }, { 1.0f, 1.0f, 1.0f } },
    { {  0.6f, -0.4f }, { 1.0f, 1.0f, 1.0f } },
    { {   0.f,  0.6f }, { 1.0f, 1.0f, 1.0f } }
};

static const Vector2 vertex_positions[3] =
{
    { -0.6f, -0.4f },
    { 0.6f, -0.4f },
    { 0.f,  0.6f }
};

static const Vector3 vertex_colors[3] =
{
    { 1.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f }
};

int main()
{
    CreateWindow(800, 800, "Week 4");

    GLuint a1_tri_vert = CreateShader(GL_VERTEX_SHADER, "./assets/shaders/a1_triangle.vert");
    GLuint a1_tri_frag = CreateShader(GL_FRAGMENT_SHADER, "./assets/shaders/a1_triangle.frag");
    GLuint a1_tri_shader = CreateProgram(a1_tri_vert, a1_tri_frag);

    GLuint vertex_buffer_rainbow_positions;
    GLuint vertex_buffer_rainbow_colors;
    glGenBuffers(1, &vertex_buffer_rainbow_positions);
    glGenBuffers(1, &vertex_buffer_rainbow_colors);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_rainbow_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_positions), vertex_positions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_rainbow_colors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_colors), vertex_colors, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
    // Can only have 1 vertex buffer bound at a time, so must unbind in order to prevent overwriting it

    GLuint vertex_buffer_white;
    glGenBuffers(1, &vertex_buffer_white);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_white);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_white), vertices_white, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

    GLuint vertex_array_rainbow;
    glGenVertexArrays(1, &vertex_array_rainbow);
    glBindVertexArray(vertex_array_rainbow);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_rainbow_positions);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_rainbow_colors);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0);

    glBindVertexArray(GL_NONE);

    GLuint vertex_array_white;
    glGenVertexArrays(1, &vertex_array_white);
    glBindVertexArray(vertex_array_white);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_white);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, col));

    glBindVertexArray(GL_NONE);

    int object_index = 0;

    GLint u_color = glGetUniformLocation(a1_tri_shader, "u_color");

	GLint u_mvp = glGetUniformLocation(a1_tri_shader, "u_mvp");

	// Aspect ratio of the window (width / height)
	float aspect = WindowWidth() / (float)WindowHeight();
	float near = 0.01f;
	float far = 100.0f;

	//Scale the projection matrix to account for the aspect ratio of the window
	Matrix world = MatrixScale(0.3f, 0.3f, 1.0f) * MatrixRotateZ(0.0f * DEG2RAD) * MatrixTranslate(0.0f, 0.0f, 5.0f);
	Matrix view = MatrixLookAt({ 0.0f, 0.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, Vector3UnitY);
	// Perspective = 3D projection, (closer objects = bigger, further objects = smaller)
	Matrix proj = MatrixPerspective(75.0f * DEG2RAD, aspect, near, far);

	Matrix mvp = world * view * proj;

    /* Loop until the user closes the window */
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ESCAPE))
            SetWindowShouldClose(true);

        // Colors are represented as fractions between 0.0 and 1.0, so convert using a colour-picker tool accordingly!
        float r = 239.0f / 255.0f;
        float g = 136.0f / 255.0f;
        float b = 190.0f / 255.0f;
        float a = 1.0f;

        // Time in seconds since GLFW was initialized ( use this with functions like sinf and cosf for repeating animations)
		float time = Time();

        /* Render here */
        glClearColor(r, g, b, a);

        glClear(GL_COLOR_BUFFER_BIT);

        if (IsKeyPressed(KEY_SPACE))
        {
            ++object_index %= 5;
        }

        switch (object_index)
        {
        case 0:
        {
            glUseProgram(a1_tri_shader);

            world = MatrixScale(0.3f, 0.3f, 1.0f) * MatrixTranslate(0.5f, 0.5f, 9.0f);
            mvp = world * view * proj;

            glUniform3f(u_color, 1.0f, 0.0f, 0.0f);
            glUniformMatrix4fv(u_mvp, 1, GL_FALSE, MatrixToFloat(mvp));
            glBindVertexArray(vertex_array_white);
            glDrawArrays(GL_TRIANGLES, 0, 3);


            world = MatrixScale(0.3f, 0.3f, 1.0f) * MatrixTranslate(-0.5f, 0.5f, 9.0f);
            mvp = world * view * proj;
            glUniformMatrix4fv(u_mvp, 1, GL_FALSE, MatrixToFloat(mvp));
            glUniform3f(u_color, 0.0f, 1.0f, 0.0f);
            glBindVertexArray(vertex_array_white);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            world = MatrixScale(0.3f, 0.3f, 1.0f) * MatrixTranslate(-0.5f, -0.5f, 9.0f);
            mvp = world * view * proj;
            glUniformMatrix4fv(u_mvp, 1, GL_FALSE, MatrixToFloat(mvp));
            glUniform3f(u_color, 0.0f, 0.0f, 1.0f);
            glBindVertexArray(vertex_array_white);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            world = MatrixScale(0.3f, 0.3f, 1.0f) * MatrixTranslate(0.5f, -0.5f, 9.0f);
            mvp = world * view * proj;
            glUniformMatrix4fv(u_mvp, 1, GL_FALSE, MatrixToFloat(mvp));
            glUniform3f(u_color, 1.0f, 1.0f, 0.0f);
            glBindVertexArray(vertex_array_white);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            break;
        }
        case 1:
        {
            glPointSize(10);

            glUseProgram(a1_tri_shader);

            world = MatrixIdentity();
            mvp = world * view * proj;
            glUniformMatrix4fv(u_mvp, 1, GL_FALSE, MatrixToFloat(mvp));

            glUniform3f(u_color, 1.0f, 0.0f, 1.0f);
            glBindVertexArray(vertex_array_white);
            glDrawArrays(GL_POINTS, 0, 3);
            break;
        }
        case 2:
        {
            glPointSize(10);
            glUseProgram(a1_tri_shader);

            Matrix s = MatrixScale(5.0f, 5.0f, 1.0f);
            Matrix r = MatrixRotateZ(time * 100.0f * DEG2RAD);
            Matrix t = MatrixTranslate(0.0f, 0.0f, 0.0f);

            world = s * r * t;
            mvp = world * view * proj;
            glUniformMatrix4fv(u_mvp, 1, GL_FALSE, MatrixToFloat(mvp));

            glUniform3f(u_color, 1.0f, 1.0f, 1.0f);
            glBindVertexArray(vertex_array_rainbow);
            glDrawArrays(GL_LINE_LOOP, 0, 3);
            break;
        }
        case 3:
        {
            Matrix s = MatrixIdentity();
            Matrix r = MatrixIdentity();
            Matrix t = MatrixIdentity();
			Matrix view = MatrixLookAt({ 0.0f, 0.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, Vector3UnitY);
			Matrix proj = MatrixOrtho(-1.0f, 1.0f, -1.0f, 1.0f, near, far);

            float time = Time();
			float alpha = cosf(time) * 0.5f + 0.5f; // oscillates between 0 and 1
			Vector3 A = { -1.0f, -1.0f, 0.0f };
            Vector3 B = { 1.0f, 1.0f, 0.0f };
            Vector3 C = Vector3Lerp(A, B, alpha);
			t = MatrixTranslate(C.x, C.y, C.z);

            world = s * r * t;
            mvp = world * view * proj;
            glUniformMatrix4fv(u_mvp, 1, GL_FALSE, MatrixToFloat(mvp));

            glUseProgram(a1_tri_shader);
            glUniform3f(u_color, 0.8, 0.8f, 0.8f);
            glBindVertexArray(vertex_array_rainbow);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
        case 4:
        {
			Matrix view5 = MatrixLookAt({ 0.0f, 0.0f, 10.0f }, { 0.0f, 0.0f, 0.0f }, Vector3UnitY);
			Matrix proj5 = MatrixOrtho(-10.0f, 10.0f, -10.0f, 10.0f, near, far);

			//Translation interpolation
            float time = Time();
            float alpha = cosf(time) * 0.5f + 0.5f; // oscillates between 0 and 1
            Vector3 tA = { 0.0f, -10.0f, 0.0f };
            Vector3 tB = { 0.0f, 10.0f, 0.0f };
            Vector3 tC = Vector3Lerp(tA, tB, alpha);

            // Scale interpolation
            Vector3 sA = { 1.0f, 1.0f, 1.0f };
            Vector3 sB = { 10.0f, 10.0f, 1.0f };
            Vector3 sC = Vector3Lerp(sA, sB, alpha);

			// Spherical Lerp because we interpolate between two quaternions (rotation) instead of two vectors (position)
			Quaternion qA = QuaternionIdentity();
			Quaternion qB = QuaternionFromEuler(0.0f, 0.0f, 90.0f * DEG2RAD);
			Quaternion qC = QuaternionSlerp(qA, qB, alpha);

			Matrix s = MatrixScale(sC.x, sC.y, sC.z);
			Matrix r = QuaternionToMatrix(qC);
			Matrix t = MatrixTranslate(tC.x, tC.y, tC.z);

            //Color interpolation
            Vector3 cA = Vector3UnitY; // Green
            Vector3 cB = Vector3UnitZ; // Blue
			Vector3 cC = Vector3Lerp(cA, cB, alpha);

            Matrix mvp = s * r * t * view * proj;

            glUseProgram(a1_tri_shader);

			glUniformMatrix4fv(u_mvp, 1, GL_FALSE, MatrixToFloat(mvp));
            glUniform3f(u_color, cC.x, cC.y, cC.z);

            glBindVertexArray(vertex_array_rainbow);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            break;
        }
        default:
            break;
        }

        // Called at end of the frame to swap buffers and update input
        Loop();
    }


    glDeleteVertexArrays(1, &vertex_array_rainbow);
    glDeleteVertexArrays(1, &vertex_array_white);
    glDeleteBuffers(1, &vertex_buffer_rainbow_positions);
    glDeleteBuffers(1, &vertex_buffer_rainbow_colors);
    glDeleteBuffers(1, &vertex_buffer_white);
    glDeleteProgram(a1_tri_shader);
    glDeleteShader(a1_tri_frag);
    glDeleteShader(a1_tri_vert);

    DestroyWindow();

    return 0;
}
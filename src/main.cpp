#include "Window.h"
#include "Shader.h"
#include "raymath.h"
#include "raymath.h"

struct Vertex
{
	Vector2 pos; // offset of 0
	Vector3 col; // offset of 8 (4 bytes for pos.x + 4 bytes for pos.y = 8)
};

static const Vertex vertices_white[3] =
{
	{ { -0.6f, -0.4f }, { 1.0f, 0.0f, 0.0f } },
	{ { 0.6f, -0.4f }, { 1.0f, 0.0f, 0.0f } },
	{ { 0.0f, 0.6f }, { 1.0f, 0.0f, 0.0f } }
};
static const Vertex vertex_positions[3] =
{
	{ -0.6f, -0.4f },
	{ 0.6f, -0.4f },
	{ 0.0f, 0.6f } 
};
static const Vertex vertex_colors[3] =
{
	{ 1.0f , 0.0f, 0.0f },
	{ 0.0f, 1.0f, 0.0f },
	{ 0.0f, 0.0f, 1.0f }
};

int main()
{
	CreateWindow(800, 800, "Triangle Example");
	return 0;
}
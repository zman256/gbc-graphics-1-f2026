#version 430 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_tcoord;

out vec3 position;
out vec3 normal;
out vec2 tcoord;

// World (model) matrix for the debug colours
uniform mat4 u_transform;

// Model * View * Projection
uniform mat4 u_mvp;

void main()
{
    vec4 pos = u_transform * vec4(vertex_position, 1.0);

    position = pos.xyz;
    tcoord = vertex_tcoord;
    normal = mat3(u_transform) * vertex_normal;

    gl_Position = u_mvp * vec4(vertex_position, 1.0);
}

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform vec2 mouse_pos;
uniform mat4 rotation;

out vec3 ourColor;

void main()
{
    gl_Position = rotation*vec4(aPos.x+mouse_pos.x, aPos.y+mouse_pos.y, aPos.z, 1.0);
    ourColor = aColor;
}
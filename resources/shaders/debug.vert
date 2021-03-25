#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;
uniform mat4 pv;
void main()
{
        gl_Position = pv * vec4(aPos.x, aPos.y, aPos.z, 1.f);
        ourColor = aColor;

}

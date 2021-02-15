#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
out vec3 ourNormal;
out vec2 TexCoord;
uniform mat4 pvm;
void main()
{
        gl_Position = pvm * vec4(aPos.x, aPos.y, aPos.z, 1.f);
        ourNormal = aNormal;
        TexCoord = aTexCoord;
}

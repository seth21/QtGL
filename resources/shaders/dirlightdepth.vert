#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 projMat;
uniform mat4 lightViewMat;
uniform mat4 modelMat;

void main()
{
    vec4 lightViewPos =  lightViewMat * modelMat * vec4(aPos, 1.0);
    gl_Position = projMat * lightViewPos;
}  
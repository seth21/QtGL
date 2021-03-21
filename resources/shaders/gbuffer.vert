#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;

out vec2 TexCoord;
out vec3 FragPos;
uniform mat4 projMat;
uniform mat4 viewMat; 
uniform mat4 modelMat;

#ifdef NORMAL
out mat3 TBN;
#else
out vec3 Normal;
#endif

void main()
{  
        FragPos = (modelMat * vec4(aPos, 1.0)).xyz;
        TexCoord = aTexCoord;
        #ifdef NORMAL
        //create TBN matrix
        vec3 T = normalize(vec3(modelMat * vec4(aTangent,   0.0)));
        vec3 N = normalize(vec3(modelMat * vec4(aNormal,    0.0)));
        vec3 B = cross(T, N);
        TBN = mat3(T, B, N);
        #else
        Normal = mat3(transpose(inverse(modelMat))) * aNormal; 
        #endif
        gl_Position = projMat * viewMat * vec4(FragPos, 1.0);
}
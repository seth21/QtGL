#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;

out vec2 TexCoord;
uniform mat4 projMat;
uniform mat4 viewMat; 
uniform mat4 modelMat;
out vec3 fragPos;

#ifdef NORMAL
out mat3 TBN;
out mat3 inverseTBN;
#else
out vec3 ourNormal;
#endif

void main()
{  
        fragPos = (modelMat * vec4(aPos, 1.0)).xyz;
        TexCoord = aTexCoord;
        #ifdef NORMAL
        //create TBN matrix (tangent to world space)
        vec3 T = normalize(vec3(modelMat * vec4(aTangent,   0.0)));
        vec3 N = normalize(vec3(modelMat * vec4(aNormal,    0.0)));
        vec3 B = cross(T, N);
        TBN = mat3(T, B, N);
        inverseTBN = transpose(TBN);
        #else
        ourNormal = mat3(transpose(inverse(modelMat))) * aNormal; 
        #endif
        gl_Position = projMat * viewMat * vec4(fragPos, 1.0);
}

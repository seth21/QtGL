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

#ifdef BUMP
uniform vec3 viewPos;
out vec3 tangentViewPos;
out vec3 tangentFragPos;
#endif

#ifdef NORMAL
out mat3 TBN;
#else
out vec3 Normal;
#endif

void main()
{  
        FragPos = (modelMat * vec4(aPos, 1.0)).xyz;
        TexCoord = aTexCoord;
        
        #if defined BUMP || defined NORMAL
        //create TBN matrix (tangent to world space)
        mat3 normalMatrix = mat3(transpose(inverse(modelMat)));
        vec3 T = normalize(normalMatrix * aTangent);
        vec3 N = normalize(normalMatrix * aNormal);
        vec3 B = cross(T, N);
        mat3 _TBN = mat3(T, B, N);
        #endif
        
        #ifdef NORMAL 
        TBN = _TBN;
        #else 
        Normal = mat3(transpose(inverse(modelMat))) * aNormal; 
        #endif

        #ifdef BUMP
        mat3 inverseTBN = transpose(_TBN);
        tangentViewPos  = inverseTBN * viewPos;
        tangentFragPos  = inverseTBN * FragPos;
        #endif
        
        gl_Position = projMat * viewMat * vec4(FragPos, 1.0);
}
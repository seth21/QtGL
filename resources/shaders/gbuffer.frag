#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoord;
in vec3 FragPos;

#ifdef ALBEDO
uniform sampler2D albedoMap;
#endif

#ifdef NORMAL
uniform sampler2D normalMap;
in mat3 TBN;
#else
in vec3 Normal;
#endif

#ifdef SPECULAR
uniform sampler2D specularMap;
#endif

void main()
{    
    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    // also store the per-fragment normals into the gbuffer
    #ifdef NORMAL
    vec3 norm = texture(normalMap, TexCoord).rgb;
    norm = norm * 2.0 - 1.0;   
    norm = normalize(TBN * norm);
    gNormal = norm;
    #else
    gNormal = normalize(Normal);
    #endif
    // and the diffuse per-fragment color
    #ifdef ALBEDO
   
    float gamma = 2.2;
    vec4 albedoTex = texture(albedoMap, TexCoord);
    if (albedoTex.a < 0.3) discard;
    //apply gamma correction ->to linear space
    gAlbedoSpec.rgb = pow(albedoTex.rgb, vec3(gamma));
    //gAlbedoSpec.rgb = albedoTex.rgb;
    #else
    gAlbedoSpec.rgb = vec3(0.8,0.8,0.8);
    #endif
    // store specular intensity in gAlbedoSpec's alpha component
    #ifdef SPECULAR
    gAlbedoSpec.a = texture(specularMap, TexCoord).r;
    #else
    gAlbedoSpec.a = 0.5;
    #endif
}  
#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
#define BUMP
in vec2 TexCoord;
in vec3 FragPos;
uniform vec3 viewPos;
#ifdef ALBEDO
uniform sampler2D albedoMap;
#endif

#ifdef NORMAL
uniform sampler2D normalMap;
in mat3 TBN;
#else
in vec3 Normal;
#endif

#ifdef BUMP
uniform sampler2D bumpMap;
in vec3 tangentViewPos;
in vec3 tangentFragPos;
uniform float height_scale = 1;
#endif

#ifdef SPECULAR
uniform sampler2D specularMap;
#endif

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir, float heightScale)
{ 
    // number of depth layers
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * heightScale; 
    vec2 deltaTexCoords = P / numLayers;
  
    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = 1-texture(bumpMap, currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = 1-texture(bumpMap, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }
    
    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = 1-texture(bumpMap, prevTexCoords).r - currentLayerDepth + layerDepth;
 
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

void main()
{    
    vec2 UV = TexCoord;
    // store the fragment position vector in the first gbuffer texture
    gPosition = FragPos;
    
    #ifdef BUMP
    // offset texture coordinates with Parallax Mapping
    vec3 viewDir = normalize(tangentViewPos - tangentFragPos);
    UV = ParallaxMapping(TexCoord, viewDir, 0.02);
    //if(UV.x > 1.0 || UV.y > 1.0 || UV.x < 0.0 || UV.y < 0.0) discard;
    #endif
    
    // also store the per-fragment normals into the gbuffer
    #ifdef NORMAL
    vec3 norm = texture(normalMap, UV).rgb;
    norm = norm * 2.0 - 1.0;   
    norm = normalize(TBN * norm);
    gNormal = norm;
    #else
    gNormal = normalize(Normal);
    #endif
    // and the diffuse per-fragment color
    #ifdef ALBEDO
   
    float gamma = 2.2;
    vec4 albedoTex = texture(albedoMap, UV);
    if (albedoTex.a < 0.3) discard;
    //apply gamma correction ->to linear space
    gAlbedoSpec.rgb = pow(albedoTex.rgb, vec3(gamma));
    //gAlbedoSpec.rgb = albedoTex.rgb;
    #else
    gAlbedoSpec.rgb = vec3(0.8,0.8,0.8);
    #endif
    // store specular intensity in gAlbedoSpec's alpha component
    #ifdef SPECULAR
    gAlbedoSpec.a = texture(specularMap, UV).r;
    #else
    //gAlbedoSpec.a = 0.5;
    gAlbedoSpec.a = texture(bumpMap, UV).r;
    #endif
}  
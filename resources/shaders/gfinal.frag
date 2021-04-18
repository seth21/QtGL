#version 330 core

out vec4 FragColor;
in vec2 TexCoords;
//GBUFFER
uniform sampler2D gAlbedoSpec;
uniform sampler2D gLight;

//uniform float exposure;
void main()
{ 
    //const float gamma = 2.2;
    vec4 albedoSpec = texture(gAlbedoSpec, TexCoords);
    vec3 albedo = albedoSpec.rgb;
    //Light is in HDR
    vec3 hdrColor = texture(gLight, TexCoords).rgb * albedo;
    // reinhard tone mapping, now everything is in LDR
    //vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
    
    // exposure tone mapping
    //vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // gamma correction -> to srgb space
    //mapped = pow(mapped, vec3(1.0 / gamma));

    FragColor = vec4(hdrColor, 1.0);
    //FragColor = vec4(hdrColor, 1.0);
}
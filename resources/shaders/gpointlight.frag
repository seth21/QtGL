#version 330 core
//out vec4 FragColor;
layout (location = 0) out vec4 FragLight;

uniform vec2 gBufferSize;
//GBUFFER
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
//LIGHTING
//uniform vec3 viewPos;
//uniform vec2 gbufferSize;

struct Light {
    vec3 position;  
  
    vec3 color;
	float maxRadius;
    float constant;
    float linear;
    float quadratic;
}; 

uniform Light light;
uniform vec3 viewPos;


void main()
{ 
    vec2 TexCoords = gl_FragCoord.xy/gBufferSize;
    vec3 worldPos = texture(gPosition, TexCoords).rgb;
    vec4 albedoSpec = texture(gAlbedoSpec, TexCoords);
    vec3 albedo = albedoSpec.rgb;
    float specularStrength = albedoSpec.a;
    vec3 worldNormal = texture(gNormal, TexCoords).rgb;
   
    vec3 lightDir = light.position - worldPos;
    float lightDist = length(lightDir);
    lightDir = normalize(lightDir);
    // diffuse
    vec3 diffuse = clamp(dot(worldNormal, lightDir), 0, 1) * light.color;
    // specular
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, worldNormal);  
    float spec = pow(clamp(dot(viewDir, reflectDir), 0, 1), 32);
    vec3 specular = specularStrength * spec * vec3(1);
    //attenuation
    float attenuation = 1.0 / (light.constant + light.linear * lightDist + 
    		    light.quadratic * (lightDist * lightDist));    
    //enforce max radius
    float radiusInner = 0.75 * light.maxRadius; //falloff to zero starting at 75% of the radius
    attenuation *= clamp((light.maxRadius - lightDist) / (light.maxRadius - radiusInner), 0 , 1);
    FragLight = vec4((diffuse + specular) * attenuation, 1);
}
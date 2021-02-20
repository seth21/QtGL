#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
#ifdef ALBEDO
uniform sampler2D albedoMap;
#endif
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 ambientLight;
uniform vec3 viewPos;
in vec3 fragPos;
#ifdef NORMAL
uniform sampler2D normalMap;
in mat3 TBN;
#else
in vec3 ourNormal;
#endif

float specularStrength = 0.6f;

void main()
{
   // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientLight * lightColor;
  	vec3 lightDir = normalize(lightPos - fragPos);
    
    #ifdef NORMAL
    vec3 norm = texture(normalMap, TexCoord).rgb;
    norm = norm * 2.0 - 1.0;   
    norm = normalize(TBN * norm);
    #else
    vec3 norm = normalize(ourNormal);
    #endif

    // diffuse
    float diff = clamp(dot(norm, lightDir), 0, 1);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(clamp(dot(viewDir, reflectDir), 0, 1), 32);
    vec3 specular = specularStrength * spec * lightColor;  

    #ifdef ALBEDO
    vec3 result = (ambient + diffuse + specular) * texture(albedoMap, TexCoord).rgb;
    #else
    vec3 result = (ambient + diffuse + specular);
    #endif
    FragColor = vec4(result, 1.0);
}

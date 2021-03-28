#version 330 core
//out vec4 FragColor;
layout (location = 0) out vec4 FragLight;
in vec2 TexCoords;
//GBUFFER
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
//LIGHTING
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 ambientLight;
uniform vec3 viewPos;

void main()
{ 
    vec4 albedoSpec = texture(gAlbedoSpec, TexCoords);
    vec3 albedo = albedoSpec.rgb;
    float specularStrength = albedoSpec.a;
    vec3 worldNormal = texture(gNormal, TexCoords).rgb;
    vec3 worldPos = texture(gPosition, TexCoords).rgb;
    //FragColor = vec4(normal.rgb, 1) + 0.1*albedoSpec + 0.1 * normal;
    //float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
    //FragColor = vec4(average, average, average, 1.0);

    float ambientStrength = 0.1;
    vec3 ambient = ambientLight * lightColor;
  	vec3 lightDir = normalize(lightPos - worldPos);
    // diffuse
    float diff = clamp(dot(worldNormal, lightDir), 0, 1);
    vec3 diffuse = diff * lightColor;
    // specular
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, worldNormal);  
    float spec = pow(clamp(dot(viewDir, reflectDir), 0, 1), 32);
    vec3 specular = specularStrength * spec * lightColor;  

    vec3 result = (ambient + diffuse + specular);
    FragLight = vec4(result, 1.0);
}
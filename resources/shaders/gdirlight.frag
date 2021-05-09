#version 330 core
//out vec4 FragColor;
layout (location = 0) out vec4 FragLight;
in vec2 TexCoords;
//GBUFFER
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
//LIGHTING
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 ambientLight;
uniform vec3 viewPos;
uniform sampler2D shadowMap;
uniform mat4 shadowMapSpaceMatrix;
uniform float shadowDistance;
uniform float transitionDistance;
const int pcfCount = 2;
const float totalTexels = (pcfCount * 2.0 + 1.0) * (pcfCount * 2.0 + 1.0);
//ssao
uniform sampler2D ssaoTexture;

/*float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 0.0 : 1.0;

    return shadow;
}  */

//1=not in shadow, 0=fully in shadow
float calculateShadowFactor(vec4 fragPosLightSpace){
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
	vec2 shadowMapSize = textureSize(shadowMap, 0);
	vec2 texelSize = 1.0 / shadowMapSize;
	float total = 0.0;
	
	//float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  
	for (int x = -pcfCount; x <= pcfCount; x++){
		for (int y = -pcfCount; y <= pcfCount; y++){
			float objectNearestLight = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			//is our current point's depth larger than the one stored in the shadowMap in the same point?
			if (projCoords.z -0.001> objectNearestLight){
				//in shadow
				total += 1.0;
			}
		}
	}
	//what percentage of neighbour texels in shadow?
	total /= totalTexels;
	float lightFactor = 1.0 - total;
	return lightFactor;
}

float calcShadowStrength(float cameraDist){
	//fade out the shadows
	float distance = cameraDist - (shadowDistance - transitionDistance);
	distance = distance / transitionDistance;
	distance = clamp(1.0 - distance, 0.0, 1.0);
	return distance;
}

float brightnessContrast(float value, float brightness, float contrast)
{
    return (value - 0.5) * contrast + 0.5 + brightness;
}

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

    float ssao = texture(ssaoTexture, TexCoords).r;
    //contrast
    ssao = pow(ssao, 1.2) * 1.6;
    
    vec3 ambient = ambientLight * ssao;
  	vec3 lightDir = normalize(lightDir);
    // diffuse
    float diff = clamp(dot(worldNormal, -lightDir), 0, 1);
    vec3 diffuse = diff * lightColor;
    // specular
    float shininess = 1;
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectDir = reflect(lightDir, worldNormal);  
    float spec = pow(clamp(dot(viewDir, reflectDir), 0, 1), 32 * shininess);
    vec3 specular = 5 * specularStrength * spec * lightColor;  

    //Shadows
    float cameraDist = length(viewPos - worldPos);
    float shadowFade = calcShadowStrength(cameraDist);
    float shadowFactor = calculateShadowFactor(shadowMapSpaceMatrix * vec4(worldPos + worldNormal * 0.3, 1)) * shadowFade;
    
    vec3 result = (diffuse + specular) * shadowFactor + ambient;
    FragLight = vec4(result, 1.0);
}
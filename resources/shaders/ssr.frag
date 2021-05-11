#version 330 core
layout (location = 0) out vec4 ssrColor;
layout (location = 1) out float rayDistance;
in vec2 TexCoords;

#define JITTER_COUNT 10
#define RAY_SAMPLES 10
#define INITIAL_STEP 1
uniform sampler2D mainTex;
uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D depthSampler;
uniform vec3 cameraPos;
uniform float depthCheckBias;
uniform vec2 RAND_SAMPLES[JITTER_COUNT];
uniform float edgeCutOff;
uniform mat4 projViewMat;
uniform mat4 invProjViewMat;
uniform vec2 camNearFar; //camNear.x = nearPlane, camNear.y = farPlane
uniform vec2 texelSize;
uniform float maxBlurDist;

struct RayTraceOutput
{
bool Hit;
vec2 UV;
float rayLength;
};


//Exponential to linear depth
float linearizeDepth(in float depth){
    float d=depth*2.-1.;
    d= (2. * camNearFar.x * camNearFar.y ) / (camNearFar.y + camNearFar.x - d * (camNearFar.y - camNearFar.x));
    return (d-camNearFar.x)/(camNearFar.y-camNearFar.x);
}

//Convert from world space to screen space [0-1]. Returns: xy=uv coordinates, z=buffer depth at that position
vec3 worldToScreenPos(vec3 worldPos){
    vec4 posClip = projViewMat * vec4(worldPos, 1);
    // Perform perspective divide and transform to [0,1] range
    posClip.xyz /= posClip.w;
    vec3 UVCoords = posClip.xyz * 0.5 + 0.5;
    return UVCoords;
}

//Get buffer depth at screen space coordinates
float depthAtScreenPos(vec2 uv){
    //vec2 inverted = vec2(uv.x, 1-uv.y);
    return texture2D(depthSampler, uv).x;
}

//Get world space coordinates from screen space coordinates+depth
vec3 screenToWorldPos(vec2 UV, float depth){
    vec4 clipSpacePosition = vec4(UV, depth, 1.0) * 2.0 - 1.0;
    vec4 worldSpacePosition = invProjViewMat * clipSpacePosition;
    // Perspective division
    return worldSpacePosition.xyz /= worldSpacePosition.w;
}

RayTraceOutput performRayMarching(vec3 reflPosition, vec3 reflDir){

    RayTraceOutput result;
    result.UV =vec2(-1,-1);
    result.Hit = false;
    // Current position of the sample along the ray
    vec3 sampleWPos;

    // Same of before, but in screen space
    vec3 sampleScreenPos;

    // Position of the nearest surface at the sample position (in screen space)
    vec3 hitSurfaceScreenPos;

    // Length of the next step
    float stepLength = INITIAL_STEP;

    float linearSourceDepth=linearizeDepth(worldToScreenPos(reflPosition).z);
    if (linearSourceDepth > 0.99) return result;

    for(int i = 0; i < RAY_SAMPLES; i++) {
        sampleWPos = reflPosition + reflDir * stepLength;
        sampleScreenPos = worldToScreenPos(sampleWPos);
        if (sampleScreenPos.x < 0 || sampleScreenPos.x > 1 || sampleScreenPos.y < 0 || sampleScreenPos.y > 1) break;
        hitSurfaceScreenPos = vec3(sampleScreenPos.xy,depthAtScreenPos(sampleScreenPos.xy));
        vec3 hitSurfaceWPos = screenToWorldPos(hitSurfaceScreenPos.xy, hitSurfaceScreenPos.z);
        
        int j=0;
        #if JITTER_COUNT>0
        do{
        #endif
        // Linearize the depth to have consistent tests for distant samples
        float linearHitSurfaceDepth=linearizeDepth(hitSurfaceScreenPos.z);
        float linearSampleDepth=linearizeDepth(sampleScreenPos.z);
        bool hit=
            linearHitSurfaceDepth>linearSourceDepth &&
            abs(linearSampleDepth - linearHitSurfaceDepth) < depthCheckBias; // check if the ray hit the surface          
        if(hit && !result.Hit){
            result.Hit = true;
            result.UV=sampleScreenPos.xy;
            result.rayLength = stepLength;
            break;
        }      
        #if JITTER_COUNT>0
        hitSurfaceScreenPos = vec3(sampleScreenPos.xy, texture(depthSampler, sampleScreenPos.xy + RAND_SAMPLES[j].xy * texelSize).r);
        j++;
        }while(j<=JITTER_COUNT);
        #endif             
        // Compute next step length
        stepLength = length(reflPosition - hitSurfaceWPos);
        //stepLength += 0.01f;
    }
    return result;    
}

void main()
{ 
	vec3 reflPosition = texture(gPosition, TexCoords).xyz;
	vec3 reflNormal = texture(gNormal, TexCoords).xyz;
    reflPosition += reflNormal * 0.005;
   
	vec3 vDir = normalize(reflPosition - cameraPos);
	vec3 reflectDir = normalize(reflect(vDir, normalize(reflNormal)));

    // Get the Amount of reflection.
	float shininess = texture(gAlbedoSpec, TexCoords).a;
    
    ssrColor = vec4(0);

	RayTraceOutput ray = performRayMarching(reflPosition, reflectDir);
	
	if (ray.Hit == true)
	{
        // Fade reflection when it samples near the edges
        vec2 edgeFade = smoothstep(vec2(0,0), vec2(0.15,0.2), ray.UV) * smoothstep(vec2(1,1), vec2(0.85,0.8), ray.UV);

        ssrColor = vec4(texture(mainTex, ray.UV).xyz, (edgeFade.x * edgeFade.y) * shininess);
        rayDistance = clamp(ray.rayLength, 0, maxBlurDist)/maxBlurDist;
        return;
	}

}
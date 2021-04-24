#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D mainTex;
uniform float FXAA_SPAN_MAX = 8.0;
uniform float FXAA_REDUCE_MIN = 1.0/(128.0*3.0);
uniform float FXAA_REDUCE_MUL = 1.0/(8.0*2.0);

void main()
{ 
	vec2 inverseScreenTextureSize = 1.0 / textureSize(mainTex, 0);

	vec3 luma = vec3(0.299, 0.587, 0.114);
	//get the luminosity values of the near texels
	float lumaTL = dot(luma, texture(mainTex, TexCoords + vec2(-1.0, -1.0)*inverseScreenTextureSize).xyz);
	float lumaTR = dot(luma, texture(mainTex, TexCoords + vec2(1.0, -1.0)*inverseScreenTextureSize).xyz);
	float lumaBL = dot(luma, texture(mainTex, TexCoords + vec2(-1.0, 1.0)*inverseScreenTextureSize).xyz);
	float lumaBR = dot(luma, texture(mainTex, TexCoords + vec2(1.0, 1.0)*inverseScreenTextureSize).xyz);
	float lumaM = dot(luma, texture(mainTex, TexCoords).xyz);
	
	vec2 dir;
	dir.x = -((lumaTL + lumaTR) - (lumaBL + lumaBR));
	dir.y = (lumaTL + lumaBL) - (lumaTR + lumaBR);
	
	//scale it so the smallest component has a length of 1
	float dirReduce = max((lumaTL + lumaTR + lumaBL + lumaBR) * 0.25 * FXAA_REDUCE_MUL, FXAA_REDUCE_MIN);
	float inverseDirAdj = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);

	dir = min(vec2(FXAA_SPAN_MAX, FXAA_SPAN_MAX),
		max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX), dir * inverseDirAdj)) * inverseScreenTextureSize;
	//dir = clamp(dir * inverseDirAdj, -FXAA_SPAN_MAX, FXAA_SPAN_MAX) * inverseScreenTextureSize;
	
	vec3 result1 = 0.5 * (texture(mainTex, TexCoords + dir * vec2(1.0/3.0 - 0.5)).xyz +
		texture(mainTex, TexCoords + dir * vec2(2.0/3.0 - 0.5)).xyz);
	
	vec3 result2 = result1 * 0.5 + 0.25 * (texture(mainTex, TexCoords + dir * vec2(0.0/3.0 - 0.5)).xyz +
		texture(mainTex, TexCoords + dir * vec2(3.0/3.0 - 0.5)).xyz);
		
	float lumaMin = min(lumaM, min(min(lumaTL, lumaTR), min(lumaBL, lumaBR)));
	float lumaMax = max(lumaM, max(max(lumaTL, lumaTR), max(lumaBL, lumaBR)));
	float lumaResult2 = dot(luma, result2);
	
	if (lumaResult2 < lumaMin || lumaResult2 > lumaMax)
		FragColor = vec4(result1, 1.0);
	else FragColor = vec4(result2, 1.0);
}
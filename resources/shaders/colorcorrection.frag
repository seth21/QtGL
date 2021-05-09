#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D mainTex;
uniform float saturation;
uniform float contrast;
uniform float brightness;
uniform float exposure;

//1 is normal
vec3 changeSaturation(vec3 original, float amount){
	vec3 color = original;
	float median =  0.2126 * original.r + 0.7152 * original.g + 0.0722 * original.b;
	color.r = median + (color.r - median) * amount;
	color.g = median + (color.g - median) * amount;
	color.b = median + (color.b - median) * amount;
	return color;
}

//1 is normal
vec3 changeContrast(vec3 original, float amount){
	vec3 color = original * amount;
	return color;
}

void main()
{ 
	const float gamma = 2.2;
    vec3 color = texture(mainTex, TexCoords).rgb;
	//color correction
	color = changeContrast(color, contrast);
	color = changeSaturation(color, saturation);
	color += brightness;
	// exposure tone mapping
    vec3 mapped = vec3(1.0) - exp(-color * exposure);
    // gamma correction -> to sRGB space
    mapped = pow(mapped, vec3(1.0 / gamma));
	

    FragColor = vec4(mapped, 1.0);
}
#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D mainTexture;
uniform float horizontal;

uniform float offset[3] = float[]( 0.0, 1, 2 );
uniform float weight[3] = float[]( 0.35, 0.2, 0.125 );

void main(void){
	vec2 tex_offset = 1.0 / textureSize(mainTexture, 0); // gets size of single texel
    vec3 result = texture(mainTexture, TexCoords).rgb * weight[0]; // current fragment's contribution
    
    if (horizontal > 0.5){
        for(int i = 1; i < 3; ++i)
        {
            result += texture(mainTexture, TexCoords + vec2(tex_offset.x * offset[i], 0.0)).rgb * weight[i];
            result += texture(mainTexture, TexCoords - vec2(tex_offset.x * offset[i], 0.0)).rgb * weight[i];
        }
   }
   else{
  		for(int i = 1; i < 3; ++i)
        {
            result += texture(mainTexture, TexCoords + vec2(0.0, tex_offset.y * offset[i])).rgb * weight[i];
            result += texture(mainTexture, TexCoords - vec2(0.0, tex_offset.y * offset[i])).rgb * weight[i];
        }

   }
    FragColor = vec4(result, 1.0);
}

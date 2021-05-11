#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D mainTexture;
uniform float horizontal;
uniform sampler2D sceneTex;
uniform float final;

uniform float offset[3] = float[]( 0.0, 1.3846153846, 3.2307692308 );
uniform float weight[3] = float[]( 0.2270270270, 0.3162162162, 0.0702702703 );

void main(void){
	vec2 tex_offset = 1.0 / textureSize(mainTexture, 0); // gets size of single texel
    vec4 result = texture(mainTexture, TexCoords) * weight[0]; // current fragment's contribution
    
    if (horizontal > 0.5){
        for(int i = 1; i < 3; ++i)
        {
            result += texture(mainTexture, TexCoords + vec2(tex_offset.x * offset[i], 0.0)) * weight[i];
            result += texture(mainTexture, TexCoords - vec2(tex_offset.x * offset[i], 0.0)) * weight[i];
        }
   }
   else{
  		for(int i = 1; i < 3; ++i)
        {
            result += texture(mainTexture, TexCoords + vec2(0.0, tex_offset.y * offset[i])) * weight[i];
            result += texture(mainTexture, TexCoords - vec2(0.0, tex_offset.y * offset[i])) * weight[i];
        }

   }
    FragColor = result;

    if (final > 0.5) FragColor = vec4(mix(texture(sceneTex, TexCoords).rgb, result.rgb, result.a), 1);
}

#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D mainTexture;
uniform vec2 direction;
uniform vec2 texelSize;
//uniform sampler2D distTex;
uniform sampler2D sceneTex;
uniform float final;

uniform float offset[3] = float[]( 0.0, 1.3846153846, 3.2307692308 );
uniform float weight[3] = float[]( 0.2270270270, 0.3162162162, 0.0702702703 );

void main(void){
    vec4 result = texture(mainTexture, TexCoords) * weight[0]; // current fragment's contribution
    //float rayDist = texture(distTex, TexCoords).r;
    //vec2 dir = direction * rayDist;
    for(int i = 1; i < 3; ++i)
    {
        result += texture(mainTexture, TexCoords + texelSize * offset[i] * direction, 0.0) * weight[i];
        result += texture(mainTexture, TexCoords - texelSize * offset[i] * direction, 0.0) * weight[i];
    }
  
    FragColor = result;

    if (final > 0.5) FragColor = vec4(mix(texture(sceneTex, TexCoords).rgb, result.rgb, result.a), 1);
}

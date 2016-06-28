#version 400

in vec3 Color;

out vec4 FragColor;
uniform sampler2D s;

in VS_OUT{
	vec2 cd;
}vs_out;



void main()
{
	FragColor = vec4(Color,1.0);
	
}
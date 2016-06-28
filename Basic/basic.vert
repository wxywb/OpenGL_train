
#version 430

layout (location  = 0) in vec3 VertexPosition  ;
layout (location  = 1) in vec3 VertexColor ;
layout (location  = 2) in vec2 TexCoord ;

layout(std140) uniform TranformBlock{
	float a;
	float b;
}transform;
out vec3 Color;
uniform sampler2D s;
uniform float aie ;
uniform vec3 yea;
uniform mat4x4 TransMat;
uniform mat4x4 projMat;

out VS_OUT{
	vec2 cd;
}vs_out;


void main()
{
	Color =vec3(aie,aie,aie);
	Color = yea;
	float test = TransMat[3][0];
	Color =vec3(1.0f,1.0f,1.0f);
	vs_out.cd = TexCoord;

	gl_Position = projMat * TransMat*vec4(VertexPosition,1.0);


}
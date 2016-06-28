#pragma comment(lib,"glew32.lib")
#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include <vmath.h>
#include <fstream>  
#include<sstream>
#include<vector>

using namespace std;

GLuint vShader, fShader;//顶点着色器对象
float positionData[] = {
	-0.5, 0.5, 0,
	0.5, 0.5, 0,
	0.5, -0.5, 0,
	-0.5, -0.5, 0,

};
GLuint vaoHandle;//vertex array object
GLuint programHandle;
void initShader(const char *VShaderFile, const char *FShaderFile)
{

	static const char * vs_source[] =
	{
		"#version 420 core                                                  \n"
		"layout (location = 0) in vec4 position;						    \n"
		"out vec4 position2;												\n"
		"uniform mat4x4 TransMat;											\n"
		"void main(void)                                                    \n"
		"{                                                                  \n"
		"	vec4 temp = vec4(0.2,0.2,0.2,0.0);    							\n"
		"                                                                   \n"
		"    gl_Position =  position + temp;							    \n"
		"	 position2   =  position + 0.8;									\n"
		//	"	 gl_Position = position2;										\n"
		"}                                                                 \n"
	};

	static const char * tcs_source_quads[] =
	{
		"#version 420 core                                                                   \n"
		"                                                                                    \n"
		"layout (vertices = 16) out;                                                          \n"
		"void main(void)                                                                     \n"
		"{                                                                                   \n"
		"    if (gl_InvocationID == 0)                                                       \n"
		"    {                                                                               \n"
		"        gl_TessLevelInner[0] = 4.0;                                                 \n"
		"        gl_TessLevelInner[1] = 4.0;                                                 \n"
		"        gl_TessLevelOuter[0] = 4.0;                                                 \n"
		"        gl_TessLevelOuter[1] = 4.0;                                                 \n"
		"        gl_TessLevelOuter[2] = 4.0;                                                 \n"
		"        gl_TessLevelOuter[3] = 4.0;                                                 \n"
		"    }                                                                               \n"
		"	 	 																			 \n"
		"    gl_out[gl_InvocationID].gl_Position =gl_in[gl_InvocationID].gl_Position;        \n"
		"}                                                                                   \n"
	};

	static const char * tes_source_quads[] =
	{
		"#version 420 core                                                                    \n"
		"                                                                                     \n"
		"layout (quads,equal_spacing) in;                                                                   \n"
		"                                                                                     \n"
		"void main(void)                                                                      \n"
		"{                                                                                    \n"
		"   vec4 p1 = mix(gl_in[0].gl_Position, gl_in[3].gl_Position, gl_TessCoord.x);    \n"
		"   vec4 p2 = mix(gl_in[12].gl_Position, gl_in[15].gl_Position, gl_TessCoord.x);    \n"
		"    gl_Position = mix(p1, p2, gl_TessCoord.y);                                   \n"
		"	gl_Position = vec4(gl_TessCoord.x , gl_TessCoord.y , 1.0, 1.0); \n"
		//	"  gl_in[2].gl_Position=gl_in[3].gl_Position;\n"
		//		" int i =12;\n"
		//	"	   gl_Position =vec4(gl_TessCoord.x+gl_in[i].gl_Position.x,gl_TessCoord.y+gl_in[i].gl_Position.y,1.0,1.0);\n"
		//"	   gl_Position =vec4(gl_TessCoord.x+0.5,gl_TessCoord.y-0.5,1.0,1.0);\n"
		"}                                                                                    \n"
	};




	static const char * fs_source[] =
	{
		"#version 420 core                                                  \n"

		"in vec3 color;														\n"
		"out vec4 color2;                                                   \n"
		"                                                                   \n"
		"void main(void)                                                    \n"
		"{                                                                  \n"
		"    color2 = vec4(1.0,0.0,0.0,0.0);                                            \n"
		"	 																\n"
		"}                                                                  \n"
	};
	programHandle = glCreateProgram();
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, vs_source, NULL);
	glCompileShader(vs);

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, fs_source, NULL);
	glCompileShader(fs);

	GLuint tcs = glCreateShader(GL_TESS_CONTROL_SHADER);
	glShaderSource(tcs, 1, tcs_source_quads, NULL);
	glCompileShader(tcs);

	GLuint tes = glCreateShader(GL_TESS_EVALUATION_SHADER);
	glShaderSource(tes, 1, tes_source_quads, NULL);
	glCompileShader(tes);

	glAttachShader(programHandle, vs);
	//glAttachShader(programHandle, tcs);
	//glAttachShader(programHandle, tes);
	glAttachShader(programHandle, fs);
	glLinkProgram(programHandle);
	glUseProgram(programHandle);

	GLuint projMat = glGetUniformLocation(programHandle, "projMat");
	GLuint TransMat = glGetUniformLocation(programHandle, "TransMat");
	vmath::mat4 TransMatrix = vmath::lookat(vmath::vec3(0.0f, 0.0f, 0.0f),
		vmath::vec3(0.02f, 0.0f, -1.0f),
		vmath::vec3(0.0f, 1.0f, 0.0f)
		)*
		vmath::translate(0.0f, 0.0f, 0.0f) *
		vmath::rotate(0.0f, 0.0f, 0.0f, 0.0f) *
		vmath::mat4::identity();
	glUniformMatrix4fv(TransMat, 1, GL_FALSE, TransMatrix);
}

void initVBO()
{
	//OBJspace::OBJreader sore("D:\\sore.ob2j");
	GLuint vboHandles[2];
	glGenBuffers(2, vboHandles);
	GLuint positionBufferHandle = vboHandles[0];
	GLuint colorBufferHandle = vboHandles[1];
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), positionData, GL_STATIC_DRAW);
	
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//GLuint m_transformFeedback;
	//glGenTransformFeedbacks(1, &m_transformFeedback);
	const char * var[] = { "position2" };
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, buffer);
	glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, sizeof(positionData), nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, buffer);
	glTransformFeedbackVaryings(programHandle,
		1,
		var,
		GL_INTERLEAVED_ATTRIBS);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, buffer);
	//glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, m_transformFeedback);
	glLinkProgram(programHandle);
	glUseProgram(programHandle);
}

void init()
{
	//初始化glew扩展库
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cout << "Error initializing GLEW: " << glewGetErrorString(err) << endl;
	}
	initShader("basic.vert", "basic.frag");
	initVBO();
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(vaoHandle);
	cout << "hey" << endl;
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, 4);
	glEndTransformFeedback();
		void * ptr = glMapBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, GL_READ_ONLY);	float *az = new float[100];
	memcpy(az, ptr, 3*4*sizeof(float));
	for (int i = 0; i < 3 * 4 ; i++){
		cout << az[i] << endl;
	}
	cout << "hey2" << endl;

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		glDeleteShader(vShader);
		glUseProgram(0);
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("GLSL Test : Draw a triangle");
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}


#pragma comment(lib,"glew32.lib")
#include <GL/glew.h>
#include "textfile.h"
#include <GL/glut.h>
#include <iostream>
#include <vmath.h>
#include"wxymath.h"
#include <fstream>  
#include<sstream>
#include<vector>
#include"OBJreader.h"
using namespace std;

GLuint vShader, fShader;//顶点着色器对象


static const GLubyte chess_data[] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0xFF,
	0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,
	0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
	0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF
};

static const GLubyte chess_data2[] = {
	0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0x00,
	0x00, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF,
	0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0xFF,
	0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00,
	0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF
};

static const GLubyte rgb_data[] = {
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f
};



float positionData[] = {
	-0.2f, 0.2, -1.5f,
	-0.2f, -0.2f, -1.5f,
	0.2f, -0.2f, -1.5f,
	0.2f, 0.2f, -1.5f,

	0.21f, 0.2, -1.5f,
	0.21f, -0.2f, -1.5f,
	0.6f, -0.2f, -1.5f,
	0.6f, 0.2f, -1.5f,

	




};

float colorData[] = {
	1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f };
const GLuint indices[] = {
	0, 1, 2, 3, 4, 5, 6
};



GLuint vaoHandle;//vertex array object
GLuint programHandle;
void initShader(const char *VShaderFile, const char *FShaderFile)
{
	//1、查看GLSL和OpenGL的版本
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion =
	glGetString(GL_SHADING_LANGUAGE_VERSION);
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	cout << "GL Vendor    :" << vendor << endl;
	cout << "GL Renderer  : " << renderer << endl;
	cout << "GL Version (string)  : " << version << endl;
	cout << "GL Version (integer) : " << major << "." << minor << endl;
	cout << "GLSL Version : " << glslVersion << endl;

	//2、编译着色器
	//创建着色器对象：顶点着色器
	vShader = glCreateShader(GL_VERTEX_SHADER);
	//错误检测
	if (0 == vShader)
	{
		cerr << "ERROR : Create vertex shader failed" << endl;
		exit(1);
	}

	//把着色器源代码和着色器对象相关联
	const GLchar *vShaderCode = textFileRead(VShaderFile);
	const GLchar *vCodeArray[1] = { vShaderCode };
	glShaderSource(vShader, 1, vCodeArray, NULL);

	//编译着色器对象
	glCompileShader(vShader);


	//检查编译是否成功
	GLint compileResult;
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &compileResult);
	if (GL_FALSE == compileResult)
	{
		GLint logLen;
		//得到编译日志长度
		glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char *log = (char *)malloc(logLen);
			GLsizei written;
			//得到日志信息并输出
			glGetShaderInfoLog(vShader, logLen, &written, log);
			cerr << "vertex shader compile log : " << endl;
			cerr << log << endl;
			free(log);//释放空间
		}
	}

	//创建着色器对象：片断着色器
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	//错误检测
	if (0 == fShader)
	{
		cerr << "ERROR : Create fragment shader failed" << endl;
		exit(1);
	}

	//把着色器源代码和着色器对象相关联
	const GLchar *fShaderCode = textFileRead(FShaderFile);
	const GLchar *fCodeArray[1] = { fShaderCode };
	glShaderSource(fShader, 1, fCodeArray, NULL);

	//编译着色器对象
	glCompileShader(fShader);

	//检查编译是否成功
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &compileResult);
	if (GL_FALSE == compileResult)
	{
		GLint logLen;
		//得到编译日志长度
		glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char *log = (char *)malloc(logLen);
			GLsizei written;
			//得到日志信息并输出
			glGetShaderInfoLog(fShader, logLen, &written, log);
			cerr << "fragment shader compile log : " << endl;
			cerr << log << endl;
			free(log);//释放空间
		}
	}

	//3、链接着色器对象
	//创建着色器程序
	programHandle = glCreateProgram();
	if (!programHandle)
	{
		cerr << "ERROR : create program failed" << endl;
		exit(1);
	}
	//将着色器程序链接到所创建的程序中
	glAttachShader(programHandle, vShader);
	glAttachShader(programHandle, fShader);
	//将这些对象链接成一个可执行程序
	glLinkProgram(programHandle);
	//查询链接的结果
	GLint linkStatus;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &linkStatus);
	if (GL_FALSE == linkStatus)
	{
		cerr << "ERROR : link shader program failed" << endl;
		GLint logLen;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH,
			&logLen);
		if (logLen > 0)
		{
			char *log = (char *)malloc(logLen);
			GLsizei written;
			glGetProgramInfoLog(programHandle, logLen,
				&written, log);
			cerr << "Program log : " << endl;
			cerr << log << endl;
		}
	}
	else//链接成功，在OpenGL管线中使用渲染程序
	{
		glUseProgram(programHandle);
	}
}
float times = 0;
GLuint chess;
GLuint chess2;
void initVBO()
{
	OBJspace::OBJreader sore("D:\\sore.ob2j");
	GLuint vboHandles[2];
	glGenBuffers(2, vboHandles);
	GLuint positionBufferHandle = vboHandles[0];
	GLuint colorBufferHandle = vboHandles[1];

	glGenVertexArrays(1, &vaoHandle);

	glBindVertexArray(vaoHandle);


	GLuint index;
	glGenBuffers(1, &index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), positionData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	//glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), colorData, GL_STATIC_DRAW);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);


	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	GLuint aie = glGetUniformLocation(programHandle, "aie");
	glUniform1f(aie, 0.8f);

	GLuint yea = glGetUniformLocation(programHandle, "yea");
	glUniform3f(yea, 0.0f, 1.0f, 0.0f);

	auto znear = 0.5f;
	auto zfar = 1.0f;
	auto width = 1000.0f;
	auto height = 1000.0f;
	auto angle = 90;

	auto Emat = vmath::mat4::identity();
	Emat[0][0] = 1.0f / tanh(angle / 2);
	Emat[1][1] = 1.0f / tanh(angle / 2);
	Emat[2][2] = (zfar + znear) / (zfar - znear);
	Emat[2][3] = (-2 * zfar*znear) / (zfar - znear);
	Emat[3][2] = 1.0f;
	Emat[3][3] = 0.0f;
	//Emat = vmath::mat4::identity();
	Emat = Emat.transpose();

	auto fuck = vmath::frustum(-0.5, 0.5, -0.5, 0.5, 0.4, 155);

	//auto fuck2 = vmath::lookat
	vmath::vec4 d(1.0f, 0.0f, 0.0f, 0.0f);


	//times++;
	GLuint TransMat = glGetUniformLocation(programHandle, "TransMat");
	vmath::mat4 Emat2 = vmath::lookat(vmath::vec3(0.0f, 0.0f, 0.0f),
		vmath::vec3(-0.5f, 0.0f, -1.0f),
		vmath::vec3(0.0f, 1.0f, 0.0f)
		)*
		vmath::translate(0.0f, 0.0f, 0.0f) *
		vmath::rotate(0.0f, 0.0f, 0.0f, 0.0f) *
		vmath::mat4::identity();
	glUniformMatrix4fv(TransMat, 1, GL_FALSE, Emat2);


	//vmath::mat4 proj_matrix = vmath::frustum();

	GLuint projMat = glGetUniformLocation(programHandle, "projMat");

	glUniformMatrix4fv(projMat, 1, GL_FALSE, fuck);



	//glEnable(GL_SCISSOR_TEST);

	//////////Transform feedback;
	
	glGenTextures(1, &chess);
	glBindTexture(GL_TEXTURE_2D, chess);
	glTexStorage2D(GL_TEXTURE_2D, 4, GL_R8, 8, 8);
	glTexSubImage2D(GL_TEXTURE_2D,
		0,
		0, 0,
		8, 8,
		GL_RED, GL_UNSIGNED_BYTE,
		chess_data);

	
	glGenTextures(1, &chess2);
	glBindTexture(GL_TEXTURE_2D, chess2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexStorage2D(GL_TEXTURE_2D, 4, GL_R8, 8, 8);
	glTexSubImage2D(GL_TEXTURE_2D,
		0,
		0, 0,
		8, 8,
		GL_RED, GL_UNSIGNED_BYTE,
		chess_data2);
	glBindTexture(GL_TEXTURE_2D, chess);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	float tex_coord[] = {
		0.0, 0.0,
		0.0, 1.0,
		1.0, 1.0,
		1.0, 0.0,
		0.0, 0.0,
		0.0, 1.0,
		1.0, 1.0,
		1.0, 0.0,


	};
	GLuint coordHandle;

	glGenBuffers(1, &coordHandle);
	glBindBuffer(GL_ARRAY_BUFFER, coordHandle);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), tex_coord, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);
	glEnableVertexAttribArray(2);



	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
	//glShadeModel(GL_SMOOTH);

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(vaoHandle);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
	glBindTexture(GL_TEXTURE_2D, chess);
	glDrawArrays(GL_QUADS, 0, 4);
	glBindTexture(GL_TEXTURE_2D, chess2);
	glDrawArrays(GL_QUADS, 4, 4);



//	cout << times << endl;
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
	//cout << int(key) <<x<< endl;
}

void mouse(int button, int state, int x, int y){
	//	cout << button << ' ' << state << ' ' << x << ' ' << y << endl;
}

float chichi = -0.5f;
void mouse2(int x, int y){
	float xoffset, yoffset = 0;
	if(x < 300){
		xoffset = -0.01;
	}
	else{
		xoffset = 0.01;
	}

	if (y < 300){
		yoffset = -0.01;
	}
	else{
		yoffset = 0.01;
	}
	chichi = chichi + xoffset;
	//cout << chichi << endl;
	x = 0;
	y = 0;
	GLuint TransMat = glGetUniformLocation(programHandle, "TransMat");
	vmath::mat4 Emat2 = vmath::lookat(vmath::vec3(0.0f, 0.0f, 0.0f),
		vmath::vec3(chichi, 0.0f, -1.0f),
		vmath::vec3(0.0f, 1.0f, 0.0f)
		)*
		vmath::translate(0.0f, 0.0f, 0.0f) *
		vmath::rotate(0.0f, 0.0f, 0.0f, 0.0f) *
		vmath::mat4::identity();
	glUniformMatrix4fv(TransMat, 1, GL_FALSE, Emat2);
	display();
}
void wxyIDLE(){
	//cout << times<< endl;
//	glutPassiveMotionFunc(int x, int y);
//	cout << x << ' ' << y << endl;
}


#define TEST3

#ifndef TEST
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("GLSL Test : Draw a triangle");
	init();
	glutDisplayFunc(display);
	glutIdleFunc(wxyIDLE);
	glutKeyboardFunc(keyboard);
	//glutMouseFunc(mouse);
	glutPassiveMotionFunc(mouse2);
	glutMainLoop();

	return 0;
}
#endif


#ifdef TEST

int main(){
	vector<string> ttt;
	string src = "f 4 3 11 9 ";
	string d = " ";
	OBJspace::OBJreader obj("D:\\sore.ob2j");


}



#endif
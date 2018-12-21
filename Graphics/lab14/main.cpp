#include<Windows.h>    
// first include Windows.h header file which is required    
#include<stdio.h>
#include "GL/glew.h"
#include<gl/GL.h>   // GL.h header file    
#include<gl/GLU.h> // GLU.h header file    
#include<gl/glut.h>  // glut.h header file from freeglut\include\GL folder    
#include<conio.h>
#include<math.h>
#include<string>
#include <vector>
#include <glm/glm.hpp>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include <SOIL.h>
#include <glm/gtc/matrix_transform.hpp>

unsigned char* image;
GLuint texture1;
int width = 0, height = 0;
//1 - ��� ������� �� ������ ����� ����� ��������� ������
//2 - ��� ������� �� ������ ����� ����� ����������� ������
//3 - � ���������� �� ������ ����� ����� ��������� ������
//4 - � ���������� �� ������ ����� ����� ����������� ������
int mode = 1;
GLuint Program1, Program2, Program3, Program4;
GLint Unif1, Unif2;
GLuint VBO, VAO, IBO;
std::vector<GLushort> indices;

//transform
glm::mat4 model, viewProjection;
glm::mat3 normaltr;
float viewPosition[]{ 0,0,-5 };
//light
float light_angle = 0, light_pos = 5, light_rad = 10;
glm::vec3 light{ 0, 5, 0 };
glm::vec4 ambient{ 0.2f,0.2f,0.2f,1.0f };
glm::vec4 diffuse{ 1.0f,1.0f,1.0f,1.0f };
glm::vec4 specular{ 1.0f,1.0f,1.0f,1.0f };
glm::vec3 attenuation{ 1.0f,0.0f,0.0f };

void makeTextureImage()
{
	texture1 = SOIL_load_OGL_texture
	(
		"penguin.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void checkOpenGLerror() {
	GLenum errCode;
	if ((errCode = glGetError()) != GL_NO_ERROR)
		std::cout << "OpenGl error! - " << errCode << gluErrorString(errCode);
}

std::string readfile(const char* path)
{
	std::string res = "";
	std::ifstream file(path);
	std::string line;
	getline(file, res, '\0');
	while (getline(file, line))
	{
		res += "\n " + line;
	}
	return res;
}

//��� ������� �� ������ ����� ����� ��������� ������ - ������������ ����
void initShader1()
{
	std::string readed = readfile("vertex1.shader");
	const char* vsSource = readed.c_str();

	std::string readed2 = readfile("fragment13.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	Program1 = glCreateProgram();
	glAttachShader(Program1, vShader);
	glAttachShader(Program1, fShader);
	glLinkProgram(Program1);

	int link_ok;
	glGetProgramiv(Program1, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(Program1, 512, &size, infoLog);
		std::cout << infoLog;
		return;
	}

	Unif1 = glGetUniformLocation(Program1, "objColor");
	if (Unif1 == -1)
	{
		std::cout << "could not bind uniform objColor(1)" << std::endl;
		checkOpenGLerror();
		return;
	}

	checkOpenGLerror();
}

// ��� ������� �� ������ ����� ����� ����������� ������ - ������������ �����
void initShader2()
{
	std::string readed = readfile("vertex24.shader");
	const char* vsSource = readed.c_str();

	std::string readed2 = readfile("fragment2.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	Program2 = glCreateProgram();
	glAttachShader(Program2, vShader);
	glAttachShader(Program2, fShader);
	glLinkProgram(Program2);

	int link_ok;
	glGetProgramiv(Program2, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(Program2, 512, &size, infoLog);
		std::cout << infoLog;
		return;
	}
	Unif2 = glGetUniformLocation(Program2, "objColor");
	if (Unif2 == -1)
	{
		std::cout << "could not bind uniform objColor(2)" << std::endl;
		checkOpenGLerror();
		return;
	}
	checkOpenGLerror();
}

//� ���������� �� ������ ����� ����� ��������� ������ - ������������ ����
void initShader3()
{
	std::string readed = readfile("vertex3.shader");
	const char* vsSource = readed.c_str();

	std::string readed2 = readfile("fragment13.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	Program3 = glCreateProgram();
	glAttachShader(Program3, vShader);
	glAttachShader(Program3, fShader);
	glLinkProgram(Program3);

	int link_ok;
	glGetProgramiv(Program3, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(Program3, 512, &size, infoLog);
		std::cout << infoLog;
		return;
	}

	checkOpenGLerror();
}

//� ���������� �� ������ ����� ����� ��������� ������ - ������������ ����
void initShader4()
{
	std::string readed = readfile("vertex24.shader");
	const char* vsSource = readed.c_str();

	std::string readed2 = readfile("fragment4.shader");
	const char* fsSource = readed2.c_str();

	GLuint vShader, fShader;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vsSource, NULL);
	glCompileShader(vShader);

	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fsSource, NULL);
	glCompileShader(fShader);

	Program4 = glCreateProgram();
	glAttachShader(Program4, vShader);
	glAttachShader(Program4, fShader);
	glLinkProgram(Program4);

	int link_ok;
	glGetProgramiv(Program4, GL_LINK_STATUS, &link_ok);
	if (!link_ok)
	{
		std::cout << "error attach shaders \n";
		GLchar infoLog[512];
		GLint size;
		glGetProgramInfoLog(Program4, 512, &size, infoLog);
		std::cout << infoLog;
		return;
	}

	checkOpenGLerror();
}

void initShaders() {
	initShader1();
	initShader2();
	initShader3();
	initShader4();
}
void freeShader()
{
	glUseProgram(0);
	glDeleteProgram(Program1);
	glDeleteProgram(Program2);
	glDeleteProgram(Program3);
	glDeleteProgram(Program4);
}

void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_DEPTH_TEST);
}

void reshape(int w, int h)
{
	width = w; height = h;
	glViewport(0, 0, w, h);
}

void initBuffers()
{
	GLfloat vertices[] = {
		// �������          // normals             // ���������� ����������
		 0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,   // ������� ������
		 0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,   // ������ ������
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ������ �����
		-0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f    // ������� �����
	};

	indices.assign({ 0,1,2,3 });

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &IBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &indices[0], GL_STATIC_DRAW);

	//pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	//texcoords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void freeBuffers()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &IBO);


	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}


const double pi = 3.14159265358979323846;
void recountLightPos()
{
	double x = light_rad * glm::cos(light_angle / 180 * pi);
	double z = light_rad * glm::sin(light_angle / 180 * pi);
	light.x = x;
	light.y = light_pos;
	light.z = z;
}


void setTransform()
{
	/*
	uniform struct Transform{
	mat4 model;
	mat4 viewProjection;
	mat3 normal;
	vec3 viewPosition;
	} transform;
	*/
	GLuint shader_program = Program1;
	if (mode == 2)
		shader_program = Program2;
	else if (mode == 3) shader_program = Program3;
	else if (mode == 4) shader_program = Program4;

	GLint s_model = glGetUniformLocation(shader_program, "transform.model");
	GLint s_proj = glGetUniformLocation(shader_program, "transform.viewProjection");
	GLint s_normal = glGetUniformLocation(shader_program, "transform.normal");
	GLint s_view = glGetUniformLocation(shader_program, "transform.viewPosition");

	glUniformMatrix4fv(s_model, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(s_proj, 1, GL_FALSE, &viewProjection[0][0]);
	glUniformMatrix3fv(s_normal, 1, GL_FALSE, &normaltr[0][0]);
	glUniform3fv(s_view, 1, viewPosition);
}

void setPointLight()
{
	/*uniform struct PointLight {
		vec4 position;
		vec4 ambient;
		vec4 diffuse;
		vec4 specular;
		vec3 attenuation;
	} light;*/
}

void setMaterial()
{
	/*uniform struct Material {
		sampler2D texture;
		vec4 ambient;
		vec4 diffuse;
		vec4 specular;
		vec4 emission;
		float shiness;
	} material;*/
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	model = glm::mat4(1.0f);
	viewProjection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 100.0f);
	normaltr = glm::transpose(glm::inverse(model));
	
	float red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	if (mode == 1 || mode == 2)
	{
		if (mode == 1)
		{
			glUseProgram(Program1);
			glUniform4fv(Unif1, 1, red);
		}
		else
		{
			glUseProgram(Program2);
			glUniform4fv(Unif2, 1, red);
		}
	}
	else
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture1);
		if (mode == 3) glUseProgram(Program3);
		else glUseProgram(Program4);
	}

	setTransform();
	setPointLight();
	setMaterial();

	glBindVertexArray(VAO);
	glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);

	glUseProgram(0);
	checkOpenGLerror();
	if (glIsEnabled(GL_TEXTURE_2D))
		glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
}

//������������ ���������� ���������� ����� (�����������, �������� ��������� ������ ������ �����) � ������� ��������
void special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1: mode = 1;
		break;
	case GLUT_KEY_F2:
		mode = 2;
		break;
	case GLUT_KEY_F3:
		mode = 3;
		break;
	case GLUT_KEY_F4:
		mode = 4;
		break;
	case GLUT_KEY_UP: light_pos += 0.5; break;
	case GLUT_KEY_DOWN: light_pos -= 0.5; break;
	case GLUT_KEY_RIGHT: light_angle -= 3; break;
	case GLUT_KEY_LEFT: light_angle += 3; break;
	case GLUT_KEY_PAGE_UP: light_rad -= 0.5; break;
	case GLUT_KEY_PAGE_DOWN: light_rad += 0.5; break;
	}
	recountLightPos();
	glutPostRedisplay();
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Lab 12");

	GLenum glew_status = glewInit();
	if (GLEW_OK != glew_status) {
		std::cout << "Error: " << glewGetErrorString(glew_status) << "\n";   return 1;
	}

	if (!GLEW_VERSION_2_0)
	{
		std::cout << "No support for OpenGL 2.0 found\n";
		return 1;
	}
	initBuffers();
	makeTextureImage();
	initShaders();
	init();
	recountLightPos();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);

	glutMainLoop();

	freeShader();
	freeBuffers();
}
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <string>
#include<stdbool.h>
#include <Windows.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#pragma warning(disable:4996)
using namespace std;
using namespace glm;
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void make_vertexShaders();
void make_fragmentShaders();
void make_shaderProgram();
void InitBuffer();
void InitTexture(const char* filename,unsigned int texture);
char* filetobuf(const char* file);
GLint height, width;
GLuint shaderProgram;
GLuint vertexShader;
GLuint fragmentShader;
GLuint VAO_axis, VBO_axis;
GLuint black_vao, black_vbo[2];
GLvoid keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
const char* timerui[5];
void TimerFunction(int value);
void TimerFunction2(int value);
typedef struct obj {
	unsigned int texture;
	GLuint VAO, VBO_pos, VBO_normal, VBO_uv;
	std::vector< glm::vec3 > out_vertices;
	std::vector< glm::vec2 > out_uvs;
	std::vector< glm::vec3 > out_normals;
};
obj cam;
obj map;
obj char2;
obj char3;
obj char4;
obj balloon;
obj cube;
obj scene;
obj blank;
obj camera;
obj reset;
obj block;
bool loadOBJ(const char* path, vector< glm::vec3 >& vertices, vector< glm::vec2 >& uvs, vector< glm::vec3 >& normals);
GLvoid keyboard(unsigned char key, int x, int y);

bool res = loadOBJ("cam.obj", cam.out_vertices, cam.out_uvs, cam.out_normals);
bool res2 = loadOBJ("char2.obj", char2.out_vertices, char2.out_uvs, char2.out_normals);
bool res3 = loadOBJ("char3.obj", char3.out_vertices, char3.out_uvs, char3.out_normals);
bool res5 = loadOBJ("char4.obj", char4.out_vertices, char4.out_uvs, char4.out_normals);
bool res6 = loadOBJ("balloon.obj", balloon.out_vertices, balloon.out_uvs, balloon.out_normals);
bool res4 = loadOBJ("map10.obj", map.out_vertices, map.out_uvs, map.out_normals);
bool resblock = loadOBJ("cube.obj", block.out_vertices, block.out_uvs, block.out_normals);
GLint object = cam.out_vertices.size();
GLint object3 = char3.out_vertices.size();
GLint object2 = char2.out_vertices.size();
GLint object4 = map.out_vertices.size();
GLint object5 = char4.out_vertices.size();
GLint object6 = balloon.out_vertices.size();
GLint object7 = block.out_vertices.size();
int iWidth;
int iHeight;
int iChannels;
int bardir = 1;
//string timerui;
bool select = false;
bool isstart = false;
bool line = false;
bool isreset = false;
bool timerend = true;

// 갈색 곰 움직이게 해볼게요
static bool b_bear_select = false;
static bool b_bear_location = false;
static int b_bear_x, b_bear_y;

// 3D picking 관련 변수_mouse picking with ray casting
static float x, y, z;
glm::vec3 ray_nds = glm::vec3(x, y, z);
static float x_window, y_window;
static float x_view, y_view;
static double wx, wz, wy;
GLdouble projection[16];
GLdouble modelView[16];
GLint viewPort[4];
GLfloat zCursor, winX, winY;


//함수=====================================
void playscene(float cx, float cy, float cz, float dx, float dy, float dz,bool board);
void selectobj();
void hinttimer();

//bool=====================================
bool start = false;
bool barsys = false;
bool cameraclick = false;
int numtimer = 4;
bool none_hint = false;
GLfloat black[24][3] = {
{0.0,0.0,0.0},
{0.0,0.0,0.0},
{0.0,0.0,0.0},
{0.0,0.0,0.0},
{0.0,0.0,0.0},
{0.0,0.0,0.0},
{0.0,0.0,0.0},
{0.0,0.0,0.0},
{0.0,0.0,0.0},
{0.0,0.0,0.0},
{0.0,0.0,0.0},
{0.0,0.0,0.0},
{0.0,0.0,0.0},
{0.0,0.0,0.0},
{0.0,0.0,0.0},
{0.0,0.0,0.0},
{0.0,0.0,0.0},
{0.0,0.0,0.0},
{0.0,0.0,0.0},
{0.0,0.0,0.0}
};

GLfloat rect_p[24][3] = {
	//1		0~3
	{ -0.1, -0.1, -0.1},
	{ -0.1, 0.1, -0.1 },
	{ 0.1, -0.1, -0.1},
	{ 0.1, 0.1, -0.1},
	//2		4~7
	{ -0.1, 0.1, 0.1},
	{ -0.1, 0.1, -0.1 },
	{ 0.1, 0.1, 0.1},
	{ 0.1, 0.1, -0.1},
	//3		8~11
	{ -0.1, -0.1, 0.1},
	{ -0.1, 0.1, 0.1 },
	{ 0.1, -0.1, 0.1},
	{ 0.1, 0.1, 0.1},
	//4		12~15
	{ -0.1, -0.1, 0.1},
	{ -0.1, -0.1, -0.1 },
	{ 0.1, -0.1, 0.1},
	{ 0.1, -0.1, -0.1},
	//5		16~19
	{ 0.1, -0.1, -0.1},
	{ 0.1, 0.1, -0.1 },
	{ 0.1, -0.1, 0.1},
	{ 0.1, 0.1, 0.1},
	//6		20~23
	{ -0.1, -0.1, -0.1},
	{ -0.1, 0.1, -0.1 },
	{ -0.1, -0.1, 0.1},
	{ -0.1, 0.1, 0.1}
};
float scene_[]
{
-0.5f, -0.5f, 0.f, 0.0, 0.0, 1.0, 0.0, 0.0,
0.5f, -0.5f, 0.f, 0.0, 0.0, 1.0, 1.0, 0.0,
0.5f, 0.5f, 0.f, 0.0, 0.0, 1.0, 1.0, 1.0,
0.5f, 0.5f, 0.f, 0.0, 0.0, 1.0, 1.0, 1.0,
-0.5f, 0.5f, 0.f, 0.0, 0.0, 1.0, 0.0, 1.0,
-0.5f, -0.5f, 0.f, 0.0, 0.0, 1.0, 0.0, 0.0
};
//힌트 구조체
typedef struct Hint
{
	int num = 3;               //힌트 개수
	bool heart_[3] = { true,true,true };
	bool blank = false;
	bool hint_ = false;            //힌트 키 

};
Hint hint;

struct Element
{
	float x;
	float y;
	float z;
	float scalex;
	float scaley;
	float scalez;

};
Element blank_h;
Element bar;

struct Exboard
{
	int check[4][4];
	bool ischeck[4][4];
	int kindof[4];
	bool isselect[4][4];
};
Exboard ex;

float bufferx = 0;
float bufferz = 0;
float radius = 10.0;
float revolve3x = 0;
float revolve3z = 0;
float revolve2x = 0;
float revolve2z = 0;

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{ //--- 윈도우 생성하기

	timerui[4] = "num3ui.png";
	timerui[3] = "num3ui.png";
	timerui[2] = "num2ui.png";
	timerui[1] = "num1ui.png";
	timerui[0] = "blank1.png";
	height = 800;
	width = 1400;
	bar.scalex = 2.5;
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 0); // 윈도우의 위치 지정
	glutInitWindowSize(width, height); // 윈도우의 크기 지정
	glutCreateWindow("take a picture!");
	glewExperimental = GL_TRUE;
	glewInit();
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	make_shaderProgram(); //--- 세이더 프로그램 만들기
	InitBuffer();
	glutMouseFunc(Mouse);

	glutKeyboardFunc(keyboard);
	glutTimerFunc(500, TimerFunction, 1);
	if (timerend) {
		glutTimerFunc(1000, TimerFunction2, 1);
	}
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape);

	for (int i = 0; i < 5; ++i) {
		ex.kindof[i] = rand() % 4 + 1; //1~4
	}

	for (int q = 0; q < 5; ++q) {
		if (ex.kindof[q] == 1) {
			int k = rand() % 5;
			if (ex.ischeck[0][k] == false) {
				ex.check[0][k] = 1;
				ex.ischeck[0][k] = true;
			}
		}
		else {
			int a = 0;
			int b = 0;
			a = rand() % 5;
			b = rand() % 5;
			if (ex.ischeck[a][b] == true) {
				a = rand() % 5;
				b = rand() % 5;
			}

			ex.check[a][b] = ex.kindof[q];
			ex.ischeck[a][b] = true;
		}
	}

	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			cout << ex.ischeck[i][j] << " ";
		}
		cout << endl;
	}

	glutMainLoop(); // 이벤트 처리 시작 
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{

	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glUseProgram(shaderProgram);

	unsigned int modelLocation = glGetUniformLocation(shaderProgram, "modelTransform");

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
	projection = glm::translate(projection, glm::vec3(0.0, 0.0, -2.0));

	unsigned int projectionLocation = glGetUniformLocation(shaderProgram, "projectionTransform");//--- 버텍스 세이더에서 viewTransform 변수위치
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);

	unsigned int lightPosLocation = glGetUniformLocation(shaderProgram, "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, 13.0, 13.0, 12.0);

	int lightColorLocation = glGetUniformLocation(shaderProgram, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, 1.0, 1.0, 1.0);

	unsigned int viewPosLocation = glGetUniformLocation(shaderProgram, "viewPos"); //--- viewPos 값 전달: 카메라 위치
	glUniform3f(viewPosLocation, -5.0, 0.0, 0.0);

	if (cameraclick == false) {
		//play
		glViewport(50, 50, 750, 500);
		playscene(7.0f, 8.0f, -2.5f, 0.0f, 0.0, 0.0, true);


		if (start) {
			//현재 상황
			glViewport(width - 350, height - 550, 300, 200);
			playscene(3.1f, 1.0f, 0.0f,0.0,1.0,1.5, false);

			//예시(힌트)화면fdgdfg

			if (hint.blank == false) {

				glViewport(width - 350, height - 250, 300, 200);
				glm::vec3 cameraPos = glm::vec3(3.1f, 1.0f, 0.0f);
				glm::vec3 cameraDirection = glm::vec3(0.0, 1.0, 1.5f);
				glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
				glm::mat4 view = glm::mat4(1.0f);
				view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
				unsigned int viewLocation = glGetUniformLocation(shaderProgram, "viewTransform");
				glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
				{
					glm::mat4 Signx = glm::mat4(1.0f);
					glm::mat4 Signy = glm::mat4(1.0f);
					glm::mat4 Signz = glm::mat4(1.0f);
					glm::mat4 SignRx = glm::mat4(1.0f);
					glm::mat4 SignRy = glm::mat4(1.0f);
					glm::mat4 SignRz = glm::mat4(1.0f);
					glm::mat4 Signscale = glm::mat4(1.0f);
					glm::mat4 Sign = glm::mat4(1.0f);
					Signx = glm::translate(Signx, glm::vec3(2.5, 0.0, 0.0));
					Signy = glm::translate(Signy, glm::vec3(0.0, 12., 0.0));
					Signz = glm::translate(Signz, glm::vec3(0.0, 0.0, -2));
					SignRx = glm::rotate(SignRx, glm::radians(0.0f), glm::vec3(1.5, 0.0, 0.0));
					SignRy = glm::rotate(SignRy, glm::radians(200.0f), glm::vec3(0.0, 1.0, 0.0));
					SignRz = glm::rotate(SignRz, glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0));
					Signscale = glm::scale(Signscale, glm::vec3(16., 13., 16.));
					Sign = Signx * Signy * Signz * SignRx * SignRy * SignRz * Signscale;
					glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Sign));
					InitTexture("map10_I.png", map.texture);
					glBindTexture(GL_TEXTURE_2D, map.texture);
					glBindVertexArray(map.VAO);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glDrawArrays(GL_TRIANGLES, 0, object4);
				}

			}
			if (hint.blank) {
				{
					glViewport(width - 350, height - 250, 300, 200);
					glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 1.0);

					glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
					glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
					glm::mat4 view = glm::mat4(1.0f);

					view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
					unsigned int viewLocation = glGetUniformLocation(shaderProgram, "viewTransform");
					glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

					glm::mat4 camx = glm::mat4(1.0f);
					glm::mat4 camy = glm::mat4(1.0f);
					glm::mat4 camz = glm::mat4(1.0f);
					glm::mat4 SignRy = glm::mat4(1.0f);
					glm::mat4 camscale = glm::mat4(1.0f);
					glm::mat4 cam1 = glm::mat4(1.0f);
					SignRy = glm::rotate(SignRy, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
					camx = glm::translate(camx, glm::vec3(0.0, 0.0, 0.0));
					camy = glm::translate(camy, glm::vec3(0.0, 0.0, 0.0));
					camz = glm::translate(camz, glm::vec3(0.0, 0.0, 0.));
					camscale = glm::scale(camscale, glm::vec3(2.5, 2.5, 2.5));
					cam1 = camx * camy * camz * SignRy * camscale;
					glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cam1));
					InitTexture("icebear.png", scene.texture);
					glBindTexture(GL_TEXTURE_2D, scene.texture);
					glBindVertexArray(scene.VAO);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glDrawArrays(GL_TRIANGLES, 0, 6);

				}

			}
		}


		if (start == false) {
			{   //시작 화면 

				glViewport(0, 0, width, height);
				glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 1.0);

				glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
				glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
				glm::mat4 view = glm::mat4(1.0f);

				view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
				unsigned int viewLocation = glGetUniformLocation(shaderProgram, "viewTransform");
				glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

				glm::mat4 camx = glm::mat4(1.0f);
				glm::mat4 camy = glm::mat4(1.0f);
				glm::mat4 camz = glm::mat4(1.0f);
				glm::mat4 SignRy = glm::mat4(1.0f);
				glm::mat4 camscale = glm::mat4(1.0f);
				glm::mat4 cam1 = glm::mat4(1.0f);
				SignRy = glm::rotate(SignRy, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
				camx = glm::translate(camx, glm::vec3(0.0, 0.0, 0.0));

				camy = glm::translate(camy, glm::vec3(0.0, 0.0, 0.0));
				camz = glm::translate(camz, glm::vec3(0.0, 0.0, 0.));
				camscale = glm::scale(camscale, glm::vec3(2.5, 2.5, 2.5));
				cam1 = camx * camy * camz * SignRy * camscale;
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cam1));
				InitTexture("ss.png", scene.texture);
				glBindTexture(GL_TEXTURE_2D, scene.texture);
				glBindVertexArray(scene.VAO);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glDisable(GL_BLEND);

			}



		}

		{//시간UI

			glViewport(65, 15, 700, 30);
			glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 1.0);

			glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
			glm::mat4 view = glm::mat4(1.0f);

			view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
			unsigned int viewLocation = glGetUniformLocation(shaderProgram, "viewTransform");
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

			glm::mat4 camx = glm::mat4(1.0f);
			glm::mat4 camy = glm::mat4(1.0f);
			glm::mat4 camz = glm::mat4(1.0f);
			glm::mat4 SignRy = glm::mat4(1.0f);
			glm::mat4 camscale = glm::mat4(1.0f);
			glm::mat4 cam1 = glm::mat4(1.0f);
			SignRy = glm::rotate(SignRy, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
			camx = glm::translate(camx, glm::vec3(bar.x, 0.0, 0.0));
			camy = glm::translate(camy, glm::vec3(0.0, 0.0, 0.0));
			camz = glm::translate(camz, glm::vec3(0.0, 0.0, 0.));
			camscale = glm::scale(camscale, glm::vec3(bar.scalex, 2.5, 2.5));
			cam1 = camx * camy * camz * SignRy * camscale;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cam1));
			InitTexture("barui.png", scene.texture);
			glBindTexture(GL_TEXTURE_2D, scene.texture);
			glBindVertexArray(scene.VAO);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		if(none_hint==false){
			{
				//초 타이머
				glViewport(width - 250, height - 350, 100, 100);
				{glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 1.0);

				glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
				glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
				glm::mat4 view = glm::mat4(1.0f);

				view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
				unsigned int viewLocation = glGetUniformLocation(shaderProgram, "viewTransform");
				glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

				glm::mat4 camx = glm::mat4(1.0f);
				glm::mat4 camy = glm::mat4(1.0f);
				glm::mat4 camz = glm::mat4(1.0f);
				glm::mat4 SignRy = glm::mat4(1.0f);
				glm::mat4 camscale = glm::mat4(1.0f);
				glm::mat4 cam1 = glm::mat4(1.0f);
				SignRy = glm::rotate(SignRy, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
				camx = glm::translate(camx, glm::vec3(0.0, 0.0, 0.0));
				camy = glm::translate(camy, glm::vec3(0.0, 0.0, 0.0));
				camz = glm::translate(camz, glm::vec3(0.0, 0.0, 0.));
				camscale = glm::scale(camscale, glm::vec3(2.5, 2., 2.5));
				cam1 = camx * camy * camz * SignRy * camscale;
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cam1));
				InitTexture(timerui[numtimer], scene.texture);
				glBindTexture(GL_TEXTURE_2D, scene.texture);
				glBindVertexArray(scene.VAO);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				}
			}
		}
		{//UI 카메라 리....
			glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 1.0);
			glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
			glm::mat4 view = glm::mat4(1.0f);
			view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
			unsigned int viewLocation = glGetUniformLocation(shaderProgram, "viewTransform");
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
			glViewport(+300, 420, width, height / 2.0);
			{
				glm::mat4 camx = glm::mat4(1.0f);
				glm::mat4 camy = glm::mat4(1.0f);
				glm::mat4 camz = glm::mat4(1.0f);
				glm::mat4 SignRy = glm::mat4(1.0f);
				glm::mat4 camscale = glm::mat4(1.0f);

				glm::mat4 cam1 = glm::mat4(1.0f);
				SignRy = glm::rotate(SignRy, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
				camx = glm::translate(camx, glm::vec3(-0.5, 0.0, 0.0));
				camy = glm::translate(camy, glm::vec3(0.0, 0.0, 0.0));
				camz = glm::translate(camz, glm::vec3(0.0, 0.0, 0.));
				camscale = glm::scale(camscale, glm::vec3(0.2, 0.35, 0.1));
				cam1 = camx * camy * camz * SignRy * camscale;
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cam1));
				InitTexture("camera1.png", camera.texture);
				glBindTexture(GL_TEXTURE_2D, camera.texture);
				glBindVertexArray(scene.VAO);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			{
				glm::mat4 camx = glm::mat4(1.0f);
				glm::mat4 camy = glm::mat4(1.0f);
				glm::mat4 camz = glm::mat4(1.0f);
				glm::mat4 SignRy = glm::mat4(1.0f);
				glm::mat4 camscale = glm::mat4(1.0f);

				glm::mat4 cam1 = glm::mat4(1.0f);
				SignRy = glm::rotate(SignRy, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
				camx = glm::translate(camx, glm::vec3(-0.3, 0.0, 0.0));
				camy = glm::translate(camy, glm::vec3(0.0, 0.0, 0.0));
				camz = glm::translate(camz, glm::vec3(0.0, 0.0, 0.));
				camscale = glm::scale(camscale, glm::vec3(0.2, 0.3, 0.2));
				cam1 = camx * camy * camz * SignRy * camscale;
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cam1));
				InitTexture("reset.png", reset.texture);
				glBindTexture(GL_TEXTURE_2D, reset.texture);
				glBindVertexArray(scene.VAO);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			{
				glm::mat4 camx = glm::mat4(1.0f);
				glm::mat4 camy = glm::mat4(1.0f);
				glm::mat4 camz = glm::mat4(1.0f);
				glm::mat4 SignRy = glm::mat4(1.0f);
				glm::mat4 camscale = glm::mat4(1.0f);

				glm::mat4 cam1 = glm::mat4(1.0f);
				SignRy = glm::rotate(SignRy, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
				camx = glm::translate(camx, glm::vec3(-0.7, 0.0, 0.0));
				camy = glm::translate(camy, glm::vec3(0.0, 0.0, 0.0));
				camz = glm::translate(camz, glm::vec3(0.0, 0.0, 0.));
				camscale = glm::scale(camscale, glm::vec3(0.2, 0.5, 0.2));
				cam1 = camx * camy * camz * SignRy * camscale;
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cam1));
				InitTexture("hint.png", reset.texture);
				glBindTexture(GL_TEXTURE_2D, reset.texture);
				glBindVertexArray(scene.VAO);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
		}


		{//UI
			if (hint.num == 3 || hint.num == 2 || hint.num == 1) {

				glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 1.0);
				glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f);
				glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
				glm::mat4 view = glm::mat4(1.0f);
				view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
				unsigned int viewLocation = glGetUniformLocation(shaderProgram, "viewTransform");
				glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
				glViewport(-300, 420, width, height / 2.0);
				glUseProgram(shaderProgram);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				{
					glm::mat4 camx = glm::mat4(1.0f);
					glm::mat4 camy = glm::mat4(1.0f);
					glm::mat4 camz = glm::mat4(1.0f);
					glm::mat4 SignRy = glm::mat4(1.0f);
					glm::mat4 camscale = glm::mat4(1.0f);

					glm::mat4 cam1 = glm::mat4(1.0f);
					SignRy = glm::rotate(SignRy, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
					camx = glm::translate(camx, glm::vec3(-0.45, 0.0, 0.0));
					camy = glm::translate(camy, glm::vec3(0.0, 0.0, 0.0));
					camz = glm::translate(camz, glm::vec3(0.0, 0.0, 0.));
					camscale = glm::scale(camscale, glm::vec3(0.2, 0.35, 0.2));
					cam1 = camx * camy * camz * SignRy * camscale;
					glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cam1));
					InitTexture("heartui.png", scene.texture);
					glBindTexture(GL_TEXTURE_2D, scene.texture);
					glBindVertexArray(scene.VAO);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
			}
			if (hint.num == 2 || hint.num == 3)
			{
				glm::mat4 camx = glm::mat4(1.0f);
				glm::mat4 camy = glm::mat4(1.0f);
				glm::mat4 camz = glm::mat4(1.0f);
				glm::mat4 SignRy = glm::mat4(1.0f);
				glm::mat4 camscale = glm::mat4(1.0f);
				glm::mat4 cam1 = glm::mat4(1.0f);
				SignRy = glm::rotate(SignRy, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
				camx = glm::translate(camx, glm::vec3(-0.25, 0.0, 0.0));
				camy = glm::translate(camy, glm::vec3(0.0, 0.0, 0.0));
				camz = glm::translate(camz, glm::vec3(0.0, 0.0, 0));
				camscale = glm::scale(camscale, glm::vec3(0.2, 0.35, 0.2));
				cam1 = camx * camy * camz * SignRy * camscale;
				glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cam1));
				InitTexture("heartui.png", scene.texture);
				glBindTexture(GL_TEXTURE_2D, scene.texture);
				glBindVertexArray(scene.VAO);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}

			if (hint.num == 3) {
				{
					glm::mat4 camx = glm::mat4(1.0f);
					glm::mat4 camy = glm::mat4(1.0f);
					glm::mat4 camz = glm::mat4(1.0f);
					glm::mat4 SignRy = glm::mat4(1.0f);
					glm::mat4 camscale = glm::mat4(1.0f);
					glm::mat4 cam1 = glm::mat4(1.0f);
					SignRy = glm::rotate(SignRy, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
					camx = glm::translate(camx, glm::vec3(-0.05, 0.0, 0.0));
					camy = glm::translate(camy, glm::vec3(0.0, 0.0, 0.0));
					camz = glm::translate(camz, glm::vec3(0.0, 0.0, 0));
					camscale = glm::scale(camscale, glm::vec3(0.2, 0.35, 0.2));
					cam1 = camx * camy * camz * SignRy * camscale;
					glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cam1));
					InitTexture("heartui.png", scene.texture);
					glBindTexture(GL_TEXTURE_2D, scene.texture);
					glBindVertexArray(scene.VAO);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
			}

		}
	}



	if (cameraclick)
	{
		glViewport(50, 50, 500, 500);
		playscene(3.1f, 0.5f, 0.0f, 0.0, 0.5, 1.5, false);

		glViewport(600, 50, 500, 500);
		playscene(6.f, 1.0f, -1.5f, 2.0, 1.0f, 0.0, false);
	}
	glutSwapBuffers(); // 화면에 출력하기S


}
void playscene(float cx, float cy  , float cz , float dx , float dy,float dz,bool board)
{

	unsigned int modelLocation = glGetUniformLocation(shaderProgram, "modelTransform");
	//메인(플레이)화면
	{
		//메인 화면 카메라
		glm::vec3 cameraPos = glm::vec3(cx , cy , cz);

		//glm::vec3 cameraDirection = glm::vec3(-11.0f, -10.0f, 4.0f);
		glm::vec3 cameraDirection = glm::vec3(dx, dy, dz);
		glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
		unsigned int viewLocation = glGetUniformLocation(shaderProgram, "viewTransform");
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
		glUseProgram(shaderProgram);
		if(board){
		// 플레이 보드판
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {

				glm::mat4 TR5 = glm::mat4(1.0f);
				TR5 = glm::rotate(TR5, glm::radians(20.0f), glm::vec3(0.0, 1.0, 0.0));
				TR5 = glm::translate(TR5, glm::vec3(3.0f, 0.0f, 1.5f));
				TR5 = glm::translate(TR5, glm::vec3(-0.6 * j, 0.0, -0.6 * i));
				TR5 = glm::scale(TR5, glm::vec3(3.0, 0.1, 3.0));
				unsigned int tr5 = glGetUniformLocation(shaderProgram, "modelTransform");
				glUniformMatrix4fv(tr5, 1, GL_FALSE, glm::value_ptr(TR5));
				glBindVertexArray(black_vao);

				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				{
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
					glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
					glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
					glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
					glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
					glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
				}
			}
			}
		}
		{
			glm::mat4 Signx = glm::mat4(1.0f);
			glm::mat4 Signy = glm::mat4(1.0f);
			glm::mat4 Signz = glm::mat4(1.0f);
			glm::mat4 SignRx = glm::mat4(1.0f);
			glm::mat4 SignRy = glm::mat4(1.0f);
			glm::mat4 SignRz = glm::mat4(1.0f);
			glm::mat4 Signscale = glm::mat4(1.0f);
			glm::mat4 Sign = glm::mat4(1.0f);
			Signx = glm::translate(Signx, glm::vec3(0, 0.0, 0.0));
			Signy = glm::translate(Signy, glm::vec3(0.0, 0., 0.0));
			Signz = glm::translate(Signz, glm::vec3(0.0, 0.0, 0));
			SignRx = glm::rotate(SignRx, glm::radians(0.0f), glm::vec3(1.5, 0.0, 0.0));
			SignRy = glm::rotate(SignRy, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
			SignRz = glm::rotate(SignRz, glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0));
			Signscale = glm::scale(Signscale, glm::vec3(0.05,0.05,0.05));
			Sign = Signx * Signy * Signz * SignRx * SignRy * SignRz * Signscale;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Sign));
			InitTexture("model0.png", block.texture);
			glBindTexture(GL_TEXTURE_2D,block.texture);
			glBindVertexArray(block.VAO);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, object7);
		}
		//맵
		{
			glm::mat4 Signx = glm::mat4(1.0f);
			glm::mat4 Signy = glm::mat4(1.0f);
			glm::mat4 Signz = glm::mat4(1.0f);
			glm::mat4 SignRx = glm::mat4(1.0f);
			glm::mat4 SignRy = glm::mat4(1.0f);
			glm::mat4 SignRz = glm::mat4(1.0f);
			glm::mat4 Signscale = glm::mat4(1.0f);
			glm::mat4 Sign = glm::mat4(1.0f);
			Signx = glm::translate(Signx, glm::vec3(2.5, 0.0, 0.0));
			Signy = glm::translate(Signy, glm::vec3(0.0, 12., 0.0));
			Signz = glm::translate(Signz, glm::vec3(0.0, 0.0, -2));
			SignRx = glm::rotate(SignRx, glm::radians(0.0f), glm::vec3(1.5, 0.0, 0.0));
			SignRy = glm::rotate(SignRy, glm::radians(200.0f), glm::vec3(0.0, 1.0, 0.0));
			SignRz = glm::rotate(SignRz, glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0));
			Signscale = glm::scale(Signscale, glm::vec3(16., 13., 16.));
			Sign = Signx * Signy * Signz * SignRx * SignRy * SignRz * Signscale;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Sign));
			InitTexture("map10_I.png", map.texture);
			glBindTexture(GL_TEXTURE_2D, map.texture);
			glBindVertexArray(map.VAO);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, object4);
		}

		//핑크 곰
		{
			glm::mat4 camx = glm::mat4(1.0f);
			glm::mat4 camy = glm::mat4(1.0f);
			glm::mat4 camz = glm::mat4(1.0f);
			glm::mat4 SignRy = glm::mat4(1.0f);
			glm::mat4 camscale = glm::mat4(1.0f);
			glm::mat4 cam1 = glm::mat4(1.0f);
			SignRy = glm::rotate(SignRy, glm::radians(120.0f), glm::vec3(0.0, 1.0, 0.0));
			camx = glm::translate(camx, glm::vec3(1.0, 0.0, 0.0));
			camy = glm::translate(camy, glm::vec3(0.0, 0.5, 0.0));
			camz = glm::translate(camz, glm::vec3(0.0, 0.0, -2.));
			camscale = glm::scale(camscale, glm::vec3(0.5, 0.5, 0.5));
			cam1 = camx * camy * camz * SignRy * camscale;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cam1));
			InitTexture("char2.png", char2.texture);
			glBindTexture(GL_TEXTURE_2D, char2.texture);
			glBindVertexArray(char2.VAO);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, object2);
		}

		//곰2 갈색곰
		{
			glm::mat4 camx = glm::mat4(1.0f);
			glm::mat4 camy = glm::mat4(1.0f);
			glm::mat4 camz = glm::mat4(1.0f);
			glm::mat4 SignRy = glm::mat4(1.0f);
			glm::mat4 camscale = glm::mat4(1.0f);
			glm::mat4 cam1 = glm::mat4(1.0f);
			glm::mat4 move = glm::mat4(1.0f);
			glm::mat4 move_b = glm::mat4(1.0f);
			glm::mat4 move_a = glm::mat4(1.0f);
			glm::mat4 move_c = glm::mat4(1.0f);
			SignRy = glm::rotate(SignRy, glm::radians(120.0f), glm::vec3(0.0, 1.0, 0.0));
			camx = glm::translate(camx, glm::vec3(3.0, 0.0, 0.0));
			camy = glm::translate(camy, glm::vec3(0.0, 0.5, 0.0));
			camz = glm::translate(camz, glm::vec3(0.0, 0.0, -2.));
			if (isreset == false && b_bear_location == true) {
				move = glm::translate(move, glm::vec3(-3.0, -0.5, 2.0));

			}
			camscale = glm::scale(camscale, glm::vec3(0.5, 0.5, 0.5));
			if (isreset == false && b_bear_location == true) {
				move_b = glm::translate(move_b, glm::vec3(2.05,0.6, 1.6));
				move_a = glm::translate(move_a, glm::vec3(0.6*b_bear_x,0.0,-0.2*b_bear_x));
				move_c = glm::translate(move_c, glm::vec3(-0.15*b_bear_y,0.0,-0.6*b_bear_y));
			

			}
			cam1 = camx * camy * camz * move * move_b  * move_a * move_c * SignRy  * camscale;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cam1));
			InitTexture("char3.png", char3.texture);
			glBindTexture(GL_TEXTURE_2D, char3.texture);
			glBindVertexArray(char3.VAO);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, object3);
			if (line) {
				glBindVertexArray(char3.VAO);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glDrawArrays(GL_TRIANGLES, 0, object3);
			}
		}

		//곰3
		{
			glm::mat4 camx = glm::mat4(1.0f);
			glm::mat4 camy = glm::mat4(1.0f);
			glm::mat4 camz = glm::mat4(1.0f);
			glm::mat4 SignRy = glm::mat4(1.0f);
			glm::mat4 camscale = glm::mat4(1.0f);
			glm::mat4 cam1 = glm::mat4(1.0f);
			SignRy = glm::rotate(SignRy, glm::radians(120.0f), glm::vec3(0.0, 1.0, 0.0));
			camx = glm::translate(camx, glm::vec3(2.0, 0.0, 0.0));
			camy = glm::translate(camy, glm::vec3(0.0, 0.5, 0.0));
			camz = glm::translate(camz, glm::vec3(0.0, 0.0, -2.));
			camscale = glm::scale(camscale, glm::vec3(0.5, 0.5, 0.5));
			cam1 = camx * camy * camz * SignRy * camscale;
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cam1));
			InitTexture("char4.png", char4.texture);
			glBindTexture(GL_TEXTURE_2D, char4.texture);
			glBindVertexArray(char4.VAO);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, object5);
		}

	}
}
GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid InitBuffer() {

	//좌표축

	glGenVertexArrays(1, &cam.VAO);
	glGenBuffers(1, &cam.VBO_pos);
	glGenBuffers(1, &cam.VBO_normal);
	glGenBuffers(1, &cam.VBO_uv);

	glUseProgram(shaderProgram);
	glBindVertexArray(cam.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, cam.VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, cam.out_vertices.size() * sizeof(glm::vec3), &cam.out_vertices[0], GL_STATIC_DRAW);
	GLint pAttribute = glGetAttribLocation(shaderProgram, "vPos");
	glVertexAttribPointer(pAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(pAttribute);


	glBindBuffer(GL_ARRAY_BUFFER, cam.VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, cam.out_normals.size() * sizeof(glm::vec3), &cam.out_normals[0], GL_STATIC_DRAW);
	GLint nAttribute = glGetAttribLocation(shaderProgram, "vNormal");
	glVertexAttribPointer(nAttribute, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute);

	glBindBuffer(GL_ARRAY_BUFFER, cam.VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, cam.out_uvs.size() * sizeof(glm::vec2), &cam.out_uvs[0], GL_STATIC_DRAW);
	GLint cAttribute = glGetAttribLocation(shaderProgram, "vTexCoord");
	glVertexAttribPointer(cAttribute, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute);

	///============================================
	glGenVertexArrays(1, &map.VAO);
	glGenBuffers(1, &map.VBO_pos);
	glGenBuffers(1, &map.VBO_normal);
	glGenBuffers(1, &map.VBO_uv);

	glUseProgram(shaderProgram);
	glBindVertexArray(map.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, map.VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, map.out_vertices.size() * sizeof(glm::vec3), &map.out_vertices[0], GL_STATIC_DRAW);
	GLint pAttribute2 = glGetAttribLocation(shaderProgram, "vPos");
	glVertexAttribPointer(pAttribute2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(pAttribute2);


	glBindBuffer(GL_ARRAY_BUFFER, map.VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, map.out_normals.size() * sizeof(glm::vec3), &map.out_normals[0], GL_STATIC_DRAW);
	GLint nAttribute2 = glGetAttribLocation(shaderProgram, "vNormal");
	glVertexAttribPointer(nAttribute2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute2);

	glBindBuffer(GL_ARRAY_BUFFER, map.VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, map.out_uvs.size() * sizeof(glm::vec2), &map.out_uvs[0], GL_STATIC_DRAW);
	GLint cAttribute2 = glGetAttribLocation(shaderProgram, "vTexCoord");
	glVertexAttribPointer(cAttribute2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute2);

	//=================================
	glGenVertexArrays(1, &char2.VAO);
	glGenBuffers(1, &char2.VBO_pos);
	glGenBuffers(1, &char2.VBO_normal);
	glGenBuffers(1, &char2.VBO_uv);

	glUseProgram(shaderProgram);
	glBindVertexArray(char2.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, char2.VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, char2.out_vertices.size() * sizeof(glm::vec3), &char2.out_vertices[0], GL_STATIC_DRAW);
	GLint pAttribute3 = glGetAttribLocation(shaderProgram, "vPos");
	glVertexAttribPointer(pAttribute3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(pAttribute3);


	glBindBuffer(GL_ARRAY_BUFFER, char2.VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, char2.out_normals.size() * sizeof(glm::vec3), &char2.out_normals[0], GL_STATIC_DRAW);
	GLint nAttribute3 = glGetAttribLocation(shaderProgram, "vNormal");
	glVertexAttribPointer(nAttribute3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute3);

	glBindBuffer(GL_ARRAY_BUFFER, char2.VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, char2.out_uvs.size() * sizeof(glm::vec2), &char2.out_uvs[0], GL_STATIC_DRAW);
	GLint cAttribute3 = glGetAttribLocation(shaderProgram, "vTexCoord");
	glVertexAttribPointer(cAttribute3, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute3);

	//
	glGenVertexArrays(1, &char3.VAO);
	glGenBuffers(1, &char3.VBO_pos);
	glGenBuffers(1, &char3.VBO_normal);
	glGenBuffers(1, &char3.VBO_uv);

	glUseProgram(shaderProgram);
	glBindVertexArray(char3.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, char3.VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, char3.out_vertices.size() * sizeof(glm::vec3), &char3.out_vertices[0], GL_STATIC_DRAW);
	GLint pAttribute4 = glGetAttribLocation(shaderProgram, "vPos");
	glVertexAttribPointer(pAttribute4, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(pAttribute4);


	glBindBuffer(GL_ARRAY_BUFFER, char3.VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, char3.out_normals.size() * sizeof(glm::vec3), &char3.out_normals[0], GL_STATIC_DRAW);
	GLint nAttribute4 = glGetAttribLocation(shaderProgram, "vNormal");
	glVertexAttribPointer(nAttribute4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute4);

	glBindBuffer(GL_ARRAY_BUFFER, char3.VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, char3.out_uvs.size() * sizeof(glm::vec2), &char3.out_uvs[0], GL_STATIC_DRAW);
	GLint cAttribute4 = glGetAttribLocation(shaderProgram, "vTexCoord");
	glVertexAttribPointer(cAttribute4, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute4);
	//======
	glGenVertexArrays(1, &char4.VAO);
	glGenBuffers(1, &char4.VBO_pos);
	glGenBuffers(1, &char4.VBO_normal);
	glGenBuffers(1, &char4.VBO_uv);

	glUseProgram(shaderProgram);
	glBindVertexArray(char4.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, char4.VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, char4.out_vertices.size() * sizeof(glm::vec3), &char4.out_vertices[0], GL_STATIC_DRAW);
	GLint pAttribute5 = glGetAttribLocation(shaderProgram, "vPos");
	glVertexAttribPointer(pAttribute5, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(pAttribute5);


	glBindBuffer(GL_ARRAY_BUFFER, char4.VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, char4.out_normals.size() * sizeof(glm::vec3), &char4.out_normals[0], GL_STATIC_DRAW);
	GLint nAttribute5 = glGetAttribLocation(shaderProgram, "vNormal");
	glVertexAttribPointer(nAttribute5, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute5);

	glBindBuffer(GL_ARRAY_BUFFER, char4.VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, char4.out_uvs.size() * sizeof(glm::vec2), &char4.out_uvs[0], GL_STATIC_DRAW);
	GLint cAttribute5 = glGetAttribLocation(shaderProgram, "vTexCoord");
	glVertexAttribPointer(cAttribute5, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute5);
	//========================
	glGenVertexArrays(1, &balloon.VAO);
	glGenBuffers(1, &balloon.VBO_pos);
	glGenBuffers(1, &balloon.VBO_normal);
	glGenBuffers(1, &balloon.VBO_uv);

	glUseProgram(shaderProgram);
	glBindVertexArray(balloon.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, balloon.VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, balloon.out_vertices.size() * sizeof(glm::vec3), &balloon.out_vertices[0], GL_STATIC_DRAW);
	GLint pAttribute6 = glGetAttribLocation(shaderProgram, "vPos");
	glVertexAttribPointer(pAttribute6, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(pAttribute6);


	glBindBuffer(GL_ARRAY_BUFFER, balloon.VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, balloon.out_normals.size() * sizeof(glm::vec3), &balloon.out_normals[0], GL_STATIC_DRAW);
	GLint nAttribute6 = glGetAttribLocation(shaderProgram, "vNormal");
	glVertexAttribPointer(nAttribute6, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute6);

	glBindBuffer(GL_ARRAY_BUFFER, balloon.VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, balloon.out_uvs.size() * sizeof(glm::vec2), &balloon.out_uvs[0], GL_STATIC_DRAW);
	GLint cAttribute6 = glGetAttribLocation(shaderProgram, "vTexCoord");
	glVertexAttribPointer(cAttribute6, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute6);
	//----------
	

	//--- Vertex Array Object 생성
	glGenVertexArrays(1, &black_vao);
	glGenBuffers(2, black_vbo); //--- 2개의 VBO를 지정하고 할당하기

	glBindVertexArray(black_vao);
	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, black_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(GLfloat), rect_p, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, black_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(GLfloat), black, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	//시작화면 씬
	glGenVertexArrays(1, &scene.VAO);
	glGenBuffers(1, &scene.VBO_pos);
	glGenBuffers(1, &scene.VBO_normal);
	glGenBuffers(1, &scene.VBO_uv);

	glUseProgram(shaderProgram);
	glBindVertexArray(scene.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, scene.VBO_pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(scene_), scene_, GL_STATIC_DRAW);
	GLint pAttribute8 = glGetAttribLocation(shaderProgram, "vPos");
	glVertexAttribPointer(pAttribute8, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(pAttribute8);


	glBindBuffer(GL_ARRAY_BUFFER, scene.VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(scene_), scene_, GL_STATIC_DRAW);
	GLint nAttribute8 = glGetAttribLocation(shaderProgram, "vNormal");
	glVertexAttribPointer(nAttribute8, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(nAttribute8);

	glBindBuffer(GL_ARRAY_BUFFER, scene.VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, sizeof(scene_), scene_, GL_STATIC_DRAW);
	GLint cAttribute8 = glGetAttribLocation(shaderProgram, "vTexCoord");
	glVertexAttribPointer(cAttribute8, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(cAttribute8);

	// 타일
	glGenVertexArrays(1, &block.VAO);
	glGenBuffers(1, &block.VBO_pos);
	glGenBuffers(1, &block.VBO_normal);
	glGenBuffers(1, &block.VBO_uv);

	glUseProgram(shaderProgram);
	glBindVertexArray(block.VAO);
	glBindBuffer(GL_ARRAY_BUFFER,block.VBO_pos);
	glBufferData(GL_ARRAY_BUFFER,block.out_vertices.size() * sizeof(glm::vec3), &block.out_vertices[0], GL_STATIC_DRAW);
	GLint pAttribute9 = glGetAttribLocation(shaderProgram, "vPos");
	glVertexAttribPointer(pAttribute9, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(pAttribute9);


	glBindBuffer(GL_ARRAY_BUFFER, block.VBO_normal);
	glBufferData(GL_ARRAY_BUFFER, block.out_normals.size() * sizeof(glm::vec3), &block.out_normals[0], GL_STATIC_DRAW);
	GLint nAttribute9 = glGetAttribLocation(shaderProgram, "vNormal");
	glVertexAttribPointer(nAttribute9, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(nAttribute9);

	glBindBuffer(GL_ARRAY_BUFFER, block.VBO_uv);
	glBufferData(GL_ARRAY_BUFFER, block.out_uvs.size() * sizeof(glm::vec2), &block.out_uvs[0], GL_STATIC_DRAW);
	GLint cAttribute9 = glGetAttribLocation(shaderProgram, "vTexCoord");
	glVertexAttribPointer(cAttribute9, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	glEnableVertexAttribArray(cAttribute9);

}

void make_vertexShaders() {

	GLchar* vertexSource;

	//버텍스 세이더 읽어와서 객체 만들고 컴파일하기
	vertexSource = filetobuf("vertext.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader); //버텍스 컴파일 


	//에러코드 작성
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}

}

void make_fragmentShaders() {

	GLchar* fragmentSource;

	//프래그먼트 세이더 읽어와서 객체 만들고 컴파일하기
	fragmentSource = filetobuf("fregmentt.glsl");
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader); //버텍스 컴파일 


	//에러코드 작성
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}

}

void make_shaderProgram() {

	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);


	//프로그램 생성 후 객체 삭제 가능
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	//에러코드 작성
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(shaderProgram, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
	glUseProgram(shaderProgram);

}

void InitTexture(const char* filename ,unsigned int texture)
{


	glGenTextures(1, &texture);
	stbi_set_flip_vertically_on_load(true);
	//glBindTexture(GL_TEXTURE_2D, texture);
	// 텍스처 wrapping/filtering 옵션 설정(현재 바인딩된 텍스처 객체에 대해)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 텍스처 로드 및 생성
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void unProject(int xCursor, int yCursor)
{
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	glGetIntegerv(GL_VIEWPORT, viewPort);

	winX = (float)xCursor;
	winY = (float)viewPort[3] - (float)yCursor;
	glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &zCursor);
	
	if (gluUnProject(winX, winY, zCursor, modelView, projection, viewPort, &wx, &wy, &wz) == GLU_FALSE) {
		printf("실패\n");
	}
	else {
		wx -= 0.2;
		wy += 3.9;
		cout << wx << " " << wy << " " << wz << endl;
	}

}

GLvoid Mouse(int button, int state, int mx, int my)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		unProject(mx, my);
		if (-0.03 < wx && wx <0.48 && -0.44>wy && wy>-1.15) {
			cout << "start" << endl;;
			start = true;
			
		}
		if(start){
			for (int i = 0; i < 5; ++i) {
				for (int j = 0; j < 5; ++j) {
					if (-0.31 + 0.076 * j< wx && wx < -0.31 + 0.076 * (j + 1) && -0.15 - 0.188 * i>wy && wy > -0.15 - 0.188 * (i + 1)) {
						cout << j << ", " << i << endl;
						if (b_bear_select) {
							cout << "여기 어때" << endl;
							b_bear_location = true;
							b_bear_x = i;
							b_bear_y = j;
						}
					}
				}
			}

			if (0.11 < wx && wx <0.15 && -0.68>wy && wy > -0.906667) {
				cout << "갈색 곰";
				b_bear_select = !b_bear_select;
				isreset = false;
			}



			if (0.335 < wx && wx <0.45 && 1.96>wy && wy > 1.8) {
				cout << "camera";

				if (start == true) {
					cameraclick = true;
				}
			}

			if (0.505 < wx && wx <0.598333 && 2>wy && wy > 1.81333) {
				cout << "reset";
				//isstart= true;
				if (start == true) {
					isreset = true;
					b_bear_select = false;
				}
			}

			if(0.19 < wx && wx <0.276667 && 1.97333>wy && wy > 1.84667) {
				cout << "hint";
				//isstart= true;
				if (start == true) {
					hint.hint_ = true;
					numtimer = 4;
					hint.blank = false;
					//none_hint = false;
					timerend = true;
					hint.num--;
					none_hint = false;
					//barsys = true;
					
				}
			}
		}
		
	}
	else
		select =false;
	glutPostRedisplay();
}

//오브젝트 파일 읽기
bool loadOBJ(const char* path, vector< glm::vec3 >& vertices, vector< glm::vec2 >& uvs, vector< glm::vec3 >& normals) {
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	float sumX = 0.0, sumY = 0.0, sumZ = 0.0;
	float aveX, aveY, aveZ;
	float scaleX, scaleY, scaleZ;
	float minX = 0.0, minY = 0.0, minZ = 0.0;
	float maxX = 0.0, maxY = 0.0, maxZ = 0.0;
	float scaleAll;

	float sizeX, sizeY, sizeZ;

	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file !\n");
		return false;
	}
	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);

			if (vertex.x < minX) minX = vertex.x;
			if (vertex.y < minY) minY = vertex.y;
			if (vertex.z < minZ) minZ = vertex.z;
			if (vertex.x > maxX) maxX = vertex.x;
			if (vertex.y > maxY) maxY = vertex.y;
			if (vertex.z > maxZ) maxZ = vertex.z;
			sumX += vertex.x;
			sumY += vertex.y;
			sumZ += vertex.z;



			temp_vertices.push_back(vertex);
		}
		//정점 텍스쳐 읽기
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		} //정점 노멀벡터 읽기 
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);

		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);

		}


		aveX = sumX / vertexIndices.size();
		aveY = sumY / vertexIndices.size();
		aveZ = sumZ / vertexIndices.size();
		scaleX = maxX - minX;
		scaleY = maxY - minY;
		scaleZ = maxZ - minZ;
	}
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		glm::vec3 temp;
		unsigned int vertexIndex = vertexIndices[i];
		temp = temp_vertices[vertexIndex - 1];
		temp.x = temp.x - minX;
		temp.y = temp.y - minY;
		temp.z = temp.z - minZ;

		temp.x = ((temp.x * 2.0f) / scaleX) - 1.0f;
		temp.y = ((temp.y * 2.0f) / scaleY) - 1.0f;
		temp.z = ((temp.z * 2.0f) / scaleZ) - 1.0f;


		vertices.push_back(temp);
		//glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		//outvertex.push_back(vertex);
	}





	for (unsigned int i = 0; i < uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 vertex = temp_uvs[uvIndex - 1];
		uvs.push_back(vertex);
	}
	for (unsigned int i = 0; i < normalIndices.size(); i++) {
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 vertex = temp_normals[normalIndex - 1];
		normals.push_back(vertex);
	}


	return true;
}

//파일 읽기-----------------------
char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading 
	if (!fptr) // Return NULL on failure 
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file 
	length = ftell(fptr); // Find out how many bytes into the file we are 
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator 
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file 
	fread(buf, length, 1, fptr);// Read the contents of the file in to the buffer 
	fclose(fptr); // Close the file 
	buf[length] = 0;
	// Null terminator 
	return buf; // Return the buffer 
}

//void blanksys() {
//	if (hint.blank) {
//		int blankdir = 1;
//		if (blank_h.x == -0.5)
//		{
//			blankdir = 0;
//		}
//		blank_h.x -= 0.5 * blankdir;
//	}
//
//}
//
//void opensys() {
//	if (hint.hint_) {
//		if (hint.num >= 0) {
//			int blankdir = 1;
//			if (blank_h.x == 5.0) {
//				blankdir = 0;
//				hint.hint_ = false;
//				hint.hintsys();
//				hint.blank = true;
//			}
//			blank_h.x += 0.5 * blankdir;
//		}
//
//	}
//
//}
void bar_time()
{
	if (barsys) {
		/*if (bar.scalex >= 0) {
			bar.scalex -= 0.01;
			bar.x -= 0.01;
		}
		if (bar.scalex == 0)
		{
			cameraclick = true;
		}*/
		if (bar.scalex <=1.0)
		{
			bardir = 0;
			cameraclick = true;
		}
		bar.scalex -= 0.05 * bardir;
		bar.x -= 0.05 * bardir;
	}
}
void selectobj()
{
	if (b_bear_select)
	{
		line = !line;
	}
}
void TimerFunction(int value)
{
	selectobj();
	bar_time();
	glutPostRedisplay();
	glutTimerFunc(500, TimerFunction, 1);
}
void hinttimer()
{
	if (hint.hint_)
	{
		numtimer--;
		if (numtimer == 0)
		{
			hint.blank = true;
			timerend = false;
			
		}
	}
}
void TimerFunction2(int value)
{

		numtimer--;
		if (numtimer == 0)
		{
			hint.blank = true;
			timerend = false;
			none_hint = true;
			barsys = true;

		}

		glutPostRedisplay();
		if (timerend) {
			glutTimerFunc(1000, TimerFunction2, 1);
		}
}


GLvoid keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		//힌트
	case 's'://시작키

		break;
	case 'h':
		
		break;
	case '1'://확인용
		hint.blank = false;
		blank_h.x += 0.5;
		break;
	case '2':
		start = true;
		
		break;
	case '3':
		
		break;
	}
	
	glutPostRedisplay();
}

#ifdef WIN32
#include <windows.h>
#define CLEAR_SCREEN "cls"
#else
#define CLEAR_SCREEN "clear"
#endif

#include <GL\glew.h>
#include <GL\freeglut.h>

#include <iostream>

#include <glm\glm.hpp>
#include <glm\ext.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include "loadshaders.h"
#include "vehicle.h"
#include "column.h"

using namespace std;

GLuint program;
GLuint VertextBuffer;
GLuint ColorBuffer;
GLuint VA;
GLuint shaderprogram;

GLuint MatrixID;
glm::mat4 MVP;

void init(void)
{

	glGenVertexArrays(1, &VA);
	glBindVertexArray(VA);

	GLfloat vertex[] = {
		-0.85, -0.85, 0.0, // Triangle 1
		-0.10, 0.0, 0.0,
		-0.85, 0.85, 0.0,
		-0.85, 0.85, 0.0, // Triangle 2
		0.0, 0.10, 0.0,
		0.85, 0.85, 0.0,
		0.85, 0.85, 0.0, // Triangle 3
		0.10, 0.0, 0.0,
		0.85, -0.85, 0.0,
		0.85, -0.85, 0.0, // Triangle 4
		0.0, -0.10, 0.0,
		-0.85, -0.85, 0.0
	};

	GLfloat colors[36] = {
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.f, 0.0f,
		0.559f, 0.436f, 0.730f,
		0.359f, 0.583f, 0.152f,
		0.483f, 0.596f, 0.789f,
		0.559f, 0.861f, 0.639f,
		0.195f, 0.548f, 0.859f,
		0.14f, 0.184f, 0.576f,
		0.771f, 0.328f, 0.970f,
		0.406f, 0.615f, 0.116f,
		0.676f, 0.977f, 0.133f
	};

	glGenBuffers(1, &VertextBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertextBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	//glBufferSubData(GL_ARRAY_BUFFER, NULL, sizeof(vert), vert);
	//glBufferSubData(GL_ARRAY_BUFFER, sizeof(vert), sizeof(col), col);

	glGenBuffers(1, &ColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	ShaderInfo shaders[] =
	{
		{ GL_VERTEX_SHADER, "NULL" },
		{ GL_FRAGMENT_SHADER, "NULL" },
		{ GL_NONE, NULL }
	};

	shaderprogram = LoadShaders(shaders);



	MatrixID = glGetUniformLocation(shaderprogram, "MVP");

	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

	glm::mat4 Scal = glm::scale(glm::vec3(3.0, 3.0, 1.0));

	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

	glm::mat4 Model = glm::mat4(1.0f);

	MVP = Scal * Projection * View * Model;
}

void display(void)
{
	glUseProgram(shaderprogram);
	glClear(GL_COLOR_BUFFER_BIT);

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VertextBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 12);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, 'A');

	glutSwapBuffers();
}

int frame = 0, t = 0, timebase = 0;
float fps;

void fpscalculate()
{
	frame++;
	t = glutGet(GLUT_ELAPSED_TIME);

	if (t - timebase > 1000) {
		fps = (frame*1000.0 / (t - timebase));
		timebase = t;
		frame = 0;
	}

	cout << "FPS: " << fps << endl;
}


float numticks = 0;

const int TICKS_PER_SECOND = 25;
const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
const int MAX_FRAMESKIP = 5;

unsigned long next_game_tick = glutGet(GLUT_ELAPSED_TIME);
unsigned long elapsed_time = 0;

int loops;
float interpolation;

void game()
{
		numticks++;

	if (numticks > 10)
		numticks = 0;

	GLfloat colors[36] = {
		numticks / 10, numticks / 10, 1.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.f, 0.0f,
		0.559f, 0.436f, numticks / 10,
		0.359f, numticks / 10, 0.152f,
		numticks / 10, 0.596f, 0.789f,
		0.559f, 0.861f, 0.639f,
		numticks / 10, numticks / 10, 0.859f,
		0.14f, 0.184f, numticks / 10,
		0.771f, numticks / 10, 0.970f,
		numticks / 10, 0.615f, 0.116f,
		0.676f, 0.977f, numticks / 10
	};

	glBindBuffer(GL_ARRAY_BUFFER, ColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);

	GLfloat vertex[] = {
		-numticks / 10, -0.85, 0.0, // Triangle 1
		-0.10, 0.0, numticks / 10,
		-0.85, numticks / 10, 0.0,
		-numticks / 10, 0.85, 0.0, // Triangle 2
		0.0, numticks / 10, 0.0,
		0.85, numticks / 10, 0.0,
		0.85, numticks / 10, 0.0, // Triangle 3
		numticks / 10, 0.0, 0.0,
		0.85, -0.85, 0.0,
		0.85, -0.85, 0.0, // Triangle 4
		0.0, -0.10, 0.0,
		-0.85, -0.85, numticks / 10
	};

	glBindBuffer(GL_ARRAY_BUFFER, VertextBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_DYNAMIC_DRAW);

}

void update_game(int = 1)
{
	if (glutGet(GLUT_ELAPSED_TIME) > next_game_tick && loops < MAX_FRAMESKIP) {		
		game();
		next_game_tick += SKIP_TICKS;
		loops++;
		glutTimerFunc(SKIP_TICKS, update_game, 1);
	}
}


void timer(int = 0)
{
	loops = 0;

	interpolation = float(glutGet(GLUT_ELAPSED_TIME) + SKIP_TICKS - next_game_tick) / float(SKIP_TICKS);

	fpscalculate();

	display();

	glutTimerFunc(1, timer, 0);
}

vehicle car1(2.0, 3.0, glm::vec3(3.0, 2.0, 0.0), 4.0, 0.0, 2.0, 6.0, 1500.0, 0, 0);
vehicle car2(2.0, 3.0, glm::vec3(3.0, 2.0, 0.0), 4.0, 0.0, 2.0, 6.0, 2500.0, 1, 1);
vehicle car3(2.0, 3.0, glm::vec3(3.0, 2.0, 0.0), 4.0, 0.0, 2.0, 6.0, 2500.0, 2, 2);

int main(int argc, char** argv) {
	column c;
	c.push(car1);
	c.push(car2);
	c.push(car3);

	c.printStack();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(argv[0]);
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		cerr << "Unable to initialize GLEW ... exiting" << endl;
		exit(EXIT_FAILURE);
	}
	init();
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glutDisplayFunc(display);
	update_game();
	timer();
	glutMainLoop();
}

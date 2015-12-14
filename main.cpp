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

const int max_veh = 10;

column c(max_veh);

glm::vec3 normolize(glm::vec3 des)
{
	float lenght = sqrt(des.x * des.x + des.y * des.y + des.z * des.z);
	glm::vec3 res;
	res.x = des.x / lenght;
	res.y = des.y / lenght;
	res.z = des.z / lenght;
	return res;
}

void init(void)
{

	glGenVertexArrays(1, &VA);
	glBindVertexArray(VA);

	GLfloat colors[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.00f, 0.0f, 0.0f,
		1.00f, 0.0f, 0.0f
	};

	glGenBuffers(1, &VertextBuffer);
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
	glm::mat4 Scal = glm::scale(glm::vec3(1.0, 1.0, 1.0));
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 14), // Camera is at (4,3,3), in World Space
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

	for (int i = 0; i < c.getTop(); i++)
	{
		glDrawArrays(GL_TRIANGLES, i * 8, 6);
		glDrawArrays(GL_LINE_STRIP, ((i + 1) * 8) - 2, 2);
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	//glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, 'A');

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

const int TICKS_PER_SECOND = 60;
const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
const int MAX_FRAMESKIP = 5;

unsigned long next_game_tick = glutGet(GLUT_ELAPSED_TIME);
unsigned long elapsed_time = 0;

int loops;
float interpolation;

void game()
{

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

float a = -0.01;
float b = 0.01;

void move()
{
	glm::vec3 vec;
	for (int i = 0; i < c.getTop(); i++)
	{
		c.Peek(i).x += c.Peek(i).destination.x * c.Peek(i).speed;
		c.Peek(i).y += c.Peek(i).destination.y * c.Peek(i).speed;
		vec = normolize(glm::vec3(c.Peek(i).destination.x + b, c.Peek(i).destination.y + a, 0.0));
		c.Peek(i).destination = vec;
	}
}



void timer(int = 0)
{
	a += 0.00005;
	b -= 0.00007;
	int const numVertex = 8 * 3;
	GLfloat vertex[max_veh * numVertex];

	move();

	for (int i = 0; i < c.getTop(); i++)
	{
		vehicle car = c.Peek(i);
		float w2 = car.width / 2;
		float h2 = car.height / 2;
		float x = car.x;
		float y = car.y;
		int j = 0;

		// Triangle 1
		vertex[(i * numVertex) + j++] = (x - w2) / 10;
		vertex[(i * numVertex) + j++] = (y + h2) / 10;
		vertex[(i * numVertex) + j++] = 0.0;
		vertex[(i * numVertex) + j++] = (x - w2) / 10;
		vertex[(i * numVertex) + j++] = (y - h2) / 10;
		vertex[(i * numVertex) + j++] = 0.0;
		vertex[(i * numVertex) + j++] = (x + w2) / 10;
		vertex[(i * numVertex) + j++] = (y - h2) / 10;
		vertex[(i * numVertex) + j++] = 0.0;
		// Triangle 2
		vertex[(i * numVertex) + j++] = (x + w2) / 10;
		vertex[(i * numVertex) + j++] = (y + h2) / 10;
		vertex[(i * numVertex) + j++] = 0.0;
		vertex[(i * numVertex) + j++] = (x + w2) / 10;
		vertex[(i * numVertex) + j++] = (y - h2) / 10;
		vertex[(i * numVertex) + j++] = 0.0;
		vertex[(i * numVertex) + j++] = (x - w2) / 10;
		vertex[(i * numVertex) + j++] = (y + h2) / 10;
		vertex[(i * numVertex) + j++] = 0.0;
		//Vector
		vertex[(i * numVertex) + j++] = (x / 10) + (car.destination.x) * 10;
		vertex[(i * numVertex) + j++] = (y / 10) + (car.destination.y) * 10;
		vertex[(i * numVertex) + j++] = 0.0;
		vertex[(i * numVertex) + j++] = (x / 10);
		vertex[(i * numVertex) + j++] = (y / 10);
		vertex[(i * numVertex) + j++] = 0.0;
		j = 0;

		float s = c.Peek(i).destination.length();

		cout << "========= Car " << i << " =========" << endl;
		cout << "x" << i << ": " << x << " y" << i << ": " << y << " destination: " << c.Peek(i).destination.length() << endl;
		cout << "Angle: " << car.getAngle() << endl;
	}

	glBindBuffer(GL_ARRAY_BUFFER, VertextBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_DYNAMIC_DRAW);

	fpscalculate();

	display();
	
	/*	if (c.Peek(0).y > 35)
	c.Peek(0).destination *= c.Peek(0).deceleration;
		else
	c.Peek(0).destination *= c.Peek(0).acceleration;
		if (c.Peek(3).y > 35)
			c.Peek(3).destination *= c.Peek(3).deceleration;
		else
			c.Peek(3).destination *= c.Peek(3).acceleration;
			*/
	//c.checkSpeed();

	glutTimerFunc(SKIP_TICKS, timer, 0);
}

vehicle car1(-35.0, -35.0, 0.5, glm::vec3(0.707, 0.707, 0.0), 1.0, 0.99, 2.0, 5.0, 1500.0, 0, 0);
vehicle car2(-35.0, -35.0, 0.5, glm::vec3(0.1, 1.0, 0.0), 1.0, 0.7, 3.0, 6.0, 1500.0, 1, 1);
vehicle car3(-35.0, -35.0, 0.8, glm::vec3(1, 0.0, 0.0), 1.0, 0.7, 3.0, 6.0, 1500.0, 2, 2);
vehicle car4(-35.0, -35.0, 0.5, glm::vec3(0.5, 0.866, 0.0), 1.0, 0.99, 2.0, 5.0, 1500.0, 0, 0);
vehicle car5(0.0, 28.0, 0.3, glm::vec3(0.0, 0.05, 0.0), 4.0, 0.0, 2.5, 4.0, 1500.0, 4, 4);

int main(int argc, char** argv) {
	c.push(car1);
	c.push(car2);
	c.push(car3);
	c.push(car4);
	c.push(car5);

	c.printStack();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1024, 1024);
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
	//update_game();
	timer();
	glutMainLoop();
}

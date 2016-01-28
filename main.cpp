#include <gl/glew.h>
#include <gl/freeglut.h>
#include <iostream>

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

const int max_veh = 8;

column c(max_veh);


/*glm::vec3 rotate(glm::vec3 cpoint, glm::vec3 point, float angle){
	glm::vec3 rotated_point;
	rotated_point.x = cpoint.x + (point.x - cpoint.x) * cos(angle) - (point.y - cpoint.y) * sin(angle);
	rotated_point.y = cpoint.y + (point.y - cpoint.y) * cos(angle) + (point.x - cpoint.x) * sin(angle);
	rotated_point.z = 0;
	return rotated_point;
}
*/
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
		glm::vec3(0, 0, 20), // Camera is at (4,3,3), in World Space
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
		if (i == c.getTop() - 1)
		{
			glDrawArrays(GL_LINE_STRIP, ((i + 1) * 8), 16);
		}
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

	//cout << "FPS: " << fps << endl;
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

float a = 0;
float b = 0;
float dx, dy, tx, ty;

void move()
{
	for (int i = 0; i < c.getTop(); i++)
	{
		if (c.Peek(i).y > 45 && c.Peek(i).x > -10)
		{
			c.Peek(i).direction.x += 0.02;
		}

		if (c.Peek(i).x > 63)
		{
			c.Peek(i).direction.y -= 0.03;
		}

		if (c.Peek(i).x > 40 && c.Peek(i).y < -40)
		{
			c.Peek(i).direction.y += 0.03;
			c.Peek(i).direction.x += 0.02;
		}

		/*if (c.Peek(i).x < -40 && c.Peek(i).y < -20)
		{
			c.Peek(i).direction.y += 0.01;
			c.Peek(i).direction.x += 0.0025;
		}*/

		c.Peek(i).direction = normolize({ c.Peek(i).direction.x, c.Peek(i).direction.y, 0.0 });

		c.Peek(i).x += c.Peek(i).direction.x * c.Peek(i).speed;
		c.Peek(i).y += c.Peek(i).direction.y * c.Peek(i).speed;

		for (int j = 0; j < 4; j++)
		{
			c.Peek(i).xCoord[j] += c.Peek(i).direction.x * c.Peek(i).speed;
			c.Peek(i).yCoord[j] += c.Peek(i).direction.y * c.Peek(i).speed;
		}

		float angle = c.Peek(i).getAngle();
		c.Peek(i).rotate(angle);

	}
}

void timer(int = 0)
{
	//a -= 0.0001;

	

	int const numVertex = 8 * 3;
	GLfloat vertex[max_veh * numVertex + 50];

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
		vertex[(i * numVertex) + j++] = car.xCoord[0] / 10;
		vertex[(i * numVertex) + j++] = car.yCoord[0] / 10;
		vertex[(i * numVertex) + j++] = 0.0;
		vertex[(i * numVertex) + j++] = car.xCoord[3] / 10;
		vertex[(i * numVertex) + j++] = car.yCoord[3] / 10;
		vertex[(i * numVertex) + j++] = 0.0;
		vertex[(i * numVertex) + j++] = car.xCoord[2] / 10;
		vertex[(i * numVertex) + j++] = car.yCoord[2] / 10;
		vertex[(i * numVertex) + j++] = 0.0;
		// Triangle 2
		vertex[(i * numVertex) + j++] = car.xCoord[1] / 10;
		vertex[(i * numVertex) + j++] = car.yCoord[1] / 10;
		vertex[(i * numVertex) + j++] = 0.0;
		vertex[(i * numVertex) + j++] = car.xCoord[2] / 10;
		vertex[(i * numVertex) + j++] = car.yCoord[2] / 10;
		vertex[(i * numVertex) + j++] = 0.0;
		vertex[(i * numVertex) + j++] = car.xCoord[0] / 10;
		vertex[(i * numVertex) + j++] = car.yCoord[0] / 10;
		vertex[(i * numVertex) + j++] = 0.0;
		//Vector
		vertex[(i * numVertex) + j++] = (x / 10) + (car.direction.x) * 0;
		vertex[(i * numVertex) + j++] = (y / 10) + (car.direction.y) * 0;
		vertex[(i * numVertex) + j++] = 0.0;
		vertex[(i * numVertex) + j++] = (x / 10);
		vertex[(i * numVertex) + j++] = (y / 10);
		vertex[(i * numVertex) + j++] = 0.0;

		if (i == c.getTop() - 1)
		{
			int index = (i * numVertex)+j;
			vertex[index++] = -3.0; //x
			vertex[index++] = -15.0; //y
			vertex[index++] = 0.0;//z
			vertex[index++] = -3.0; //x
			vertex[index++] = 9.0;//y
			vertex[index++] = 0.0; //z
			vertex[index++] = 15.0;//x
			vertex[index++] = 9.0; //y
			vertex[index++] = 0.0;//z
			vertex[index++] = 15.0; //x
			vertex[index++] = 5.0; //y
			vertex[index++] = 0.0;//z
			vertex[index++] = 11.0; //x
			vertex[index++] = 5.0;//y
			vertex[index++] = 0.0; //z
			vertex[index++] = 11.0; //x
			vertex[index++] = -3.0;//y
			vertex[index++] = 0.0; //z
			vertex[index++] = 15.0; //x
			vertex[index++] = -3.0;//y
			vertex[index++] = 0.0; //z
			vertex[index++] = 15.0; //x
			vertex[index++] = -7.0;//y
			vertex[index++] = 0.0; //z
			vertex[index++] = 11.0; //x
			vertex[index++] = -7.0;//y
			vertex[index++] = 0.0; //z
			vertex[index++] = 11.0; //x
			vertex[index++] = -15.0;//y
			vertex[index++] = 0.0; //z
			vertex[index++] = 7.0; //x
			vertex[index++] = -15.0;//y
			vertex[index++] = 0.0; //z
			vertex[index++] = 7.0; //x
			vertex[index++] = 5.0;//y
			vertex[index++] = 0.0; //z
			vertex[index++] = 1.0; //x
			vertex[index++] = 5.0;//y
			vertex[index++] = 0.0; //z
			vertex[index++] = 1.0; //x
			vertex[index++] = -15.0;//y
			vertex[index++] = 0.0; //z
			
			
		}


		j = 0;


	}

	glBindBuffer(GL_ARRAY_BUFFER, VertextBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_DYNAMIC_DRAW);

	fpscalculate();

	display();

	for (int i = 1; i < c.getTop(); i++)
	{
		//c.Peek(i).direction = normolize(glm::vec3(c.Peek(i).direction.x + a, c.Peek(i).direction.y + b, 0.0));
		c.Peek(i).checkDis(c.Peek(i - 1));
	}

	/*	if (c.Peek(0).y > 35)
	c.Peek(0).direction *= c.Peek(0).deceleration;
	else
	c.Peek(0).direction *= c.Peek(0).acceleration;
	if (c.Peek(3).y > 35)
	c.Peek(3).direction *= c.Peek(3).deceleration;
	else
	c.Peek(3).direction *= c.Peek(3).acceleration;
	*/
	//c.checkSpeed();

	glutTimerFunc(5, timer, 0);
}

vehicle car1(5.0, 0.0, 0.1, glm::vec3(0.0, 1.0, 0.0), 0.001, 0.0009, 4.0, 9.0, 1500.0, 0, 0);
vehicle car2(5.0, -20, 0.2, glm::vec3(0.0, 1.0, 0.0), 0.001, 0.0009, 3.0, 6.0, 1500.0, 1, 1);
vehicle car3(5.0, -46, 0.3, glm::vec3(0.0, 1.0, 0.0), 0.001, 0.0009, 3.0, 6.0, 1500.0, 2, 2);
vehicle car4(-10.0, 0.0, 0.25, glm::vec3(0.0, 1.0, 0.0), 1.0, 0.99, 2.0, 5.0, 1500.0, 0, 0);
vehicle car5(-5.0, 0.0, 0.2, glm::vec3(0.0, 1.0, 0.0), 4.0, 0.0, 2.5, 4.0, 1500.0, 4, 4);

vehicle cars[max_veh];

int main(int argc, char** argv) {
	//c.push(car1);
	//c.push(car2);
	//c.push(car3);
	//c.push(car4);
	//c.push(car5);

	for (int i = 0; i < max_veh; i++)
	{
		cars[i].x = (i - max_veh / 2) * 0 - 5;
		cars[i].y = -100 - (i - max_veh / 2) * 10;
		//cars[i].y = 0;
		cars[i].speed = +0.20;

		cars[i].direction = normolize({ 0.0, 1.0, 0.0 });
		cars[i].acceleration = 0.01;
		cars[i].deceleration = 0.0009;
		cars[i].width = 3;
		cars[i].height = 6;
		cars[i].idVehicle = i;
		cars[i].position = i;
		float w2 = cars[i].width / 2;
		float h2 = cars[i].height / 2;

		if (cars[i].direction.y > 0)
		{

			cars[i].xCoord[0] = cars[i].x - w2;
			cars[i].yCoord[0] = cars[i].y + h2;

			cars[i].xCoord[1] = cars[i].x + w2;
			cars[i].yCoord[1] = cars[i].y + h2;

			cars[i].xCoord[2] = cars[i].x + w2 + 1;
			cars[i].yCoord[2] = cars[i].y - h2;

			cars[i].xCoord[3] = cars[i].x - w2 - 1;
			cars[i].yCoord[3] = cars[i].y - h2;
		}

		if (cars[i].direction.y < 0)
		{

			cars[i].xCoord[0] = cars[i].x - w2;
			cars[i].yCoord[0] = cars[i].y - h2;

			cars[i].xCoord[1] = cars[i].x + w2;
			cars[i].yCoord[1] = cars[i].y - h2;

			cars[i].xCoord[2] = cars[i].x + w2 + 1;
			cars[i].yCoord[2] = cars[i].y + h2;

			cars[i].xCoord[3] = cars[i].x - w2 - 1;
			cars[i].yCoord[3] = cars[i].y + h2;
			cout << "" << endl;
		}



		c.push(cars[i]);
	}

	//c.Peek(0).speed = 0.2;
	//c.printStack();

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

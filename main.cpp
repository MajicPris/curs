#define M_PI 3.14159265358979323846
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <iostream>

#include "loadshaders.h"
#include "vehicle.h"
#include "column.h"

using namespace std;

class direction
{
public:
	glm::vec2 dir;
	bool main;
};

class intersection
{
public:
	float x, y;
	float size; //height and width
	direction directions[4];
	int connections[4];
};

GLuint VertextBuffer;
GLuint ColorBuffer;
GLuint VA;
GLuint shaderprogram;

GLuint MatrixID;
glm::mat4 MVP;

const int num_intersetions = 7;
intersection intersections[num_intersetions] = { { 0.0f, -230.0f, 20.0f, { { glm::vec2(0.0, 1.0), true }, { glm::vec2(0.0, 0.0), false }, { glm::vec2(0.0, -1.0), false }, { glm::vec2(0.0, 0.0), false } }, { 1, -1, -1, -1 } }, //id 0
{ 0.0f, 40.0f, 20.0f, { { glm::vec2(0.0, 0.0), false }, { glm::vec2(1.0, 0.0), false }, { glm::vec2(0.0, -1.0), false }, { glm::vec2(-1.0, 0.0), true } }, { -1, 2, 0, 6 } }, //id 1
{ 70.0f, 40.0f, 20.0f, { { glm::vec2(0.0, 0.0), false }, { glm::vec2(0.0, 0.0), false }, { glm::vec2(0.0, -1.0), true }, { glm::vec2(-1.0, 0.0), false } }, { -1, -1, 3, 1 } }, //id 2
{ 70.0f, -40.0f, 20.0f, { { glm::vec2(0.0, 1.0), false }, { glm::vec2(1.0, 0.0), true }, { glm::vec2(0.0, 0.0), false }, { glm::vec2(-1.0, 0.0), false } }, { 2, 4, -1, 5 } }, //id 3
{ 140.0f, -40.0f, 20.0f, { { glm::vec2(0.0, 0.0), false }, { glm::vec2(1.0, 0.0), false }, { glm::vec2(0.0, 0.0), false }, { glm::vec2(-1.0, 0.0), true } }, { -1, -1, -1, 3 } }, //id 4
{ 30.0f, -40.0f, 20.0f, { { glm::vec2(0.0, 1.0), true }, { glm::vec2(1.0, 0.0), false }, { glm::vec2(0.0, 0.0), false }, { glm::vec2(0.0, 0.0), false } }, { -1, 3, -1, -1 } }, //id 5
{ -90.0f, 40.0f, 20.0f, { { glm::vec2(0.0, 1.0), true }, { glm::vec2(1.0, 0.0), false }, { glm::vec2(0.0, 0.0), false }, { glm::vec2(0.0, 0.0), false } }, { -1, 1, -1, -1 } } }; //id 6



const int max_veh = 1;
column c(max_veh);

void init(void)
{
	glGenVertexArrays(1, &VA);
	glBindVertexArray(VA);

	GLfloat colors[] = {
		0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,
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
		glm::vec3(0, 0, 15), // Camera is at (4,3,3), in World Space
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

			glDrawArrays(GL_LINES, (i + 1) * 8, 60);
		}
	}



	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

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

int vehicle_in_inters(vehicle &car)
{
	for (int i = 0; i < num_intersetions; i++)
	{
		int size = intersections[i].size / 2;
		if ((car.x > intersections[i].x - size) && (car.x < intersections[i].x + size) && (car.y > intersections[i].y - size) && (car.y < intersections[i].y + size))
		{
			if (!car.busy)
			{
				car.busy = true;
				if (!car.path_traversed)
					return 0;
				for (int j = 0; j < 4; j++)
				{
					if (intersections[i].directions[j].main)
					{
						float angle = car.getAngle(intersections[i].directions[j].dir);
						if (angle > 0)
						{
							car.angle_of_wheel = -0.04;

							car.old_x = car.x;
							car.old_y = car.y;
						}
						if (angle < 0)
						{
							car.angle_of_wheel = 0.04;
							car.old_x = car.x;
							car.old_y = car.y;
							car.distance = 10;

							if (car.path_traversed)
							{
								cout << "zzz";
								car.path_traversed = false;
							}
						}
						if (angle == 0)
						{
							car.angle_of_wheel = 0;
							car.distance = -1;
						}
						car.angle_of_rotate = M_PI / 2;
					}
				}
				car.rotated = false;
				return i;
			}
		}
		else
			car.busy = false;
	}
	return -1;
}

void move()
{
	for (int i = 0; i < c.getTop(); i++)
	{

		c.Peek(i).move_forward();
		vehicle_in_inters(c.Peek(i));

		if (!c.Peek(i).rotated && c.Peek(i).path_traversed)
		{
			c.Peek(i).rotate_the_car();
		}

	}
}

void updategame()
{
	// ИСПРАВАИТЬ
	int const numVertex = 8 * 3;
	//GLfloat vertex[max_veh * numVertex + (num_intersetions - 1) * 2 * 3 * 2];
	GLfloat vertex[max_veh * numVertex + 250]; //186 for 5

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
		vertex[(i * numVertex) + j++] = car.xCoord[0];
		vertex[(i * numVertex) + j++] = car.yCoord[0];
		vertex[(i * numVertex) + j++] = 0.0;
		vertex[(i * numVertex) + j++] = car.xCoord[3];
		vertex[(i * numVertex) + j++] = car.yCoord[3];
		vertex[(i * numVertex) + j++] = 0.0;
		vertex[(i * numVertex) + j++] = car.xCoord[2];
		vertex[(i * numVertex) + j++] = car.yCoord[2];
		vertex[(i * numVertex) + j++] = 0.0;
		// Triangle 2
		vertex[(i * numVertex) + j++] = car.xCoord[1];
		vertex[(i * numVertex) + j++] = car.yCoord[1];
		vertex[(i * numVertex) + j++] = 0.0;
		vertex[(i * numVertex) + j++] = car.xCoord[2];
		vertex[(i * numVertex) + j++] = car.yCoord[2];
		vertex[(i * numVertex) + j++] = 0.0;
		vertex[(i * numVertex) + j++] = car.xCoord[0];
		vertex[(i * numVertex) + j++] = car.yCoord[0];
		vertex[(i * numVertex) + j++] = 0.0;
		//Vector
		vertex[(i * numVertex) + j++] = (x)+(car.direction.x) * 50;
		vertex[(i * numVertex) + j++] = (y)+(car.direction.y) * 50;
		vertex[(i * numVertex) + j++] = 0.0;
		vertex[(i * numVertex) + j++] = (x);
		vertex[(i * numVertex) + j++] = (y);
		vertex[(i * numVertex) + j++] = 0.0;

		//ИСПАРВИТЬ: Карта расчитывается единожды

		if (i == c.getTop() - 1)
		{
			int index = (i * numVertex) + j;
			for (int k = 0; k < num_intersetions; k++)
			{
				for (int n = 0; n < 4; n++)
				{
					if (intersections[k].directions[n].dir.x == 0 && intersections[k].directions[n].dir.y == 0)
					{
						//cout << "k = " << k << " n = " << n << endl;
						switch (n)
						{
						case 0:
							vertex[index++] = intersections[k].x - intersections[k].size / 2;
							vertex[index++] = intersections[k].y + intersections[k].size / 2;
							vertex[index++] = 0.0;

							vertex[index++] = intersections[k].x + intersections[k].size / 2;
							vertex[index++] = intersections[k].y + intersections[k].size / 2;
							vertex[index++] = 0.0;


							break;

						case 1:
							vertex[index++] = intersections[k].x + intersections[k].size / 2;
							vertex[index++] = intersections[k].y + intersections[k].size / 2;
							vertex[index++] = 0.0;

							vertex[index++] = intersections[k].x + intersections[k].size / 2;
							vertex[index++] = intersections[k].y - intersections[k].size / 2;
							vertex[index++] = 0.0;

							break;

						case 2:
							vertex[index++] = intersections[k].x - intersections[k].size / 2;
							vertex[index++] = intersections[k].y - intersections[k].size / 2;
							vertex[index++] = 0.0;

							vertex[index++] = intersections[k].x + intersections[k].size / 2;
							vertex[index++] = intersections[k].y - intersections[k].size / 2;
							vertex[index++] = 0.0;

							break;

						case 3:
							vertex[index++] = intersections[k].x - intersections[k].size / 2;
							vertex[index++] = intersections[k].y + intersections[k].size / 2;
							vertex[index++] = 0.0;

							vertex[index++] = intersections[k].x - intersections[k].size / 2;
							vertex[index++] = intersections[k].y - intersections[k].size / 2;
							vertex[index++] = 0.0;

							break;
						}
					}
					else
					{
						switch (n)
						{
						case 0:
							if (intersections[k].connections[n] != -1)
							{
								vertex[index++] = intersections[k].x - intersections[k].size / 2;
								vertex[index++] = intersections[k].y + intersections[k].size / 2;
								vertex[index++] = 0.0;

								vertex[index++] = intersections[intersections[k].connections[n]].x - intersections[intersections[k].connections[n]].size / 2;
								vertex[index++] = intersections[intersections[k].connections[n]].y - intersections[intersections[k].connections[n]].size / 2;
								vertex[index++] = 0.0;

								vertex[index++] = intersections[k].x + intersections[k].size / 2;
								vertex[index++] = intersections[k].y + intersections[k].size / 2;
								vertex[index++] = 0.0;

								vertex[index++] = intersections[intersections[k].connections[n]].x + intersections[intersections[k].connections[n]].size / 2;
								vertex[index++] = intersections[intersections[k].connections[n]].y - intersections[intersections[k].connections[n]].size / 2;
								vertex[index++] = 0.0;

							}

							break;


						case 1:
							if (intersections[k].connections[n] != -1)
							{
								vertex[index++] = intersections[k].x + intersections[k].size / 2;
								vertex[index++] = intersections[k].y + intersections[k].size / 2;
								vertex[index++] = 0.0;

								vertex[index++] = intersections[intersections[k].connections[n]].x - intersections[intersections[k].connections[n]].size / 2;
								vertex[index++] = intersections[intersections[k].connections[n]].y + intersections[intersections[k].connections[n]].size / 2;
								vertex[index++] = 0.0;

								vertex[index++] = intersections[k].x + intersections[k].size / 2;
								vertex[index++] = intersections[k].y - intersections[k].size / 2;
								vertex[index++] = 0.0;

								vertex[index++] = intersections[intersections[k].connections[n]].x - intersections[intersections[k].connections[n]].size / 2;
								vertex[index++] = intersections[intersections[k].connections[n]].y - intersections[intersections[k].connections[n]].size / 2;
								vertex[index++] = 0.0;
							}
							break;

						case 2:
							if (intersections[k].connections[n] != -1)
							{
								vertex[index++] = intersections[k].x - intersections[k].size / 2;
								vertex[index++] = intersections[k].y - intersections[k].size / 2;
								vertex[index++] = 0.0;

								vertex[index++] = intersections[intersections[k].connections[n]].x - intersections[intersections[k].connections[n]].size / 2;
								vertex[index++] = intersections[intersections[k].connections[n]].y + intersections[intersections[k].connections[n]].size / 2;
								vertex[index++] = 0.0;

								vertex[index++] = intersections[k].x + intersections[k].size / 2;
								vertex[index++] = intersections[k].y - intersections[k].size / 2;
								vertex[index++] = 0.0;

								vertex[index++] = intersections[intersections[k].connections[n]].x + intersections[intersections[k].connections[n]].size / 2;
								vertex[index++] = intersections[intersections[k].connections[n]].y + intersections[intersections[k].connections[n]].size / 2;
								vertex[index++] = 0.0;
							}

							break;

						case 3:
							if (intersections[k].connections[n] != -1)
							{
								vertex[index++] = intersections[k].x - intersections[k].size / 2;
								vertex[index++] = intersections[k].y + intersections[k].size / 2;
								vertex[index++] = 0.0;

								vertex[index++] = intersections[intersections[k].connections[n]].x + intersections[intersections[k].connections[n]].size / 2;
								vertex[index++] = intersections[intersections[k].connections[n]].y + intersections[intersections[k].connections[n]].size / 2;
								vertex[index++] = 0.0;

								vertex[index++] = intersections[k].x - intersections[k].size / 2;
								vertex[index++] = intersections[k].y - intersections[k].size / 2;
								vertex[index++] = 0.0;

								vertex[index++] = intersections[intersections[k].connections[n]].x + intersections[intersections[k].connections[n]].size / 2;
								vertex[index++] = intersections[intersections[k].connections[n]].y - intersections[intersections[k].connections[n]].size / 2;
								vertex[index++] = 0.0;
							}
							break;
						}
					}
				}
			}
		}

		j = 0;

	}

	glBindBuffer(GL_ARRAY_BUFFER, VertextBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_DYNAMIC_DRAW);

	//fpscalculate();
}


void timer(int = 0)
{
	updategame();

	glutTimerFunc(22, timer, 0);
}


void timer2(int = 0)
{
	display();
	glutTimerFunc(0, timer2, 0);
}

void MouseFunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		cout << c.Peek(0).getDistance() << endl;
		cout << c.Peek(0).distance << endl;
	}
}


int main(int argc, char** argv) {
	vehicle car;
	for (int i = 0; i < max_veh; i++)
	{
		car.x = car.old_x = (i - max_veh / 2) * 0 + 5;
		car.y = car.old_y = -0 - (i - max_veh / 2) * 15;
		car.speed = 0.20;
		car.distance = -1;
		car.path_traversed = true;

		car.direction = normolize({ 0.0, 1.0, 0.0 });
		car.acceleration = 0.01;
		car.deceleration = 0.0009;
		car.width = 3;
		car.height = 6;
		car.idVehicle = i;
		car.position = i;

		car.rotated = true;
		car.busy = false;
		car.common_angle = 0.0;
		car.angle_of_wheel = 0.02;
		car.angle_of_rotate = M_PI;
		car.old_dir = car.direction;

		float w2 = car.width / 2;
		float h2 = car.height / 2;

		if (car.direction.y > 0)
		{

			car.xCoord[0] = car.x - w2;
			car.yCoord[0] = car.y + h2;

			car.xCoord[1] = car.x + w2;
			car.yCoord[1] = car.y + h2;

			car.xCoord[2] = car.x + w2 + 0.5;
			car.yCoord[2] = car.y - h2;

			car.xCoord[3] = car.x - w2 - 0.5;
			car.yCoord[3] = car.y - h2;
		}

		if (car.direction.y < 0)
		{

			car.xCoord[0] = car.x - w2;
			car.yCoord[0] = car.y - h2;

			car.xCoord[1] = car.x + w2;
			car.yCoord[1] = car.y - h2;

			car.xCoord[2] = car.x + w2 + 0.5;
			car.yCoord[2] = car.y + h2;

			car.xCoord[3] = car.x - w2 - 0.5;
			car.yCoord[3] = car.y + h2;
		}

		c.push(car);
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(728, 728);
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
	glutMouseFunc(MouseFunc);
	glutTimerFunc(0, timer, 0);
	glutTimerFunc(0, timer2, 0);
	glutMainLoop();
}

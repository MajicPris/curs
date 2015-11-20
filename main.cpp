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

	//glm::mat4 Scal = glm::scale(glm::vec3(3.0, 3.0, 1.0));

	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

	glm::mat4 Model = glm::mat4(1.0f);

	MVP = Projection * View * Model;

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
	glFlush();
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
	glutInitDisplayMode(GLUT_RGBA);
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
	glutMainLoop();
}

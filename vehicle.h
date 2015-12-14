#include <glm\glm.hpp>
#include <glm\ext.hpp> 
#include <math.h>

using namespace std;

class vehicle {
public:
	float x, y;
	glm::vec3 destination;
	float speed;
	float acceleration, deceleration;
	float width, height;
	float weight;
	int idVehicle;
	int position;
	

	vehicle();
	vehicle(float, float, float, glm::vec3, float, float, float, float, float, int, int);

	float getAngle();
};

vehicle::vehicle()
{
	x = 0.0, y = 0.0;
	speed = 0.0;
	destination = { 0.0, 0.0, 0.0 }; //glm::vec3(0.0, 0.0, 0.0)
	acceleration = 0.0, deceleration = 0.0;
	width = 0.0, height = 0.0;
	weight = 0.0;
	idVehicle = 0;
	position = 0;
}

vehicle::vehicle(float xx, float yy,float s, glm::vec3 des, float a, float d, float w, float h, float weig, int i, int p)
{
	x = xx, y = yy;
	speed = s;
	destination = des; //glm::vec3(0.0, 0.0, 0.0)
	acceleration = a, deceleration = d;
	width = w, height = h;
	weight = weig;
	idVehicle = i;
	position = p;
}



float vehicle::getAngle()
{
	return glm::angle(destination, glm::vec3(0.0, 1.0, 0.0));
}



#include <glm\glm.hpp>
#include <glm\ext.hpp> 
using namespace std;


class vehicle {

public:
	float x, y;
	glm::vec3 speed;
	float acceleration, deceleration;
	float width, height;
	float weight;
	int idVehicle;
	int position;

	vehicle();
	vehicle(float, float, glm::vec3, float, float, float, float, float, int, int);

};

vehicle::vehicle(){
	x = 0.0, y = 0.0;
	speed = {0.0, 0.0, 0.0}; //glm::vec3(0.0, 0.0, 0.0)
	acceleration = 0.0, deceleration = 0.0;
	width = 0.0, height = 0.0;
	weight = 0.0;
	idVehicle = 0;
	position = 0;
}

vehicle::vehicle(float xx, float yy, glm::vec3 s, float a, float d, float w, float h, float weig, int i, int p){
	x = xx, y = yy;
	speed = s; //glm::vec3(0.0, 0.0, 0.0)
	acceleration = a, deceleration = d;
	width = w, height = h;
	weight = weig;
	idVehicle = i;
	position = p;
}

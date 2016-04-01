#include <glm\glm.hpp>
#include <glm\ext.hpp> 
#include <math.h>

using namespace std;

class steering_wheel
{
public:


};

class vehicle {
public:
	float x, y;
	float xCoord[4], yCoord[4];
	glm::vec3 direction;
	float speed;
	float acceleration, deceleration;
	float width, height;
	float weight;
	int idVehicle;
	int position;



	vehicle();
	vehicle(float, float, float, glm::vec3, float, float, float, float, float, int, int);

	float getAngle();
	float getAngle(glm::vec2 vec);
	void checkSpeed(vehicle v1);
	void checkDis(vehicle v1);
	void rotate(float angle);


	float angle_of_wheel;
	float angle_of_rotate;
	float common_angle;
	glm::vec3 old_dir;
	bool rotated;
	bool busy;
	void rotate_the_car();

	float old_x, old_y;
	float distance;
	bool path_traversed;
	void move_forward();
	float getDistance();
};

glm::vec3 normolize(glm::vec3);
glm::vec2 normolize(glm::vec2);


vehicle::vehicle()
{
	x = 0.0, y = 0.0;
	speed = 0.0;
	direction = { 0.0, 0.0, 0.0 }; //glm::vec3(0.0, 0.0, 0.0)
	acceleration = 0.0, deceleration = 0.0;
	width = 3, height = 6;
	weight = 0.0;
	idVehicle = 0;
	position = 0;

	angle_of_wheel = 0;
	old_dir = { 0.0, 0.0, 0.0 };
	rotated = true;

	float w2 = width / 2;
	float h2 = height / 2;

	xCoord[0] = x - w2;
	yCoord[0] = y + h2;

	xCoord[1] = x + w2;
	yCoord[1] = y + h2;

	xCoord[2] = x + w2;
	yCoord[2] = y - h2;

	xCoord[3] = x - w2;
	yCoord[3] = y - h2;

}

vehicle::vehicle(float xx, float yy, float s, glm::vec3 des, float a, float d, float w, float h, float weig, int i, int p)
{
	x = xx, y = yy;
	speed = s;
	direction = normolize(des); //glm::vec3(0.0, 0.0, 0.0)
	acceleration = a, deceleration = d;
	width = w, height = h;
	weight = weig;
	idVehicle = i;
	position = p;

	float w2 = width / 2;
	float h2 = height / 2;

	xCoord[0] = x - w2;
	yCoord[0] = y + h2;

	xCoord[1] = x + w2;
	yCoord[1] = y + h2;

	xCoord[2] = x + w2;
	yCoord[2] = y - h2;

	xCoord[3] = x - w2;
	yCoord[3] = y - h2;
}

glm::vec3 normolize(glm::vec3 des)
{
	float lenght = sqrt(des.x * des.x + des.y * des.y + des.z * des.z);
	glm::vec3 res;
	res.x = des.x / lenght;
	res.y = des.y / lenght;
	res.z = des.z / lenght;
	return res;
}

glm::vec2 normolize(glm::vec2 des)
{
	float lenght = sqrt(des.x * des.x + des.y * des.y);
	glm::vec2 res;
	res.x = des.x / lenght;
	res.y = des.y / lenght;
	return res;
}

float vehicle::getAngle()
{

	float f = glm::orientedAngle(glm::vec2(direction.x, direction.y), normolize(glm::vec2(((xCoord[0] + xCoord[1]) / 2) - x, ((yCoord[0] + yCoord[1]) / 2) - y)));
	//float f = glm::angle(direction, normolize(glm::vec3(((xCoord[0] + xCoord[1]) / 2) - x, ((yCoord[0] + yCoord[1]) / 2) - y, 0.0)));
	return -f;
}

float vehicle::getAngle(glm::vec2 vec)
{
	float f = glm::orientedAngle(glm::vec2(direction.x, direction.y), normolize(vec));
	//float f = glm::angle(direction, normolize(glm::vec3(((xCoord[0] + xCoord[1]) / 2) - x, ((yCoord[0] + yCoord[1]) / 2) - y, 0.0)));
	return -f;
}


void vehicle::checkDis(vehicle v1)
{
	float dis = sqrt(pow((this->x - v1.x), 2) + pow((this->y - v1.y), 2));
	if (dis > this->speed * 200)
		this->speed += this->acceleration;
	if (dis < this->speed * 200)
		this->speed -= this->deceleration;

}

void vehicle::checkSpeed(vehicle v1)
{
	if (this->speed < v1.speed)
	{
		this->speed += this->acceleration;
	}
	if (this->speed > v1.speed)
	{
		this->speed -= this->deceleration;
	}

}

void vehicle::rotate(float angle){
	float x1, y1;
	for (int j = 0; j < 4; j++)
	{
		x1 = this->x + (xCoord[j] - this->x) * cos(angle) - (yCoord[j] - this->y) * sin(angle);
		y1 = this->y + (yCoord[j] - this->y) * cos(angle) + (xCoord[j] - this->x) * sin(angle);
		xCoord[j] = x1;
		yCoord[j] = y1;
	}
}

void vehicle::rotate_the_car(){
	if (abs(this->common_angle) < abs(this->angle_of_rotate))
	{
		float x1, y1;
		float angle = angle_of_wheel;
		float a = (abs((abs(this->angle_of_rotate) - abs(this->common_angle))));
		if (a < abs(this->angle_of_wheel))
		{
			if (angle_of_wheel < 0)
				angle = -a;
			else
				angle = a;
			this->rotated = true;
			this->path_traversed = true;
			this->distance = -1;
		}
		this->common_angle += angle;
		x1 = (this->direction.x) * cos(angle) - (this->direction.y) * sin(angle);
		y1 = (this->direction.y) * cos(angle) + (this->direction.x) * sin(angle);
		this->direction.x = x1;
		this->direction.y = y1;
		for (int j = 0; j < 4; j++)
		{
			x1 = this->x + (xCoord[j] - this->x) * cos(angle) - (yCoord[j] - this->y) * sin(angle);
			y1 = this->y + (yCoord[j] - this->y) * cos(angle) + (xCoord[j] - this->x) * sin(angle);
			xCoord[j] = x1;
			yCoord[j] = y1;
		}
	}
	else
	{
		this->common_angle = 0;
		this->rotated = true;
		this->path_traversed = true;
		this->distance = -1;
	}


}

float vehicle::getDistance()
{
	float a = sqrt((y - old_y) * (y - old_y) + (x - old_x) * (x - old_x));
	return a;
}

void vehicle::move_forward()
{
	float d;
	d = getDistance();
	// if (distance > (d = getDistance()))
	if (!path_traversed)
	{
		d = distance - d;
		if (d <= direction.x * speed)
		{
			x += d;
			y += d;

			for (int j = 0; j < 4; j++)
			{
				xCoord[j] += d;
				yCoord[j] += d;
			}
		}
		else
		{
			x += direction.x * speed;
			y += direction.y * speed;

			for (int j = 0; j < 4; j++)
			{
				xCoord[j] += direction.x * speed;
				yCoord[j] += direction.y * speed;
			}
		}

		if (getDistance() >= distance)
		{
			cout << "getDistance = " << getDistance() << " distance = " << distance << endl;
			distance = -1;
			path_traversed = true;
			return;
		}

	}

	if (distance == -1)
	{

		x += direction.x * speed;
		y += direction.y * speed;

		for (int j = 0; j < 4; j++)
		{
			xCoord[j] += direction.x * speed;
			yCoord[j] += direction.y * speed;
		}
	}
}

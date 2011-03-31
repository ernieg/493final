#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <zenilib.h>
#include <vector>

class GameObject
{
public:
	void render();

private:
	Zeni::Point3f position;
	Zeni::Vector3f size;
	Zeni::Quaternion orientation;
	Zeni::Vector3f velocity;
	Zeni::Model* model;
}

#endif // GAMEOBJECT_H
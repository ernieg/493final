#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <zenilib.h>
#include <vector>

class GameObject
{
public:
	void render();

	Zeni::Point3f getPosition();
	void setPosition(Zeni::Point3f position_);

	Zeni::Vector3f getSize();
	void setSize(Zeni::Vector3f& size_);

	Zeni::Quaternion getOrientation();
	void setOrientation(Zeni::Quaternion& orientation_);

	Zeni::Vector3f getVelocity();
	void setVelocity(Zeni::Vector3f velocity_);

	Zeni::Model* getModel();
	void setModel(Zeni::Model* model_);

private:
	Zeni::Point3f position;
	Zeni::Vector3f size;
	Zeni::Quaternion orientation;
	Zeni::Vector3f velocity;
	Zeni::Model* model;
};

#endif // GAMEOBJECT_H
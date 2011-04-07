#include "zenilib.h"
#include "GameObject.h"

Zeni::Point3f GameObject::getPosition()
{
	return position;
}

void GameObject::setPosition(Zeni::Point3f position_)
{
	position = position_;
}

Zeni::Vector3f GameObject::getSize()
{
	return size;
}

void GameObject::setSize(Zeni::Vector3f& size_)
{
	size = size_;
}

Zeni::Quaternion GameObject::getOrientation()
{
	return orientation;
}

void GameObject::setOrientation(Zeni::Quaternion orientation_)
{
	orientation = orientation_;
}

Zeni::Vector3f GameObject::getVelocity()
{
	return velocity;
}

void GameObject::setVelocity(Zeni::Vector3f velocity_)
{
	velocity = velocity_;
}

Zeni::Model* GameObject::getModel()
{
	return model;
}

void GameObject::setModel(Zeni::Model* model_)
{
	model = model_;
}

void GameObject::render()
{

}
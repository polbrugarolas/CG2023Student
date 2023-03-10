#include "light.h"
#include "main/includes.h"
#include <iostream>

Light::Light()
{
	this->pos = Vector3();
	this->Id = Vector3();
	this->Is = Vector3();
}

Light::Light(Vector3 pos, Vector3 Id, Vector3 Is)
{
	this->pos = pos;
	this->Id = Id;
	this->Is = Is;
}

#pragma once
#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include "mesh.h"
#include "utils.h"
#include "camera.h"


class Light
{
public:
	//**ATRIBUTES**
	Vector3 pos;
	Vector3 Id;
	Vector3 Is;

	//**FUNCTIONS**
	//Constructors
	Light();
	Light(Vector3 pos, Vector3 Id, Vector3 Is);
};
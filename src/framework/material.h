#pragma once
#include "main/includes.h"
#include <iostream>
#include "framework.h"
#include "image.h"
#include "mesh.h"
#include "utils.h"
#include "camera.h"
#include "shader.h"
#include "texture.h"
#include "light.h"

class Material
{
public:
	//**ATRIBUTES**
	Shader* shader;
	Vector3 ka, kd, ks;
	float alpha;
	Texture* difuse_specular;
	Texture* normal;

	//**FUNCTIONS**
	//Constructors
	Material();
	Material(Shader* shader, Vector3 ka, Vector3 kd, Vector3 ks, float alpha, Texture* difuse_specular, Texture* normal);
};
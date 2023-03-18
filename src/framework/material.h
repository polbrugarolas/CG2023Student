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

struct sLight {
	Vector3 position;
	Vector3 color_diffuse;
	Vector3 color_specular;
	
};
struct sUniformData {
	Matrix44 u_model;
	Matrix44 u_viewprojection;
	sLight light;
	Vector3 ambient_light;
	Texture* texture;
};

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
	void Enable(const sUniformData& uniformData);
	void Disable();	
	
};
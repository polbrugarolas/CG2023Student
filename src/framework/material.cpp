#include "material.h"

Material::Material()
{
	this->shader = new Shader();
	this->ka = Vector3();
	this->kd = Vector3();
	this->ks = Vector3();
	this->alpha = float();
	this->difuse_specular = new Texture;
	this->normal = new Texture;
}

Material::Material(Shader* shader, Vector3 ka, Vector3 kd, Vector3 ks, float alpha, Texture* difuse_specular, Texture* normal)
{
	this->shader = shader;
	this->ka = ka;
	this->kd = kd;
	this->ks = ks;
	this->alpha = alpha;
	this->difuse_specular = difuse_specular;
	this->normal = normal;
}

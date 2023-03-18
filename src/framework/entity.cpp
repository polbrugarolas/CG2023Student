#include "entity.h"
#include "framework.h"
#include "main/includes.h"
#include <iostream>


//CONSTRUCTORS

Entity::Entity() { this->matrix_e = Matrix44(); }
Entity::Entity(Matrix44 mtx) {
	this->matrix_e = mtx;
}
Entity::Entity(Mesh* msh) {
	this->matrix_e = Matrix44();
	this->mesh_e = *msh;
}
Entity::Entity(Matrix44 mtx, Mesh* msh) {
	this->matrix_e = mtx;
	this->mesh_e = *msh;
}

void Entity::Render() {
	this->mesh_e.Render();
}

void Entity::Render(sUniformData* uniformData) {
	material.Enable(*uniformData);
	material.Disable();

}

//FUNCTIONS
void Entity::Render(Image* framebuffer, Camera* camera, const Color& c) {
	const std::vector<Vector3>& vertices = mesh_e.GetVertices();
	int size = vertices.size();
	Vector3 v[3];
	bool negZ[3];

	for (int i = 0; i < size; i)
	{
		for (int j = 0; j < 3; j++)
		{
			v[j] = vertices[i];
			i++;
			v[j] = matrix_e * v[j];
			v[j] = camera->ProjectVector(v[j], negZ[j]);
		}
		if (negZ[0] == false && negZ[1] == false && negZ[2] == false) {
			for (size_t j = 0; j < 3; j++)
			{
				v[j].x = ((v[j].x + 1) / 2) * framebuffer->width;
				v[j].y = ((v[j].y + 1) / 2) * framebuffer->height;
			}
			framebuffer->DrawLineBresenham(v[0].x, v[0].y, v[1].x, v[1].y, c);
			framebuffer->DrawLineBresenham(v[1].x, v[1].y, v[2].x, v[2].y, c);
			framebuffer->DrawLineBresenham(v[2].x, v[2].y, v[0].x, v[0].y, c);
		}
	}
}

void Entity::Render_raster(Image* framebuffer, Camera* camera, const Color& c) {
	const std::vector<Vector3>& vertices = mesh_e.GetVertices();
	int size = vertices.size();
	Vector3 v[3];
	bool negZ[3];

	for (int i = 0; i < size; i)
	{
		for (int j = 0; j < 3; j++)
		{
			v[j] = vertices[i];
			i++;
			v[j] = matrix_e * v[j];
			v[j] = camera->ProjectVector(v[j], negZ[j]);
		}
		if (negZ[0] == false && negZ[1] == false && negZ[2] == false) {
			for (size_t j = 0; j < 3; j++)
			{
				v[j].x = ((v[j].x + 1) / 2) * framebuffer->width;
				v[j].y = ((v[j].y + 1) / 2) * framebuffer->height;
			}
			framebuffer->DrawTriangle(Vector2(v[0].x, v[0].y), Vector2(v[1].x, v[1].y), Vector2(v[2].x, v[2].y), c);
		}
	}
}

void Entity::Render_interpol(Image* framebuffer, Camera* camera) {
	const std::vector<Vector3>& vertices = mesh_e.GetVertices();
	int size = vertices.size();
	Vector3 v[3];
	bool negZ[3];

	for (int i = 0; i < size; i)
	{
		for (int j = 0; j < 3; j++)
		{
			v[j] = vertices[i];
			i++;
			v[j] = matrix_e * v[j];
			v[j] = camera->ProjectVector(v[j], negZ[j]);
		}
		if (negZ[0] == false && negZ[1] == false && negZ[2] == false) {
			for (size_t j = 0; j < 3; j++)
			{
				v[j].x = ((v[j].x + 1) / 2) * framebuffer->width;
				v[j].y = ((v[j].y + 1) / 2) * framebuffer->height;
			}
			framebuffer->DrawTriangleInterpolated(v[0], v[1], v[2], Color::RED,Color::BLUE,Color::GREEN);
		}
	}
}

void Entity::Render_occlu(Image* framebuffer, Camera* camera, FloatImage* zBuffer) {
	const std::vector<Vector3>& vertices = mesh_e.GetVertices();
	int size = vertices.size();
	Vector3 v[3];
	bool negZ[3];
	zBuffer->Fill(INT_MAX);

	for (int i = 0; i < size; i)
	{
		for (int j = 0; j < 3; j++)
		{
			v[j] = vertices[i];
			i++;
			v[j] = matrix_e * v[j];
			v[j] = camera->ProjectVector(v[j], negZ[j]);
		}
		if (negZ[0] == false && negZ[1] == false && negZ[2] == false) {
			for (size_t j = 0; j < 3; j++)
			{
				v[j].x = ((v[j].x + 1) / 2) * framebuffer->width;
				v[j].y = ((v[j].y + 1) / 2) * framebuffer->height;
			}
			framebuffer->DrawTriangleInterpolated_occlu(v[0], v[1], v[2], Color::RED, Color::BLUE, Color::GREEN, zBuffer);
		}
	}
}
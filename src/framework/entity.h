
#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include "mesh.h"
#include "utils.h"
#include "camera.h"


class Entity
{
public:
	//**ATRIBUTES**
	Matrix44 matrix_e;
	Mesh mesh_e;

	//**FUNCTIONS**
	//Constructors
	Entity();
	Entity(Matrix44 mtx);
	Entity(Mesh* msh);
	Entity(Matrix44 mtx, Mesh* msh);
	void Render(Image* framebuffer, Camera* camera, const Color& c);
	void Render_raster(Image* framebuffer, Camera* camera, const Color& c);
	void Render_interpol(Image* framebuffer, Camera* camera);
	void Render_occlu(Image* framebuffer, Camera* camera, FloatImage* zBuffer);
};



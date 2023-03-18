#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 
#include "entity.h"
#include "material.h"

Mesh* quad;
Shader* general;
Shader* shader;
Shader* shader_transform;
Shader* shader_entity;
float my_customtime;
Shader* shader_texture;
Texture* fruits;
Texture* street;
Texture* entity_texture;
Material* material;
sUniformData* data;

Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	int w,h;
	SDL_GetWindowSize(window,&w,&h);

	this->mouse_state = 0;
	this->time = 0.f;
	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(nullptr);

	this->framebuffer.Resize(w, h);
	this->option = -1;
	this->exercice = 0;
	this->RenderMode = 0;
	this->Occlu = 0;
	this->total_t = 0;
	this->dragging = false;
	this->mouse_x = 0;
	this->mouse_y = 0;
	this->mouse_z = 0;
	this->mouse_drag = false;
	this->prev_mouse = Vector2(0, 0);


}

Application::~Application()
{
}

void Application::Init(void)
{
	std::cout << "Initiating app..." << std::endl;
	Mesh* mesh1 = new Mesh();
	mesh1->LoadOBJ("meshes/lee.obj");
	entity1 = new Entity(mesh1);
	Mesh* mesh2 = new Mesh();
	mesh2->LoadOBJ("meshes/anna.obj");
	entity2 = new Entity(mesh2);
	entity2->matrix_e.Translate(0.5, 0, 0);

	cam = new Camera();
	cam->eye.Set(1, 0, 1);
	cam->center.Set(0, 0, 0);
	cam->up.Set(0, 1, 0);
	cam->LookAt(cam->eye, cam->center, cam->up);
	cam->SetPerspective(45, float(framebuffer.width / framebuffer.height), 0.01, 100);

	zbuffer = new FloatImage(framebuffer.width, framebuffer.height);
	zbuffer->Fill(INT_MAX);

	quad = new Mesh();
	quad->CreateQuad();
	shader = Shader::Get("shaders/quad.vs", "shaders/quad.fs");
	shader_texture = Shader::Get("shaders/quad.vs", "shaders/quad_texture.fs");
	shader_transform = Shader::Get("shaders/quad.vs", "shaders/quad_transform.fs");
	shader_entity = Shader::Get("shaders/simple.vs", "shaders/simple.fs");

	my_customtime = 0;
	fruits = new Texture;
	fruits->Load("images/fruits.png");
	street = new Texture;
	street->Load("images/street.png");
	entity_texture = new Texture;
	entity_texture->Load("textures/lee_color_specular.tga");

	data = new sUniformData();
	
	Vector3 ambientL = (0,0,0);

	data->ambient_light = ambientL;
	data->light.color_diffuse = Vector3(1, 1, 1);
	data->light.color_specular = Vector3(1, 1, 1);
	data->light.position = Vector3(1, 1, 1);
	data->u_model = entity1->matrix_e;
	data->u_viewprojection = cam->GetViewProjectionMatrix();
	data->texture = entity_texture;
	
	material = new Material;
	material->ka = Vector3(1, 1, 1);
	material->kd = Vector3(1, 1, 1);
	material->ks = Vector3(1, 1, 1);
	material->shader = shader;
	entity1->material = *material;
	

}

// Render one frame
void Application::Render(void)
{
	
	if (exercice == 0) {
		
		entity1->Render(data);

	}
	
}

// Called after render
void Application::Update(float seconds_elapsed)
{	
	cam->eye.Set(1.5*sin(time / 2), 0.5, 1.5*cos(time / 2)); 
	cam->LookAt(cam->eye, cam->center, cam->up);
}

//keyboard press event 
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
	switch(event.keysym.sym) {
		case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
		case SDLK_0: exercice = 0; option = 0; break;
	}
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
		if (option == 3) {
			dragging = true;
		}
	}
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {
		if (option == 3) {
			dragging = false;
		}
	}
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
	
}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}
#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 
#include "entity.h"

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
	this->option = 1;
	this->total_t = 0;
}

Application::~Application()
{
	SDL_DestroyWindow(window);
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
	cam->eye.Set(0, 0, 2);
	cam->center.Set(0, 0, 0);
	cam->up.Set(0, 1, 0);
	cam->LookAt(cam->eye, cam->center, cam->up);
	cam->SetPerspective(45, float(framebuffer.width / framebuffer.height), 0.01, 100);
	
}

// Render one frame
void Application::Render(void)
{
	if (option == 1) {
		entity1->Render(&framebuffer, cam, Color(255, 255, 255));
		entity2->Render(&framebuffer, cam, Color(255, 0, 0));
	}
	if (option == 2){
		entity1->Render(&framebuffer, cam, Color(255, 255, 255));
		entity2->Render(&framebuffer, cam, Color(255, 0, 0));
	}	

	framebuffer.Render();
	framebuffer.Fill(Color::BLACK);

}

// Called after render
void Application::Update(float seconds_elapsed)
{
	if (option == 2) {
		total_t = total_t + seconds_elapsed;
		entity2->matrix_e = Matrix44();
		entity2->matrix_e.Translate(-0.5 * cos(0.5 * total_t), 0, 0.5 * sin(0.5 * total_t));
	}

}

//keyboard press event 
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
	switch(event.keysym.sym) {
		case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
		case SDLK_1: option = 1; cam->eye.Set(0, 0, 2); cam->LookAt(cam->eye, cam->center, cam->up); break;
		case SDLK_2: option = 2; cam->eye.Set(0, 2, 2); cam->LookAt(cam->eye, cam->center, cam->up); break;
	}
}

void Application::OnMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {

	}
}

void Application::OnMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) {

	}
}

void Application::OnMouseMove(SDL_MouseButtonEvent event)
{
	
}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}
#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 
#include "entity.h"

Mesh* quad;
Shader* shader;
float my_customtime;

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
	this->option = 0;
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
	cam->eye.Set(0, 0, 2);
	cam->center.Set(0, 0, 0);
	cam->up.Set(0, 1, 0);
	cam->LookAt(cam->eye, cam->center, cam->up);
	cam->SetPerspective(45, float(framebuffer.width / framebuffer.height), 0.01, 100);

	zbuffer = new FloatImage(framebuffer.width, framebuffer.height);
	zbuffer->Fill(INT_MAX);

	quad = new Mesh();
	quad->CreateQuad();
	shader = Shader::Get("shaders/quad.vs", "shaders/quad.fs");
	my_customtime = 0;
	
}

// Render one frame
void Application::Render(void)
{
	/*if (option == 1) {
		if (RenderMode == 0) {
			entity1->Render_raster(&framebuffer, cam, Color(255, 255, 255));
		}
		else {
			entity1->Render_interpol(&framebuffer, cam);
		}
		entity2->Render(&framebuffer, cam, Color(255, 0, 0));
	}
	if (option == 2){
		entity1->Render(&framebuffer, cam, Color(255, 255, 255));
		entity2->Render(&framebuffer, cam, Color(255, 0, 0));
	}
	if (option == 3) {
		entity1->Render(&framebuffer, cam, Color(255, 255, 255));
		entity2->Render(&framebuffer, cam, Color(255, 0, 0));
	}
	if (option == 4) {
		if (RenderMode == 0) {
			entity1->Render_raster(&framebuffer, cam, Color(255, 255, 255));
		}
		else {
			if (Occlu == 0) {
				entity1->Render_interpol(&framebuffer, cam);
			}
			else {
				entity1->Render_occlu(&framebuffer, cam,zbuffer);
			}
		}
	}


	framebuffer.Render();
	framebuffer.Fill(Color::BLACK);
	*/

	shader->Enable();
	
	shader->SetUniform1("option", option);
	shader->SetFloat("u_time2", time - my_customtime);
	shader->SetFloat("u_timeinf", time);
	
	quad->Render();
	shader->Disable();
}

// Called after render
void Application::Update(float seconds_elapsed)
{

}

//keyboard press event 
void Application::OnKeyPressed( SDL_KeyboardEvent event )
{
	// KEY CODES: https://wiki.libsdl.org/SDL2/SDL_Keycode
	switch(event.keysym.sym) {
		case SDLK_ESCAPE: exit(0); break; // ESC key, kill the app
		case SDLK_0: option = 0; my_customtime = time; break;
		case SDLK_1: option = 1; my_customtime = time; break;
		case SDLK_2: option = 2; my_customtime = time; break;
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
	if (dragging && option == 3) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		float t = (float)x / (float)window_width;
		entity2->matrix_e = Matrix44();
		entity2->matrix_e.Translate(-1.0 + 2.0 * t, 0, 0);
	}
	
}

void Application::OnFileChanged(const char* filename)
{ 
	Shader::ReloadSingleShader(filename);
}
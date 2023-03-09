#include "application.h"
#include "mesh.h"
#include "shader.h"
#include "utils.h" 
#include "entity.h"

Mesh* quad;
Shader* general;
Shader* shader;
Shader* shader_transform;
float my_customtime;
Shader* shader_texture;
Texture* fruits;
Texture* street;

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
	shader_texture = Shader::Get("shaders/quad.vs", "shaders/quad_texture.fs");
	shader_transform = Shader::Get("shaders/quad.vs", "shaders/quad_transform.fs");
	my_customtime = 0;
	fruits = new Texture;
	fruits->Load("images/fruits.png");
	street = new Texture;
	street->Load("images/street.png");
}

// Render one frame
void Application::Render(void)
{
	if(exercice == 1){
		shader->Enable();
	
		shader->SetUniform1("option", option);
		shader->SetFloat("u_time2", time - my_customtime);
		shader->SetFloat("u_timeinf", time);
	
		quad->Render();
		shader->Disable();
	}
	if(exercice == 2){
		shader_texture->Enable();

		shader_texture->SetUniform1("option", option);
		shader_texture->SetTexture("fruits", fruits);
		shader_texture->SetFloat("u_time2", time - my_customtime);
		shader_texture->SetFloat("u_timeinf", time);

		quad->Render();
		shader_texture->Disable();
	}
	if (exercice == 3) {
		shader_transform->Enable();

		shader_transform->SetUniform1("option", option);
		shader_transform->SetTexture("street", street);
		shader_transform->SetFloat("u_time2", time - my_customtime);
		shader_transform->SetFloat("u_timeinf", time);

		quad->Render();
		shader_transform->Disable();
	}
	
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
		case SDLK_0: exercice = 0; option = 0; break;
		case SDLK_1: exercice = 1; option = (option + 1) % 6; my_customtime = time; break;
		case SDLK_2: exercice = 2; option = (option + 1) % 7; my_customtime = time; break;
		case SDLK_3: exercice = 3; option = (option + 1) % 3; my_customtime = time; break;
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
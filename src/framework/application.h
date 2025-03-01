/*  
	+ This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#pragma once
#pragma once

#include "main/includes.h"
#include "framework.h"
#include "image.h"
#include "mesh.h"
#include "material.h"
#include "entity.h"

class Application
{
public:

	FloatImage* zbuffer;
	Mesh* mesh1;
	Mesh* mesh2;
	Entity* entity1;
	Entity* entity2;
	Camera* cam;

	sUniformData* data;
	Material* material;
	// Window

	SDL_Window* window = nullptr;
	int window_width;
	int window_height;

	float time;
	float total_t;
	bool dragging;
	int mouse_x;
	int mouse_y;
	int mouse_z;
	bool mouse_drag;
	Vector2 prev_mouse;

	// Input
	int RenderMode;
	int Occlu;
	int exercice;
	int option;
	const Uint8* keystate;
	int mouse_state; // Tells which buttons are pressed
	Vector2 mouse_position; // Last mouse position
	Vector2 mouse_delta; // Mouse movement in the last frame

	void OnKeyPressed(SDL_KeyboardEvent event);
	void OnMouseButtonDown(SDL_MouseButtonEvent event);
	void OnMouseButtonUp(SDL_MouseButtonEvent event);
	void OnMouseMove(SDL_MouseButtonEvent event);
	void OnFileChanged(const char* filename);

	// CPU Global framebuffer
	Image framebuffer;

	// Constructor and main methods
	Application(const char* caption, int width, int height);
	~Application();

	void Init( void );
	void Render( void );
	void Update( float dt );

	// Other methods to control the app
	void SetWindowSize(int width, int height) {
		glViewport( 0,0, width, height );
		this->window_width = width;
		this->window_height = height;
	}

	Vector2 GetWindowSize()
	{
		int w,h;
		SDL_GetWindowSize(window,&w,&h);
		return Vector2(float(w), float(h));
	}
};

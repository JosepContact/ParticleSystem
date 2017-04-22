#include "Application.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "p2Log.h"

Scene::Scene() : Module()
{
	name = "scene";
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	pair<float, float> position(666, 0);
	pair<float, float> force (0, 0);
	pos = position;
	forc = force;
	example = (Ball*)App->particlesystem->CreateBall(position, force, true);
	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
		example = (Ball*)App->particlesystem->CreateBall(pos, forc, true);
	}

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}


#include "Application.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "p2Log.h"
#include <stdlib.h>
#include <time.h>

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
	pair<float, float> position(1100, 0);
	pair<float, float> force (0, 0);
	pos = position;
	forc = force;
	example = (MovableParticle*)App->particlesystem->CreateMovableParticle(position, force, true, BALL);
	position.first = 100;
	position.second = 300;
	StaticBucle* fire = (StaticBucle*)App->particlesystem->CreateStaticBucle(position, false, FIRE);
	position.first = 200;
	position.second = 150;
	StaticBucle* smoke = (StaticBucle*)App->particlesystem->CreateStaticBucle(position, true, SMOKE);

	int x, y;
	App->input->GetMousePosition(x, y);
	pair<float, float> mousepos(x, y);

	emitter = App->particlesystem->CreateEmitter(mousepos, false, 10, STAR);
	pair<float, float> pos(600, 200);

	StaticFinite* bomb = (StaticFinite*)App->particlesystem->CreateStaticFinite(pos, EXPLOSION);

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
	/*
	int x, y;
	App->input->GetMousePosition(x, y);
	pair<float, float> mousepos(x, y);
	emitter->SetPos(mousepos);

	App->input->GetMouseMotion(x, y);
	pair<float, float> mousemot(x, y);
	emitter->SetSpd(mousemot);
*/
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
		example = (MovableParticle*)App->particlesystem->CreateMovableParticle(pos, forc, true, BALL);
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


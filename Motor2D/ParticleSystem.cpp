#include "ParticleSystem.h"
#include "Window.h"
#include "Application.h"
#include "Render.h"
#include "Textures.h"
#include "p2Defs.h"
#include "p2Log.h"
#include <stdlib.h>  


ParticleSystem::ParticleSystem() : Module()
{
	name = "particlesystem";
}

ParticleSystem::~ParticleSystem()
{
}

bool ParticleSystem::Awake(pugi::xml_node &config )
{
	for (pugi::xml_node node = config.child("particle"); node; node = node.next_sibling("particle"))
	{
		string name(node.child("Info").child("Name").attribute("value").as_string());
		int id = node.child("Info").child("ID").attribute("value").as_int();
		string path(node.child("Textures").child("Texture").attribute("value").as_string());
		int lifespan = node.child("Stats").child("Lifespan").attribute("value").as_int();
		int w = node.child("Info").child("Collider").attribute("w").as_int();
		int h = node.child("Info").child("Collider").attribute("h").as_int();
		int rows = node.child("Animations").child("Animation").attribute("rows").as_int();
		int columns = node.child("Animations").child("Animation").attribute("columns").as_int();
		// -----------------------------
		Info curr;
		curr.Set(name, id, path, lifespan, w, h, rows, columns);
		info.push_back(curr);
	}
	return true;
}

bool ParticleSystem::Start()
{
	App->win->GetWindowSize(window_size.first, window_size.second);
	return true;
}

bool ParticleSystem::Update(float dt)
{
	for (list<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it)
	{
		it._Ptr->_Myval->Update();
	}
	return true;
}

bool ParticleSystem::PostUpdate()
{
	for (list<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it)
	{
		if (it._Ptr->_Myval->alive == false)
			DestroyParticle(it._Ptr->_Myval);
	}
	return true;
}

bool ParticleSystem::CleanUp()
{
	for (list<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it)
	{
		if (it._Ptr->_Myval != nullptr)
			DestroyParticle(it._Ptr->_Myval);
	}
	particles.clear();
	return true;
}

Particle * ParticleSystem::CreateBall(pair<float, float> startingposition, pair<float, float> startingforce, bool gravity)
{
	Particle* ret;
	ret = new Ball(true, info[BALL].path.c_str(), startingforce, startingposition);
	ret->type = (ParticleType)info[BALL].id;
	ret->name = info[BALL].name;
	ret->lifetime = info[BALL].lifespan;
	particles.push_back(ret);
	return ret;
}

Particle * ParticleSystem::CreateStaticBucle(pair<float, float> startingposition, bool finite, ParticleType type)
{
	Particle* ret;
	ret = new StaticBucle(info[type].path.c_str(), startingposition, info[type].w, info[type].h, info[type].rows, info[type].columns, finite);
	ret->type = (ParticleType)info[type].id;
	ret->name = info[type].name;
	ret->lifetime = info[type].lifespan;
	particles.push_back(ret);
	return ret;
}

bool ParticleSystem::DestroyParticle(Particle * curr)
{
	bool ret = true;
	if (curr != nullptr) {
		particles.remove(curr);
		curr->CleanUp();
		delete curr;	
	}
	else ret = false;
	return ret;
}

// --------------------------
//          BALL
// --------------------------

Ball::Ball(bool gravity, const char* path, pair<float, float> startingforce, pair<float, float> startingposition)
{
	force = startingforce;
	pos = startingposition;
	if (gravity) force.second += GRAVITY;
	texture = App->tex->Load(path);
	timer.Start();
}

void Ball::Update() {
	float secs = timer.ReadSec();
	pos.first = pos.first + spd.first * timer.ReadSec() + ((force.first / 2) * (timer.ReadSec() * timer.ReadSec()));
	pos.second = pos.second + spd.second * timer.ReadSec() + ((force.second / 2) * (timer.ReadSec() * timer.ReadSec()));
	Draw();
	alive = IsAlive();
}

void Ball::Draw() {
	App->render->Blit(texture, pos.first, pos.second);
}
bool Ball::IsAlive() {
	bool ret = true;
	if (timer.ReadSec() >= lifetime) {
		ret = false;
	}
	else if (pos.first >= App->particlesystem->window_size.first || pos.second >= App->particlesystem->window_size.second)
	{
		ret = false;
	}
	//
	//      COLLIDER EFFECT
	//
	return ret;
}

void Ball::CleanUp() {
	App->tex->UnLoad(texture);
}

// -----------------------
//     STATIC BUCLES
// -----------------------
// NOTE: Static bucles include all particles that stay in place 
// for an indefinite amount of time repeating (often) the same animation.

StaticBucle::StaticBucle(const char * path, pair<float, float> startingposition, int w, int h, int rows, int columns, bool argfinite)
{
	pos = startingposition;
	texture = App->tex->Load(path);
	SDL_Rect our_rect{0,0,w,h};

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			our_rect.x = j* w ;
			our_rect.y = i* h;
			anim.PushBack(our_rect);
		}
		our_rect.x = 0;
	}
	anim.loop = true;
	anim.speed = 0.2f;
	finite = argfinite;
	timer.Start();
}

void StaticBucle::Update()
{
	Draw();
	int b = anim.frames[9].x;
	int c = anim.frames[4].x;
	int c2 = anim.frames[11].x;
	alive = IsAlive();
}

void StaticBucle::Draw()
{
	App->render->Blit(texture, pos.first, pos.second, &anim.GetCurrentFrame());
}

bool StaticBucle::IsAlive()
{
	bool ret = true;
	if (timer.ReadSec() >= lifetime && finite == true) {
		ret = false;
	}
	else if (pos.first >= App->particlesystem->window_size.first || pos.second >= App->particlesystem->window_size.second)
	{
		ret = false;
	}
	//
	//      COLLIDER EFFECT
	//
	return ret;
}

void StaticBucle::CleanUp()
{
	App->tex->UnLoad(texture);
}
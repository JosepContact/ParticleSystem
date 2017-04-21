#include "ParticleSystem.h"
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
		Info curr;
		curr.Set(name, id, path, lifespan);
		info.push_back(curr);
	}
	return true;
}

bool ParticleSystem::Start()
{
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

bool ParticleSystem::CleanUp()
{
	for (list<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it)
	{
		it._Ptr->_Myval->CleanUp();
		if (it._Ptr->_Myval != nullptr)
			delete it._Ptr->_Myval;
	}
	return true;
}

Particle * ParticleSystem::CreateBall(pair<int, int> startingposition, pair<int, int> startingforce, bool gravity)
{
	Particle* ret;
	ret = new Ball(true, info[BALL].path.c_str());
	ret->force = startingforce;
	ret->pos = startingposition;
	ret->type = (ParticleType)info[BALL].id;
	ret->name = info[BALL].name;
	ret->lifetime = info[BALL].lifespan;
	particles.push_back(ret);
	return ret;
}


Ball::Ball(bool gravity, const char* path)
{
	if (gravity) force.second += GRAVITY;
	texture = App->tex->Load(path);
}

void Ball::Update() {
	Draw();
}

void Ball::Draw() {
	App->render->Blit(texture, pos.first, pos.second);
}
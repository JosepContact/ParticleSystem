#include "ParticleSystem.h"
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

bool ParticleSystem::Awake(pugi::xml_node &)
{
	return true;
}

bool ParticleSystem::Start()
{
	return true;
}

bool ParticleSystem::Update(float dt)
{
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

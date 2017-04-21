#include "ParticleSystem.h"

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
	return true;
}

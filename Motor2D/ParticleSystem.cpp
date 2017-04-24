#include "ParticleSystem.h"
#include "Window.h"
#include "Application.h"
#include "Render.h"
#include "Textures.h"
#include "p2Defs.h"
#include "p2Log.h"
#include <stdlib.h>  
#include <time.h>

ParticleSystem::ParticleSystem() : Module()
{
	name = "particlesystem";
}

ParticleSystem::~ParticleSystem()
{
}

// We are loading all the information from config here.
// Ideally we would have the position also taken from the xml.

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
		Info curr(name, id, path, lifespan, w, h, rows, columns);
		info.push_back(curr);
	}
	return true;
}

bool ParticleSystem::Start()
{
	srand(time(NULL));
	App->win->GetWindowSize(window_size.first, window_size.second);
	return true;
}

bool ParticleSystem::Update(float dt)
{
	update.Start();
	for (list<Particle*>::iterator it = particles.begin(); it != particles.end(); ++it)
	{
		it._Ptr->_Myval->Update();
	}
	LOG("Particles: %.10f", update.ReadSec());
	update.Start();
	for (list<Emitter*>::iterator it = emitters.begin(); it != emitters.end(); ++it)
	{
		it._Ptr->_Myval->Update(dt);
	}
	LOG("Emitter: %.10f", update.ReadSec());
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

Particle * ParticleSystem::CreateMovableParticle(pair<float, float> startingposition, pair<float, float> startingforce, bool gravity, ParticleType type)
{
	Particle* ret = nullptr;
	if (particles.size() < MAX_PARTICLES)
	{
		ret = new MovableParticle(gravity, info[type].path.c_str(), startingforce, startingposition, info[type].w, info[type].h, info[type].rows, info[type].columns);
		ret->type = (ParticleType)info[type].id;
		ret->name = info[type].name;
		ret->lifetime = info[type].lifespan;
		particles.push_back(ret);
	}
	else {
		LOG("Maximum particles achieved.");
	}
	return ret;
}

Particle * ParticleSystem::CreateStaticBucle(pair<float, float> startingposition, bool finite, ParticleType type)
{
	Particle* ret = nullptr;
	if (particles.size() < MAX_PARTICLES)
	{
		ret = new StaticBucle(info[type].path.c_str(), startingposition, info[type].w, info[type].h, info[type].rows, info[type].columns, finite);
		ret->type = (ParticleType)info[type].id;
		ret->name = info[type].name;
		ret->lifetime = info[type].lifespan;
		particles.push_back(ret);
	}
	else {
		LOG("Maximum particles achieved.");
	}
	return ret;
}

Particle * ParticleSystem::CreateStaticFinite(pair<float, float> startingposition, ParticleType type)
{
	Particle* ret = nullptr;
	if (particles.size() < MAX_PARTICLES)
	{
		ret = new StaticFinite(info[type].path.c_str(), startingposition, info[type].w, info[type].h, info[type].rows, info[type].columns);
		ret->type = (ParticleType)info[type].id;
		ret->name = info[type].name;
		ret->lifetime = info[type].lifespan;
		particles.push_back(ret);
	}
	else {
		LOG("Maximum particles achieved.");
	}
	return ret;
}

Emitter * ParticleSystem::CreateEmitter(pair<float, float> startingposition, bool finite, float duration, ParticleType type)
{
	Emitter* ret = nullptr;
	ret = new Emitter(startingposition, finite, duration);
	ret->type = type;
	emitters.push_back(ret);
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
//     MOVABLE PARTICLES
// --------------------------
// Particles that are not meant to be static, which means
// that they follow 'physics' laws and have speed and acceleration.
// 1. Notice the code doesn't accept movable particles with animations,
// you should be able to do it easily following the other particles guidelines.
// 2. Notice the code introduces a lifetime to all particles automatically,
// this can be removed, although I don't recommend it.

MovableParticle::MovableParticle(bool gravity, const char* path, pair<float, float> startingforce, pair<float, float> startingposition, int w, int h, int rows, int columns)
{
	spd = startingforce;
	pos = startingposition;
	this->gravity = gravity;
	SDL_Rect our_rect{ 0,0,w,h };

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			our_rect.x = j* w;
			our_rect.y = i* h;
			anim.PushBack(our_rect);
		}
		our_rect.x = 0;
	}
	anim.loop = true;
	anim.speed = 0.2f;
	// Deactivate gravity on moving particles by sending 'false' on creation
	texture = App->tex->Load(path);
	timer.Start();
}

void MovableParticle::Update() {
	// These are the simple physics formulas I used for the particles.
	float secs = timer.ReadSec();
	pos.first = pos.first + spd.first * timer.ReadSec();
	if (gravity) {
		pos.second = pos.second + spd.second * timer.ReadSec() + ((GRAVITY / 2) * (timer.ReadSec() * timer.ReadSec()));
	}
	else {
		pos.second = pos.second + spd.second * timer.ReadSec();
	}
	// You can be creative and use other movement functions so they follow new movement patterns!
	Draw();
	alive = IsAlive();
}

void MovableParticle::Draw() {
	App->render->Blit(texture, pos.first, pos.second, &anim.GetCurrentFrame());
}

bool MovableParticle::IsAlive() {
	bool ret = true;
	if (timer.ReadSec() >= lifetime) {
		ret = false;
	}
	else if (pos.first >= App->particlesystem->window_size.first || pos.second >= App->particlesystem->window_size.second)
	{
		ret = false;
	}
	return ret;
}

void MovableParticle::CleanUp() {
	App->tex->UnLoad(texture);
}

// -----------------------
//     STATIC BUCLES
// -----------------------
// Particles that stay in place for an indefinite amount
// of time repeating (often) the same animation.
// 1. The code makes all the animation print on the same place
// following the same pattern. This is not ideal, you might want your code to
// randomly print a different frame.
// 2. You can give it a lifetime on the xml and send 'true' on creation. Or
// You can send 'false' so it bucles forever.
//
// NOTICE that non finite particles that leave the screen are deleted, is that what your game needs?

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
	return ret;
}

void StaticBucle::CleanUp()
{
	App->tex->UnLoad(texture);
}


// -----------------------
//     FINITE BUCLES
// -----------------------
// Particles that stay in place for a fixed amount of time
// due to its animation.
// 1. This code deletes the particle after its animation is over.
//
// NOTICE that particles that leave the screen are deleted, is that what your game needs?

StaticFinite::StaticFinite(const char * path, pair<float, float> startingposition, int w, int h, int rows, int columns)
{
	pos = startingposition;
	texture = App->tex->Load(path);
	SDL_Rect our_rect{ 0,0,w,h };

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			our_rect.x = j* w;
			our_rect.y = i* h;
			anim.PushBack(our_rect);
		}
		our_rect.x = 0;
	}
	anim.loop = false;
	anim.speed = 0.2f;
}

void StaticFinite::Update()
{
	Draw();
	alive = IsAlive();
}

void StaticFinite::Draw()
{
	App->render->Blit(texture, pos.first, pos.second, &anim.GetCurrentFrame());
}

bool StaticFinite::IsAlive()
{
	bool ret = true;
	if (pos.first >= App->particlesystem->window_size.first || pos.second >= App->particlesystem->window_size.second)
	{
		ret = false;
	}
	else if (anim.Finished()) {
		ret = false;
	}
	return ret;
}

void StaticFinite::CleanUp()
{
	App->tex->UnLoad(texture);
}

// -----------------------
//     EMITTER
// -----------------------
// Emitters are tools that automatically create movable particles to (more or less)
// randomised directions.
//
// 1. This Emetter ALWAYS creates finite particles. 'finite' on creation stands for
// the lifetime of the emetter itself.
// 2. #define EMITTER_SPEED at the beginning of the ParticleSystem.h define
// the frequency of particle creation. It works with the dt send to the module in the Update.
// 3. Emitters should always stop working outside the camera.
// 5. SetPos and SetSpd functions can help you to create travelling emetters
// for example: smoke from a rocket, thrown spells, etc

Emitter::Emitter(pair<float, float> startingposition, bool finite, float duration) : pos(startingposition), finite(finite), lifetime(duration)
{
}

void Emitter::Update(float dt)
{
	speed += dt;
	if (speed > EMITTER_SPEED)
	{

		// Ideally you want to set up your own forces depending on the emitter.
		force.first = (float)(rand() % 8 + 1);
		bool negative = rand() % 2;
		if (negative) force.first *= -1;
		force.first += speed_orig.first;
		// speed_orig is 0 unless stated otherwise with the SetSpd() method.
		force.second = (float)(rand() % 8 + 1);
		negative = rand() % 2;
		if (negative) force.second *= -1;
		force.second += speed_orig.second;
		//
		App->particlesystem->CreateMovableParticle(pos, force, true, type);
		speed = 0;
	}
	alive = IsAlive();
}

bool Emitter::IsAlive()
{
	bool ret = true;
	if (timer.ReadSec() >= lifetime && finite == true) {
		ret = false;
	}
	return ret;
}

void Emitter::SetPos(pair<float, float> pos)
{
	this->pos = pos;
}

void Emitter::SetSpd(pair<float, float> extra_speed)
{
	speed_orig = extra_speed;
}

#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__

#include "SDL\include\SDL_pixels.h"
#include "SDL/include/SDL.h"
#include "Module.h"
#include "Timer.h"
#include "Animation.h"
#include "Collision.h"
#include <list>
#include <string>

using namespace std;

#define GRAVITY 10
#define MAX_PARTICLES 50
#define EMITTER_SPEED 0.05

struct SDL_Texture;
class Collider;

enum ParticleType {
	BALL,
	STAR,
	FIRE,
	SMOKE,
	EXPLOSION,
	UNKNOWN
};

// Info holds the data gathered in Awake.
struct Info {
	string name;
	int id;
	int lifespan;
	Animation anim;
	string path;
	SDL_Texture* texture;

	Info::Info(string argname, string argpath, int argid, int arglifespan, Animation arganim)
		: name(argname), path(argpath), id(argid), lifespan(arglifespan), anim(arganim){}
};


class Particle {
public:
	pair<float, float> pos;

	int lifetime; // (s)
	bool alive = true;

	ParticleType type;
	SDL_Texture* texture;
	string name;
	Collider* collider;
	Animation anim;
	Timer timer;

	virtual void Update() {};
	virtual void Draw() {};
	virtual bool IsAlive() { return true; };
};

class MovableParticle : public Particle {
public:
	MovableParticle(bool gravity, pair<float, float> startingforce, pair<float, float> startingposition);
	

	pair<float, float> spd;
	pair<float, float> force;
	bool gravity = false;


	void Update();
	void Draw();
	bool IsAlive();
};

class StaticBucle : public Particle {
public:
	StaticBucle( pair<float, float> startingposition, bool);

	bool finite = false;

	void Update();
	void Draw();
	bool IsAlive();
};


class Emitter {
public:
	Emitter(pair<float, float> startingposition, bool finite, float duration);

	pair<float, float> pos;
	// It is not actual force as in m/s^2.
	pair<float, float> force;

	Timer timer;

	// Just a helper:
	pair<float, float> speed_orig;
	// Emision frequency:
	float speed;

	float lifetime;
	bool finite = false;
	bool alive = true;
	ParticleType type;


	void Update(float dt);
	bool IsAlive();
	void SetPos(pair<float, float> pos);
	void SetSpd(pair<float, float> extra_speed);
};


class ParticleSystem : public Module {
public:

	ParticleSystem();

	virtual ~ParticleSystem();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	Particle* CreateMovableParticle(pair<float,float> startingposition, pair<float,float> startingforce, bool gravity, ParticleType type);
	Particle* CreateStaticBucle(pair<float, float> startingposition, bool finite, ParticleType type);
	Emitter* CreateEmitter(pair<float, float> startingposition, bool finite, float duration, ParticleType type);
private:
	bool DestroyParticle(Particle* curr);
	bool DestroyEmitter(Emitter * curr);


public:
	pair<uint, uint> window_size;
	Timer update;
private:
	list<Particle*> particles;
	list<Emitter*> emitters;
	vector<Info> info;
};

#endif

#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__

#include "SDL\include\SDL_pixels.h"
#include "SDL/include/SDL.h"
#include "Module.h"
#include "Timer.h"
#include "Animation.h"
#include <list>
#include <string>

using namespace std;

#define GRAVITY 10
#define MAX_PARTICLES 100
#define MAX_PARTICLES_EMITTER 40
#define EMITTER_SPEED 0.02

struct SDL_Texture;

enum ParticleType {
	BALL,
	STAR,
	FIRE,
	SMOKE,
	EXPLOSION,
	UNKNOWN
};

struct Info {
	string name;
	int id;
	string path;
	int lifespan;
	int w, h, rows, columns;

	Info::Info(string argname, int argid, string argpath, int arglifespan, int argw, int argh, int argrows, int argcolumns)
		: name(argname), id(argid), path(argpath), lifespan(arglifespan), w(argw), h (argh), rows(argrows), columns(argcolumns){}
};

class Particle {
public:
	pair<float, float> pos;

	int lifetime; // (s)
	bool alive = true;
	ParticleType type;
	string name;
	SDL_Texture* texture;

	virtual void Update() {};
	virtual void Draw() {};
	virtual void CleanUp() {};
	virtual bool IsAlive() { return true; };

};

class MovableParticle : public Particle {
public:
	MovableParticle(bool gravity, const char * path, pair<float, float> startingposition, pair<float, float> startingforce);

	Timer timer;


	pair<float, float> spd;
	pair<float, float> force;
	int angle; // (degrees)

	void Update();
	void Draw();
	bool IsAlive();
	void CleanUp();
};

class StaticBucle : public Particle {
public:
	StaticBucle(const char * path, pair<float, float> startingposition, int, int ,int, int, bool);
	
	Animation anim;
	
	Timer timer;
	bool finite = false;
	void Update();
	void Draw();
	bool IsAlive();
	void CleanUp();
};

class StaticFinite : public Particle {
public:
	StaticFinite(const char * path, pair<float, float> startingposition, int, int, int, int);
	Animation anim;

	void Update();
	void Draw();
	bool IsAlive();
	void CleanUp();
};


class Emitter {
public:
	Emitter(pair<float, float> startingposition, bool finite, float duration);
	pair<float, float> pos;
	pair<float, float> force;
	pair<float, float> speed_orig;
	float speed;
	float lifetime;
	Timer timer;
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
	Particle* CreateStaticFinite(pair<float, float> startingposition, ParticleType type);
	Emitter* CreateEmitter(pair<float, float> startingposition, bool finite, float duration, ParticleType type);
	bool DestroyParticle(Particle* curr);

private:
	list<Particle*> particles;
	list<Emitter*> emitters;

	vector<Info> info;
public:
	pair<uint, uint> window_size;
};

#endif

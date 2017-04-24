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
	SDL_Texture* texture;
	string name;
	Collider* collider;
	Animation anim;
	Timer timer;

	virtual void Update() {};
	virtual void Draw() {};
	virtual void CleanUp() {};
	virtual bool IsAlive() { return true; };
};

class MovableParticle : public Particle {
public:
	MovableParticle(bool gravity, const char* path, pair<float, float> startingforce, pair<float, float> startingposition, int w, int h, int rows, int columns);

	

	pair<float, float> spd;
	pair<float, float> force;
	bool gravity = false;


	void Update();
	void Draw();
	bool IsAlive() ;
	void CleanUp();
};

class StaticBucle : public Particle {
public:
	StaticBucle(const char * path, pair<float, float> startingposition, int, int ,int, int, bool);

	bool finite = false;

	void Update();
	void Draw();
	bool IsAlive();
	void CleanUp();
};

class StaticFinite : public Particle {
public:
	StaticFinite(const char * path, pair<float, float> startingposition, int, int, int, int);

	void Update();
	void Draw();
	bool IsAlive();
	void CleanUp();
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
	Particle* CreateStaticFinite(pair<float, float> startingposition, ParticleType type);
	Emitter* CreateEmitter(pair<float, float> startingposition, bool finite, float duration, ParticleType type);
	bool DestroyParticle(Particle* curr);

private:
	list<Particle*> particles;
	list<Emitter*> emitters;

	vector<Info> info;
public:
	pair<uint, uint> window_size;
	Timer update;
};

#endif

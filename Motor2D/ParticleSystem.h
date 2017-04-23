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

struct SDL_Texture;

enum ParticleType {
	BALL,
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

	void Set(string argname, int argid, string argpath, int arglifespan, int argw, int argh, int argrows, int argcolumns){
		name = argname;
		id = argid;
		path = argpath;
		lifespan = arglifespan;
		w = argw;
		h = argh;
		rows = argrows;
		columns = argcolumns;
	}
};

class Particle {
public:
	pair<float, float> pos;

	int lifetime; // (s)
	bool alive = true;
	ParticleType type;
	string name;
	

	virtual void Update() {};
	virtual void Draw() {};
	virtual void CleanUp() {};
	virtual bool IsAlive() { return true; };

};

class Ball : public Particle {
public:
	Ball(bool gravity, const char * path, pair<float, float> startingposition, pair<float, float> startingforce);
	SDL_Texture* texture;

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
	SDL_Texture* texture;
	Animation anim;

	Timer timer;
	bool finite = false;
	void Update();
	void Draw();
	bool IsAlive();
	void CleanUp();
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

	Particle* CreateBall(pair<float,float> startingposition, pair<float,float> startingforce, bool gravity);
	Particle* CreateStaticBucle(pair<float, float> startingposition, bool finite, ParticleType type);
	Particle* CreateExplosion(pair<float, float> startingposition);
	bool DestroyParticle(Particle* curr);

private:
	list<Particle*> particles;
	vector<Info> info;
public:
	pair<uint, uint> window_size;
};

#endif

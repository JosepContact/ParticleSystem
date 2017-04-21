#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__
#include "SDL\include\SDL_pixels.h"

#include "Module.h"
#include <list>
#include <string>
using namespace std;

#define GRAVITY -10;

struct SDL_Texture;

enum ParticleType {
	BALL,
	EXPLOSION,
	UNKNOWN
};

struct Info {
	string name;
	int id;
	string path;
	int lifespan;

	void Set(string argname, int argid, string argpath, int arglifespan){
		name = argname;
		id = argid;
		path = argpath;
		lifespan = arglifespan;
	}
};

class Particle {
public:
	pair<int, int> pos;
	pair<int, int> spd;
	pair<int, int> force;
	int angle; // (degrees)
	int lifetime; // (s)
	ParticleType type;
	string name;

	virtual void Update() {};
	virtual void Draw() {};
	virtual void CleanUp() {};
};

class Ball : public Particle {
public:
	Ball(bool gravity, const char * path);
	SDL_Texture* texture;

	void Update();
	void Draw();
};

class ParticleSystem : public Module {
public:

	ParticleSystem();

	virtual ~ParticleSystem();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	Particle* CreateBall(pair<int,int> startingposition, pair<int,int> startingforce, bool gravity);
	Particle* CreateExplosion(pair<int, int> startingposition);
	void DestroyParticle(Particle* curr);

private:
	list<Particle*> particles;
	vector<Info> info;
};

#endif

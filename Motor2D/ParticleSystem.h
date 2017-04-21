#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__
#include "SDL\include\SDL_pixels.h"

#include "Module.h"
#include <list>
using namespace std;


struct SDL_Texture;

enum ParticleType {
	BALL,
	EXPLOSION,
	UNKNOWN
};

class Particle {
public:
	pair<int, int> pos;
	pair<int, int> spd;
	pair<int, int> force;
	int angle; // (degrees)
	int lifetime; // (s)
	ParticleType type;

	virtual void Update() {};
	virtual void Draw() {};
	virtual void CleanUp() {};
};

class Ball : public Particle {
public:
	SDL_Texture* texture;
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
};

#endif

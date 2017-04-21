#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__

#include "Module.h"
#include <vector>
using namespace std;

class Particle {
private:
	pair<int, int> pos;
	pair<int, int> spd;
	pair<int, int> acc;
	int angle; // (degrees)
	int lifetime; // (s)
};

class ParticleSystem : public Module {
public:

	ParticleSystem();

	virtual ~ParticleSystem();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	Particle* CreateParticle(pair<int,int> startingforce, bool gravity);

private:
	vector<Particle*> particles;
};

#endif

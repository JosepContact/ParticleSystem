## What is a 2D particle system? 
A **particle system** is a method used in computer graphics that uses small sprites (in 2D) to simulate small portions of a fluid or amorphous entities. The illusion of all the particles together creates a vision of a real entity (some examples: fire, clouds, explosions, electricity, 'magic', etc.). We use particle systems to create and organise large amounts of these particles automatically.

![Image](https://cdn.tutsplus.com/gamedev/uploads/2013/08/XNA_Geometry_Wars_Particle_Effects_400px.jpg)

## Particles in 2D videogames.

## The 'particle' concept
Particles are, as individuals, all the same, but together they create complete distinct visuals. By changing their sprites we can create infinite amount of effects, and by changing their patterns we can create different approaches that may be more convinient for our concept. Some examples:
![Image](http://www.gamingtruth.com/wp-content/uploads/2010/10/ss_429486678c380ae3e4ea6040e30a920b0095a4ce.600x338.jpg)

We can see here the explosion is consistent. The outter particles are less noticable, but the smoke spawns randomly, in a short amount of time and in the same area. Here's a similar example:

![Image](http://media.indiedb.com/images/games/1/24/23133/Explosion.jpg) 

Particles are not always spread around from one point, they can also be spread in within one angle. What will happen with the smoke if we spin the ship?

![Image]https://i.ytimg.com/vi/8lyiEytbvd0/hqdefault.jpg

Here, the bullets are particles shot from the spaceship on a linear trajectory. As you may have guessed, particles can interact with the player or the environment too.

![Image]http://i.imgur.com/ujnnrwg.jpg

Particles will be as complex as you want them to be. I will be explaining the basic concept of a Particle System, its general structure and a couple of general particles. But keep in mind that you might need dozens of these elements, you might have to create some new ones yourself.

## How do we achieve it?
Let's do a general approach before jumping to code.
1. First of all, we need to think about the particles, and how they behave. Particles need to be created and deleted automatically so the system doesn't collapse. Also, they need a starting speed for them, ideally it will be a randomised speed (between fixed parameters) to achieve that special *flashy* effect. It is interesting that particles can also change over time, for example: change colors, sizes, shapes... But after all, they are just **sprites** that we can treat as animations.
2. We have to consider a solid system capable of creating particles on call and store them for proper use. The system must update every particle, and every update should change its properties, render it to screen and check if it is time to be destroyed.
3. Finally, we need a support class that can create particles and throw them randomly. This element is called *Emitter*.
4. The particle system should be able to create multiple emitters, and they are also updated from the Particle System.
It is important that you keep in mind that particles can be completely different from each other, in the future you should be able to decide how your particle is going to act. As an introduction, I will bring you static and non static particles.

### The Class Particle
The class particle holds all general information and methods that other particles will inherit
- As I have mentioned before the particles are deleted automatically, for that we need a variable that stores its lifetime and a timer[1].
- They also need a position.
- A texture.
- An Animation[2].
- OPTIONAL: The name of the particle and/or an id.
- OPTIONAL: Collider.

As methods you should add at least a virtual Update.
```
`class Particle {
public:
	pair<float, float> pos;

	int lifetime; // (s)
	Timer timer;
	bool alive = true;

	ParticleType type; // this is my id
	SDL_Texture* texture;
	string name;
	Collider* collider;
	Animation anim;

	virtual void Update() {};
	virtual void Draw() const{};
	virtual bool IsAlive() { return true; };
};`
```
On the particle's Update():
	- Update its postion.
	- Render it to screen.
	- Check if it is still alive.
	
### Static Particles:
Particles that stay in place for an indefinite amount of time repeating (often) the same animation.
These particles can be useful when a travelling emitter continuosly drops particles a player can pick up.
```
`class StaticParticle: public Particle {
public:
	bool finite;

	void Update(); void Draw(); bool IsAlive();
};`
```
I suggest it holds a boolean just in case you do not want a finite lifetime for it.

### Non Static or Movable Particles:
Particles that follow 'physics' laws and respond to speed and forces.
```
class MovableParticle: public Particle {
public:
	pair<float, float> spd;
	pair<float, float> force;
	bool gravity = false; // OPTIONAL


	void Update(); void Draw(); bool IsAlive();
};
```

### Setting up a Particle Manager.
The ParticleManager is in charge of all your particles. It creates them, updates them and destroys them. Like a god to its creations. Something like this:

```class ParticleManager {
public:
	ParticleManager();
	virtual ~ParticleManager();
	
	bool Update(float dt); 

	Particle* CreateMovableParticle(pair<float,float> startingposition, pair<float,float> startingspeed, bool gravity, ParticleType type);
	Particle* CreateStaticParticle(pair<float, float> startingposition, bool finite, ParticleType type);
	bool DestroyParticle(Particle* particle);

private:
	list<Particle*> particles;
};```

It holds a list or vector with all the particles you have. This is what you will be using to call all the updates of the particles inside your Particle Manager update, and also to destroy them all when you leave the program in CleanUp.
### Some tips before starting...
- I used an XML document to load different information for each type of particle. Make sure you check it.
- Think of different kinds of particles you want. My movable particles only have linear movement, but maybe you want them describe a circle on your screen.
### NOTES:
[1] **The Timer** is already in the code.

[2] **The Animation** is also in there.

### Let's practise.




Markdown is a lightweight and easy-to-use syntax for styling your writing. It includes conventions for




[Link](url) and ![Image](src)

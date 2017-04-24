## What is a 2D particle system? 
A **particle system** is a method used in computer graphics that uses small sprites (in 2D) to simulate small portions of a fluid or amorphous entities. The illusion of all the particles together creates a vision of a real entity (some examples: fire, clouds, exmplosions, electricity, 'magic', etc.). We use particle systems to create and organise large amounts of these particles.

## How do we achieve it?
1. First of all, we need to think about the particles, and how they behave. Particles need to be created and deleted automatically so the system doesn't collapse. Also, they need a starting speed for them, ideally it will be a randomised speed (between fixed parameters) to achieve that special *flashy* effect. It is interesting that particles can change over time as well, for example: change colors, sizes, shapes... But after all, they are just **sprites** that we can treat as animations.

## What is a 2D particle system? 
A **particle system** is a method used in computer graphics that uses small sprites (in 2D) to simulate small portions of a fluid or amorphous entities. The illusion of all the particles together creates a vision of a real entity (some examples: fire, clouds, exmplosions, electricity, 'magic', etc.). We use particle systems to create and organise large amounts of these particles.

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
- They also need a position. I used a *pair* of type *float*.
- A texture.
- An Animation[2].
- OPTIONAL: The name of the particle and/or an id
As methods you should add at least a virtual Update.
```
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

	virtual void Update() {};
	virtual void Draw() {};
	virtual void CleanUp() {};
	virtual bool IsAlive() { return true; };
};
```
### Types of Particles:
- Static Particles.
- Movable Particles.

### Setting up a Particle Manager.


### Some tips before starting...

### Let's practise.


Markdown is a lightweight and easy-to-use syntax for styling your writing. It includes conventions for

```markdown
Syntax highlighted code block

# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```

For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/JosepUPC/ParticleSystem/settings). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://help.github.com/categories/github-pages-basics/) or [contact support](https://github.com/contact) and we’ll help you sort it out.

#ifndef __COLLISION_H__
#define __COLLISION_H__

#define MAX_COLLIDERS 50
#define COLLIDER_MAX 200

#define FARLANDS iPoint({5000, 5000})

#include "Module.h"
#include "p2Defs.h"
#include "Animation.h"

class SDL_Rect;
enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type;
	Module* callback = nullptr;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, Module* callback = nullptr) :
		rect(rectangle),
		type(type),
		callback(callback)
	{}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	//Directions ColliderHit(const SDL_Rect& r) const;
	bool CheckCollision(const SDL_Rect& r) const;
};

class Collision : public Module
{
public:

	Collision();
	~Collision();

	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, Module* callback = nullptr);
	bool EraseCollider(Collider* collider);
	void DebugDraw();

private:

	Collider* colliders[MAX_COLLIDERS];
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool debug = false;
};

#endif // __ModuleCollision_H__


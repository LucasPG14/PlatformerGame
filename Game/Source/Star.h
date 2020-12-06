#ifndef __STAR_H__
#define __STAR_H__
#include "Animation.h"
#include "Enemy.h"

class Star: public Enemy
{
public:

	Star(iPoint position);
	~Star();
	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

	void Draw() override;

private:

	Collider* starItem = nullptr;
	Animation starAnim;
	Animation shineAnim;
	SDL_Texture* starTex;
	SDL_Texture* shineTex;
	uint star = -1;

};
#endif

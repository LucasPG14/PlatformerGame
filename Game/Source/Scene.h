#ifndef __SCENE_H__
#define __SCENE_H__

#include "Scenes.h"
#include "ColliderManagement.h"
#include "PugiXml\src\pugixml.hpp"

struct SDL_Texture;
class Bat;
class Slime;

class Scene : public Scenes
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Load();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all updates
	bool Draw();

	// Called before quitting
	bool Unload();

	void LoadingEntities();

	void PlayerPosition();

private:

	SDL_Texture* bg;
	SDL_Texture* bg2;
	SDL_Texture* bg3;
	SDL_Texture* lifesTex;
	SDL_Texture* starTex;

	// Font
	int yellowFont = -1;

	Player* player;
};
#endif // __SCENE_H__
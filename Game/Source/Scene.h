#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

#include "PugiXml\src\pugixml.hpp"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:
	SDL_Texture* bg;
	pugi::xml_node node;
	SString folder;
	SString stringBackground;
};

#endif // __SCENE_H__
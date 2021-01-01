#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Module.h"
#include "Scenes.h"

#include "PugiXml\src\pugixml.hpp"

class SceneManager : public Module
{
public:

	SceneManager();

	// Destructor
	virtual ~SceneManager();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	bool LoadState(pugi::xml_node& load);

	bool SaveState(pugi::xml_node& save) const;

public:

	Scenes* current;
	Scenes* next;

	int score;
	int finalScore;
	int stars;

	bool onTransition;
	bool fadeOutCompleted;
	float transitionAlpha;
	bool pause;
	bool saved;
	bool continueClicked;
};
#endif // __SCENEMANAGER_H__
#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Module.h"
#include "Scenes.h"
#include "List.h"

#include "PugiXml\src\pugixml.hpp"

class SceneIntro;
class Scene;
class SceneDie;
class SceneWin;

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

	// Called before all updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void AddScene(Scenes* scene, bool active);

public:

	List<Scenes*> scenes;
	SceneIntro* intro;
	Scene* level1;
	SceneDie* dieScene;
	SceneWin* winScene;

};
#endif // __SCENEMANAGER_H__
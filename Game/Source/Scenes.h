#ifndef __SCENES_H__
#define __SCENES_H__

#include "SString.h"

#include "PugiXml\src\pugixml.hpp"

class GuiControl;

class Scenes 
{
public:

	Scenes() : active(false) {}

	// Destructor
	virtual ~Scenes() {};

	// Called before the first frame
	virtual bool Load() { return true; }

	// Called each loop iteration
	virtual bool Update(float dt) { return true; }

	// Called before all updates
	virtual bool Draw() { return true; }

	// Called before quitting
	virtual bool Unload() { return true; }

	virtual void PlayerPosition() {}

	// Define multiple Gui Event methods
	virtual bool OnGuiMouseClickEvent(GuiControl* control)
	{
		return true;
	}


	void Enable()
	{
		if (!this->active)
		{
			this->active = true;
			this->Load();
		}
	}

	// Disable the module when the fade fucntion is called
	void Disable()
	{
		if (this->active)
		{
			this->active = false;
			this->Unload();
		}
	}

public:

	SString name;
	bool active;

};
#endif // __SCENES_H__
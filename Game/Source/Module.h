#ifndef __MODULE_H__
#define __MODULE_H__

#include "SString.h"

#include "PugiXml/src/pugixml.hpp"

class App;

class Module
{
public:

	Module() : active(false)
	{
	
	}

	void Init(bool activeModule)
	{
		active = activeModule;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

    // Loads the information in the save XML
	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	// Saves the information in the save XML
	virtual bool SaveState(pugi::xml_node&) const
	{
		return true;
	}

	// Enable the module when the fade function is called
	void Enable()
	{
		if (!this->active)
		{
			this->active = true;
			this->Start();
		}
	}

	// Disable the module when the fade fucntion is called
	void Disable()
	{
		if (this->active)
		{
			this->active = false;
			this->CleanUp();
		}
	}

public:

	SString name;
	bool active;
};
#endif // __MODULE_H__
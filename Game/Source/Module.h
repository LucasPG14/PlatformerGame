#ifndef __MODULE_H__
#define __MODULE_H__

#include "SString.h"

#include "PugiXml/src/pugixml.hpp"

class App;

class Module
{
public:

	Module() : active(true)
	{}

	void Init(bool activeModule)
	{
		active = activeModule;
	}

	// Called before render is available
	// L01: DONE 5: Sending config file to all modules
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

    // L02: DONE 2: Create new virtual methods to Load / Save state
	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&) const
	{
		return true;
	}

	//void IsActive(bool activeModule)
	//{
	//	active = activeModule;
	//}

	void Enable()
	{
		if (!this->active)
		{
			this->active = true;
			this->Start();
		}
	}

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
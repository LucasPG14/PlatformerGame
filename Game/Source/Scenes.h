#ifndef __SCENES_H__
#define __SCENES_H__

#include "SString.h"

#include "PugiXml\src\pugixml.hpp"

class Scenes 
{
public:

	Scenes() : active(false) {}

	// Destructor
	virtual ~Scenes() {};

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called before all updates
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	virtual void PlayerPosition()
	{
		return;
	}


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
#endif // __SCENES_H__
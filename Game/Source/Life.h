#ifndef __LIFE_H__
#define __LIFE_H__

#include "Animation.h"
#include "Item.h"

class Life : public Item
{
public:

	Life(iPoint position, EntityType entityType);
	~Life();

	bool Update(float dt) override;

private:

	Animation lifeAnim;
};
#endif

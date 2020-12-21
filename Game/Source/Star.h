#ifndef __STAR_H__
#define __STAR_H__

#include "Animation.h"
#include "Item.h"

class Star : public Item
{
public:

	Star(iPoint pos, EntityType entityType);
	~Star();

	bool Update(float dt) override;

private:

	Animation starAnim;
};
#endif

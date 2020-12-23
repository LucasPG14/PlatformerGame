#ifndef __STAR_H__
#define __STAR_H__

#include "Animation.h"
#include "Item.h"

class Star : public Item
{
public:

	Star(iPoint pos);
	~Star();

	bool Update(float dt) override;

	bool Load(pugi::xml_node& load);

	bool Save(pugi::xml_node& save) const;

private:

	Animation starAnim;
};
#endif

#ifndef __LIFE_H__
#define __LIFE_H__

#include "Animation.h"
#include "Item.h"

class Life : public Item
{
public:

	Life(iPoint position);
	~Life();

	bool Update(float dt) override;

	bool Load(pugi::xml_node& load);

	bool Save(pugi::xml_node& save) const;

private:

	Animation lifeAnim;
};
#endif

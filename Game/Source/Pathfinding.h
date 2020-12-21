#ifndef __PATHFINDING_H__
#define __PATHFINDING_H__

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "PQueue.h"
#include "DynArray.h"
#include "SDL/include/SDL_pixels.h"

#define COST_MAP_SIZE	100

struct SDL_Texture;
struct SDL_Color;
struct SDL_Rect;
struct TileSet;
class Player;

class Pathfinding : public Module
{
public:

	Pathfinding();

	// Destructor
	virtual ~Pathfinding();

	bool Start();

	// Called each loop iteration
	void Draw();

	bool CleanUp();

	// BFS Pathfinding methods
	void ResetPath(iPoint start);
	void DrawPath(DynArray<iPoint>& path);
	bool IsWalkable(int x, int y) const;

	// More pathfinding methods
	int MovementCost(int x, int y) const;
	DynArray<iPoint>* ComputePath(int x, int y);
	DynArray<iPoint>* ComputePathAStar(int x, int y);

	// Propagation methods
	void PropagateBFS(Player* player);
	bool PropagateDijkstra(Player* player);
	bool PropagateAStar(int x, int y);


	bool Load(pugi::xml_node& load);

	bool Save(pugi::xml_node& save);

	iPoint goalAStar;
	bool finishAStar = false;

public:
	// L10: BFS Pathfinding variables
	PQueue<iPoint> frontier;
	List<iPoint> visited;

	// L11: Additional variables
	List<iPoint> breadcrumbs;
	uint costSoFar[COST_MAP_SIZE][COST_MAP_SIZE];
	DynArray<iPoint> path;
	uint newCost = 0;
	SDL_Texture* cross;
};

#endif //__PATHFINDING_H__
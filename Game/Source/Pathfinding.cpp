#include "Pathfinding.h"
#include "App.h"
#include "Map.h"
#include "Render.h"
#include "Player.h"
#include "Input.h"
#include "Textures.h"
#include "Enemy.h"

Pathfinding::Pathfinding()
{
}

Pathfinding::~Pathfinding()
{
}

bool Pathfinding::Start()
{
	cross = app->tex->Load("Assets/Textures/cross.png");

	return true;
}

bool Pathfinding::CleanUp()
{
	path.Clear();
	frontier.Clear();
	visited.Clear();
	breadcrumbs.Clear();

	return true;
}

void Pathfinding::ResetPath(iPoint start)
{
	frontier.Clear();
	visited.Clear();
	breadcrumbs.Clear();

	frontier.Push(start, 0);
	visited.Add(start);
	breadcrumbs.Add(start);

	memset(costSoFar, 0, sizeof(uint) * COST_MAP_SIZE * COST_MAP_SIZE);
}

void Pathfinding::DrawPath(DynArray<iPoint>& path)
{
	// Draw path
	for (uint i = 0; i < path.Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path[i].x, path[i].y);
		app->render->DrawTexture(cross, pos.x, pos.y);
	}
}

bool Pathfinding::IsWalkable(int x, int y) const
{
	ListItem<MapLayer*>* layer = app->map->data.layers.start;

	while (layer != NULL)
	{
		if (layer->data->Get(x, y) == 0 && x >= 0 && y >= 0 && x < app->map->data.width && y < app->map->data.height)
		{
			return true;
		}
		layer = layer->next;
	}

	return false;
}

int Pathfinding::MovementCost(int x, int y) const
{
	int ret = -1;

	if ((x >= 0) && (x < app->map->data.width) && (y >= 0) && (y < app->map->data.height))
	{
		int id = app->map->data.layers.start->next->data->Get(x, y);

		if (id == 289) ret = 0;
		else if (id == 290) ret = 0;
		else if (id == 294) ret = 0;
		else if (id == 291) ret = 0;
		else ret = 3;
	}

	return ret;
}

DynArray<iPoint>* Pathfinding::ComputePath(int x, int y)
{
	path.Clear();
	iPoint goal = app->map->WorldToMap(x, y);

	int index = 0;
	path.PushBack(goal);
	while (goal != visited.start->data)
	{
		index = visited.Find(goal);
		goal = breadcrumbs[index];
		path.PushBack(goal);
	}

	path.PushBack(visited.start->data);

	return &path;
}

DynArray<iPoint>* Pathfinding::ComputePathAStar(int x, int y)
{
	// L12a: Compute AStart pathfinding
	path.Clear();
	iPoint goal = app->map->WorldToMap(x, y);

	if (finishAStar == true || visited.Find(goal) > -1)
	{
		iPoint aux;

		int index;

		index = visited.Find(goal);
		path.PushBack(visited.At(index)->data);
		aux = breadcrumbs.At(index)->data;

		while (aux != visited.start->data)
		{
			index = visited.Find(aux);
			path.PushBack(visited.At(index)->data);
			aux = breadcrumbs.At(index)->data;
		}

		path.PushBack(visited.start->data);
	}

	return &path;
}

bool Pathfinding::PropagateAStar(int x, int y)
{
	iPoint curr;
	goalAStar = app->map->WorldToMap(x, y);
	while (frontier.Pop(curr))
	{
		if (curr == goalAStar)
			return true;

		iPoint neighbors[4];
		neighbors[0].Create(curr.x + 1, curr.y + 0);
		neighbors[1].Create(curr.x + 0, curr.y + 1);
		neighbors[2].Create(curr.x - 1, curr.y + 0);
		neighbors[3].Create(curr.x + 0, curr.y - 1);

		for (uint i = 0; i < 4; ++i)
		{
			int newCost = MovementCost(neighbors[i].x, neighbors[i].y);
			int g = neighbors[i].DistanceManhattan(visited.start->data);
			int h = neighbors[i].DistanceManhattan(goalAStar);
			if (newCost > 0)
			{
				newCost += costSoFar[curr.x][curr.y];
				if ((visited.Find(neighbors[i]) == -1) || (newCost < costSoFar[neighbors[i].x][neighbors[i].y]))
				{
					int f = g + h;
					costSoFar[neighbors[i].x][neighbors[i].y] = newCost;
					frontier.Push(neighbors[i], h);
					visited.Add(neighbors[i]);
					breadcrumbs.Add(curr);
				}
			}
		}
	}

	return false;
}

void Pathfinding::PropagateBFS(Player* player)
{
	iPoint curr;

	goalAStar = app->map->WorldToMap(player->GetPosition().x, player->GetPosition().y);

	while (frontier.Pop(curr))
	{
		iPoint neighbors[4];
		neighbors[0].Create(curr.x + 1, curr.y + 0);
		neighbors[1].Create(curr.x + 0, curr.y + 1);
		neighbors[2].Create(curr.x - 1, curr.y + 0);
		neighbors[3].Create(curr.x + 0, curr.y - 1);

		if (goalAStar == curr)
			break;

		for (uint i = 0; i < 4; ++i)
		{
			if (MovementCost(neighbors[i].x, neighbors[i].y) > 0)
			{
				if (visited.Find(neighbors[i]) == -1)
				{
					frontier.Push(neighbors[i], 0);
					visited.Add(neighbors[i]);
					breadcrumbs.Add(curr);
				}
			}
		}
	}
}

bool Pathfinding::PropagateDijkstra(Player* player)
{
	iPoint curr;
	goalAStar = app->map->WorldToMap(player->GetPosition().x, player->GetPosition().y + 32);

	while (frontier.Pop(curr))
	{
		iPoint neighbors[4];
		neighbors[0].Create(curr.x + 1, curr.y + 0);
		neighbors[1].Create(curr.x + 0, curr.y + 1);
		neighbors[2].Create(curr.x - 1, curr.y + 0);
		neighbors[3].Create(curr.x + 0, curr.y - 1);

		if (goalAStar == curr)
		{
			return true;
		}

		for (uint i = 0; i < 4; ++i)
		{
			newCost = costSoFar[curr.x][curr.y] + MovementCost(neighbors[i].x, neighbors[i].y);
			if (newCost > costSoFar[curr.x][curr.y] && visited.Find(neighbors[i]) == -1)
			{
				frontier.Push(neighbors[i], newCost);
				visited.Add(neighbors[i]);
				breadcrumbs.Add(curr);
				costSoFar[neighbors[i].x][neighbors[i].y] = newCost;
			}
		}
	}

	return false;
}

bool Pathfinding::Load(pugi::xml_node& load)
{
	return true;
}

bool Pathfinding::Save(pugi::xml_node& save)
{
	return true;
}
#include "Pathfinding.h"
#include "App.h"
#include "Map.h"
#include "Render.h"
#include "Textures.h"

Pathfinding::Pathfinding()
{
}

Pathfinding::~Pathfinding()
{
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

void Pathfinding::DrawPath()
{
	iPoint point;

	// Draw visited
	ListItem<iPoint>* item = visited.start;

	while (item)
	{
		point = item->data;
		TileSet* tileset = app->map->GetTilesetFromTileId(290);

		SDL_Rect rec = tileset->GetTileRect(290);
		iPoint pos = app->map->MapToWorld(point.x, point.y);

		app->render->DrawTexture(tileset->texture, pos.x, pos.y, &rec);

		item = item->next;
	}

	// Draw frontier
	for (uint i = 0; i < frontier.Count(); ++i)
	{
		point = *(frontier.Peek(i));
		TileSet* tileset = app->map->GetTilesetFromTileId(289);

		SDL_Rect rec = tileset->GetTileRect(289);
		iPoint pos = app->map->MapToWorld(point.x, point.y);
		app->render->DrawTexture(tileset->texture, pos.x, pos.y, &rec);
	}

	// Draw path
	for (uint i = 0; i < path.Count(); ++i)
	{
		TileSet* tileset = app->map->GetTilesetFromTileId(291);

		SDL_Rect rec = tileset->GetTileRect(291);
		iPoint pos = app->map->MapToWorld(path[i].x, path[i].y);
		if (checkPath == true)
		{
			app->render->DrawTexture(tileset->texture, pos.x, pos.y, &rec);
		}
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

		if (id == 0) ret = 3;
		else ret = 0;
	}

	return ret;
}

void Pathfinding::ComputePath(int x, int y)
{
	path.Clear();
	iPoint goal = app->map->WorldToMap(x, y);

	ListItem<iPoint>* current = breadcrumbs.end;
	int index = 0;
	while (current != breadcrumbs.start)
	{
		index = visited.Find(current->data);
		current = breadcrumbs.At(index);
		path.PushBack(current->data);
	}

	path.PushBack(breadcrumbs.start->data);
}

void Pathfinding::PropagateBFS()
{
	iPoint curr;

	if (frontier.Pop(curr))
	{
		iPoint neighbors[4];
		neighbors[0].Create(curr.x + 1, curr.y + 0);
		neighbors[1].Create(curr.x + 0, curr.y + 1);
		neighbors[2].Create(curr.x - 1, curr.y + 0);
		neighbors[3].Create(curr.x + 0, curr.y - 1);

		for (uint i = 0; i < 4; ++i)
		{
			if (MovementCost(neighbors[i].x, neighbors[i].y) > 0)
			{
				if (visited.Find(neighbors[i]) == -1)
				{
					frontier.Push(neighbors[i], 0);
					visited.Add(neighbors[i]);

					// L11: TODO 1: Record the direction to the previous node 
					// with the new list "breadcrumps"
					breadcrumbs.Add(curr);
				}
			}
		}
	}
}

void Pathfinding::PropagateDijkstra()
{
	iPoint curr;

	if (frontier.Pop(curr))
	{
		iPoint neighbors[4];
		neighbors[0].Create(curr.x + 1, curr.y + 0);
		neighbors[1].Create(curr.x + 0, curr.y + 1);
		neighbors[2].Create(curr.x - 1, curr.y + 0);
		neighbors[3].Create(curr.x + 0, curr.y - 1);

		for (uint i = 0; i < 4; ++i)
		{
			newCost = costSoFar[curr.x][curr.y] + MovementCost(neighbors[i].x, neighbors[i].y);
			if (newCost > costSoFar[curr.x][curr.y] && visited.Find(neighbors[i]) == -1)
			{
				frontier.Push(neighbors[i], 0);
				visited.Add(neighbors[i]);
				breadcrumbs.Add(curr);
				costSoFar[neighbors[i].x][neighbors[i].y] = newCost;
			}
		}
	}
}

bool Pathfinding::Load(pugi::xml_node& load)
{
	return true;
}

bool Pathfinding::Save(pugi::xml_node& save)
{
	return true;
}

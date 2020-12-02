#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Player.h"
#include "ColliderManagement.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>

Map::Map() : Module(), mapLoaded(false)
{
	name.Create("map");
}

// Destructor
Map::~Map()
{

}

int Properties::GetProperty(const char* value, int defaultValue) const
{
	ListItem<Property*>* prop = list.start;

	while(prop !=NULL)
	{
		if (strcmp(value, prop->data->name.GetString()) == 0)
		{
			return prop->data->value;
		}
		prop = prop->next;
	}

	return defaultValue;
}

bool Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.Create(config.child("folder").child_value());

	return ret;
}

// Draw the map (all required layers)
void Map::Draw()
{
	if (mapLoaded == false) return;

	ListItem<MapLayer*> *layer = data.layers.start;

	while (layer != NULL)
	{
		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int tileId = layer->data->Get(x, y);
				if (tileId > 0)
				{
					iPoint ret = MapToWorld(x, y);
					TileSet* set = GetTilesetFromTileId(tileId);
					
					if (ret.x > (app->render->offset.x - 16)  && ret.x < app->render->offset.x + app->render->camera.w && ret.y > (app->render->offset.y - 16) && ret.y < app->render->offset.y + app->render->camera.h)
					{
						if (layer->data->properties.GetProperty("Drawable") == 1 || viewCollisions == true)
						{
							app->render->DrawTexture(set->texture, ret.x, ret.y, &set->GetTileRect(tileId));
						}
					}
				}
			}
		}
		layer = layer->next;
	}

}

// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tileWidth;
		ret.y = y * data.tileHeight;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (data.tileWidth / 2);
		ret.y = (x + y) * (data.tileHeight / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	if (data.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / data.tileWidth;
		ret.y = y / data.tileHeight;
	}
	else if (data.type == MAPTYPE_ISOMETRIC)
	{

		float half_width = data.tileWidth * 0.5f;
		float half_height = data.tileHeight * 0.5f;
		ret.x = int((x / half_width + y / half_height) / 2);
		ret.y = int((y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}


	return ret;
}

TileSet* Map::GetTilesetFromTileId(int id) const
{
	ListItem<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while (item->next != nullptr)
	{
		if (id <= (set->numTilesWidth * set->numTilesHeight))
		{
			break;
		}
		else
		{
			item = item->next;
			set = item->data;
		}
	}

	return set;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };

	int relativeId = id - firstgid;
	rect.w = tileWidth;
	rect.h = tileHeight;
	rect.x = margin + ((rect.w + spacing) * (relativeId % numTilesWidth));
	rect.y = margin + ((rect.h + spacing) * (relativeId / numTilesWidth));
	
	return rect;
}

// Called before quitting
bool Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	ListItem<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.Clear();

	// Remove all layers
	ListItem<MapLayer*>* itemLayer;
	itemLayer = data.layers.start;

	while (itemLayer != NULL)
	{
		RELEASE(itemLayer->data);
		itemLayer = itemLayer->next;
	}
	data.layers.Clear();

	// Clean up the pugui tree
	mapFile.reset();

	return true;
}

// Load new map
bool Map::Load(const char* filename)
{
	bool ret = true;
	SString tmp("%s%s", folder.GetString(), filename);

	pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
		ret = false;
	}

	// Load general info
	if(ret == true)	ret = LoadMap();

	// Calls a private function to load the tileset values
	pugi::xml_node tileset;
	for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true) ret = LoadTilesetDetails(tileset, set);

		if (ret == true) ret = LoadTilesetImage(tileset, set);

		data.tilesets.Add(set);
	}

	// Load layer info
	pugi::xml_node layer;
	for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		// Create new tileset
		MapLayer* lay = new MapLayer(); 

		ret = LoadLayer(layer, lay);

		if (ret == true) data.layers.Add(lay); 
	}
	
	if(ret == true)
	{
		LOG("--------------------------------------------------------------------------");
		LOG("<< MAP DATA >>");
		LOG("Width=%d", data.width);
		LOG("Height=%d", data.height);
		LOG("TileWidth=%d", data.tileWidth);
		LOG("TileHeight=%d", data.tileHeight);
		LOG("<< END DATA >>\n");

		// L04: TODO 4: LOG the info for each loaded layer
		ListItem<MapLayer*>* layerList;
		layerList = data.layers.start;
		while (layerList != NULL)
		{
			LOG("<< LAYER >>");
			LOG("Name=%s", layerList->data->name.GetString());
			LOG("Width=%d", layerList->data->width);
			LOG("Height=%d", layerList->data->height);
			LOG("<< END LAYER >>\n");
			layerList = layerList->next;
		}
	}

	mapLoaded = ret;

	return ret;
}

bool Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		LOG("Filling Map info");

		data.width = map.attribute("width").as_int(0);
		data.height = map.attribute("height").as_int(0);
		data.tileWidth = map.attribute("tilewidth").as_int(0);
		data.tileHeight = map.attribute("tileheight").as_int(0);

		SString tmp("%s", map.attribute("orientation").as_string());
		if (tmp == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if (tmp == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if (tmp == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

// Loads tileset attributes
bool Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;

	LOG("Filling TilesetDetails");
	set->firstgid = tileset_node.attribute("firstgid").as_int(0);
	set->name = tileset_node.attribute("name").as_string("no");
	set->tileWidth = tileset_node.attribute("tilewidth").as_int(0);
	set->tileHeight = tileset_node.attribute("tileheight").as_int(0);
	set->spacing = tileset_node.attribute("spacing").as_int(0);
	set->margin = tileset_node.attribute("margin").as_int(0);
	set->texWidth = tileset_node.child("image").attribute("width").as_int(0);
	set->texHeight = tileset_node.child("image").attribute("height").as_int(0);
	set->numTilesWidth = set->texWidth / set->tileWidth;
	set->numTilesHeight = set->texHeight / set->tileHeight;

	return ret;
}

// Loads Tileset image
bool Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		SString tmp("%s%s", folder.GetString(), image.attribute("source").as_string("s"));
		set->texture = app->tex->Load(tmp.GetString());
	}

	return ret;
}

// Create the definition for a function that loads the layers
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	// L04: TODO 3: Load a single layer
	layer->name = node.attribute("name").as_string("no");
	layer->width = node.attribute("width").as_int(0);
	layer->height = node.attribute("height").as_int(0);

	layer->data = new unsigned int[layer->width * layer->height];

	memset(layer->data, 0, layer->width * layer->height * sizeof(unsigned int));

	int i = 0;

	for (pugi::xml_node tile = node.child("data").first_child(); tile; tile = tile.next_sibling("tile"))
	{
		layer->data[i] = tile.attribute("gid").as_uint(0);
		i++;
	}

	LoadProperties(node, layer->properties);

	return ret;
}

// Load a group of properties from a node and fills a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = true;

	for (pugi::xml_node proper = node.child("properties").first_child(); proper; proper = proper.next_sibling("property"))
	{
		Properties::Property* prop = new Properties::Property;
		prop->name = proper.attribute("name").as_string();
		prop->value = proper.attribute("value").as_bool();

		properties.list.Add(prop);
	}

	return ret;
}
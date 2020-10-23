
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>

Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    folder.Create(config.child("folder").child_value());

    return ret;
}

// Draw the map (all requried layers)
void Map::Draw()
{
	if (mapLoaded == false) return;

	// L04: DONE 5: Prepare the loop to draw all tilesets + DrawTexture()
	MapLayer* layer = data.layers.start->data;

	for (int y = 0; y < data.height; ++y)
	{
		for (int x = 0; x < data.width; ++x)
		{
			int tileId = layer->Get(x, y);
			if (tileId > 0)
			{
				// L04: TODO 9: Complete the draw function
				SDL_Rect rect = data.tilesets.start->data->GetTileRect(tileId);
				iPoint coords = MapToWorld(x, y);
				app->render->DrawTexture(data.tilesets.start->data->texture, coords.x, coords.y, &rect);
			}
		}
	}
}

// L04: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	if (data.type == MapTypes::MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * data.tileWidth;
		ret.y = y * data.tileHeight;
	}
	else
	{
		ret.x = (x - y) * (data.tileWidth * 0.5f);
		ret.y = (x + y) * (data.tileWidth * 0.5f);
	}

	// L05: TODO 1: Add isometric map to world coordinates

	return ret;
}

// L05: TODO 2: Add orthographic world to map coordinates
iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	 if (data.type == MapTypes::MAPTYPE_ORTHOGONAL)
    {
        ret.x = x / data.tileWidth;
        ret.y = y / data.tileHeight;
    }
    else
    {
        ret.x = (x + y) / (data.tileWidth / 0.5f);
        ret.y = (x - y) / (data.tileWidth / 0.5f);
    }

	// L05: TODO 3: Add the case for isometric maps to WorldToMap

	return ret;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };

	// L04: DONE 7: Get relative Tile rectangle
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

    // L03: DONE 2: Make sure you clean up any memory allocated from tilesets/map
    // Remove all tilesets
	ListItem<TileSet*>* item;
	item = data.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// L04: DONE 2: clean up all layer data
	// Remove all layers
	ListItem<MapLayer*>* item2;
	item2 = data.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();

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
    if(ret == true)
    {
        // L03: DONE 3: Create and call a private function to load and fill all your map data
		ret = LoadMap();
	}

    // L03: DONE 4: Create and call a private function to load a tileset
    // remember to support more any number of tilesets!
	pugi::xml_node tileset;
	for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true) ret = LoadTilesetDetails(tileset, set);

		if (ret == true) ret = LoadTilesetImage(tileset, set);

		data.tilesets.add(set);
	}

	// L04: DONE 4: Iterate all layers and load each of them
	// Load layer info
	pugi::xml_node layer;
	for (layer = mapFile.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* lay = new MapLayer(); // Create new tileset

		ret = LoadLayer(layer, lay);

		if (ret == true)
			data.layers.add(lay); // Add the filled tileset to the list of tilesets
	}
    
    if(ret == true)
    {
        // L03: TODO 5: LOG all the data loaded iterate all tilesets and LOG everything
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

// L03: TODO: Load map general properties
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
		// L03: TODO: Load map general properties
		LOG("Filling Map info");

		data.height = map.attribute("height").as_int();
		data.width = map.attribute("width").as_int();
		data.tileHeight = map.attribute("tileheight").as_int();
		data.tileWidth = map.attribute("tilewidth").as_int();
	}

	return ret;
}

// L03: TODO: Load Tileset attributes
bool Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	
	// L03: TODO: Load Tileset attributes

	LOG("Filling TilesetDetails");
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->name = tileset_node.attribute("name").value();
	set->tileWidth = tileset_node.attribute("tilewidth").as_int();
	set->tileHeight = tileset_node.attribute("tileheight").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	set->margin = tileset_node.attribute("margin").as_int();

	return ret;
}

// L03: TODO: Load Tileset image
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
		// L03: TODO: Load Tileset image
		SString imgSource;
		imgSource.Create("%s%s", folder.GetString(), tileset_node.child("image").attribute("source").value());
		set->texture = app->tex->Load(imgSource.GetString());
		set->texWidth = tileset_node.child("image").attribute("width").as_int();
		set->texHeight = tileset_node.child("image").attribute("height").as_int();
		set->numTilesWidth = set->texWidth / set->tileWidth;
		set->numTilesHeight = set->texHeight / set->tileHeight;
	}

	return ret;
}

// L04: TODO 3: Create the definition for a function that loads a single layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	// L04: TODO 3: Load a single layer
	layer->height = node.attribute("height").as_int();
	layer->width = node.attribute("width").as_int();
	layer->name = node.attribute("name").value();

	int newSize = layer->width * layer->height;

	layer->data = new uint[newSize];

	pugi::xml_node sibling = node.child("data").child("tile");

	for (int i = 0; i < newSize; i++)
	{
		layer->data[i] = sibling.attribute("gid").as_int();
		sibling = sibling.next_sibling("tile");

		return ret;
	}
}
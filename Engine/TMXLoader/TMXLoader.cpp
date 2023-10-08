//
//  TMXLoader.cpp
//  TMXLoader
//
//  Created by Marty on 06/09/2015.
//  Copyright (c) 2015 - 2020 Martin Grant. All rights reserved.
//  Available under MIT license. See License.txt for more information.
//
//  Uses RapidXML for file parsing.
//  Copyright (c) 2006, 2007 Marcin Kalicinski
//  http://rapidxml.sourceforge.net/
//  See /RapidXML/License.txt for more information.
//
//  www.midnightpacific.com
//  contact@midnightpacific.com
//  @_martingrant
//  http://bitbucket.org/martingrant/tmxloader
//
#include <filesystem>
#include "TMXLoader.h"


TMXLoader::TMXLoader()
{
}


//TMXLoader::~TMXLoader()
//{
//    m_mapContainer.clear();
//    std::unordered_map<std::string, std::unique_ptr<TMXMap>>().swap(m_mapContainer);
//}


void TMXLoader::loadMap(std::string mapName, std::string filePath)
{
    // String to hold file contents
    std::string fileContents = "";
    
    // Attempt to load file using provided file path
    bool fileLoaded = loadFile(filePath, fileContents);
    
    if (fileLoaded == true)
    {
        // Create new RapidXML document instance to use to parse map data
        rapidxml::xml_document<char> m_currentMap;
        m_currentMap.parse<0>((char*)fileContents.c_str());
        rapidxml::xml_node<> *parentNode = m_currentMap.first_node("map");
        
        // Add new TMXMap to m_mapContainer
		std::unique_ptr<TMXMap> map = std::make_unique<TMXMap>();
		m_mapContainer[mapName] = std::move(map);
        
        // Load the map settings, tilesets and layers
        loadMapSettings(m_mapContainer[mapName], parentNode);
        loadTileSets(m_mapContainer[mapName], parentNode);
		loadObjectLayers(m_mapContainer[mapName], parentNode);
        loadLayers(m_mapContainer[mapName], parentNode);
        
        std::cout << "TMXLoader: loaded map '" << mapName << "' from: '" << filePath << "' successfully" << std::endl;
    }
    else
    {
        std::cout << "TMXLoader: map '" << mapName << "' at '" << filePath << "' could not be loaded." << std::endl;
    }
}


TMXMap* TMXLoader::getMap(std::string mapName)
{
    // Attempt to find and return a map using provided name, else return nullptr
    
    std::unordered_map<std::string, std::unique_ptr<TMXMap>>::const_iterator iterator = m_mapContainer.find(mapName);
    
    if (iterator == m_mapContainer.end())
    {
        std::cout << "TMXLoader: map '" << mapName << "' not found." << std::endl;
    }
    else
    {
        return iterator->second.get();
    }
    
    return nullptr;
}


void TMXLoader::printMapData(std::string mapName)
{
    // Attempt to print data for a specific map
    
    std::unordered_map<std::string, std::unique_ptr<TMXMap>>::const_iterator iterator = m_mapContainer.find(mapName);
    
    if (iterator == m_mapContainer.end())
    {
        std::cout << "TMXLoader: map '" << mapName << "' not found." << std::endl;
    }
    else
    {
        iterator->second->printData();
    }
}


void TMXLoader::loadMapSettings(std::unique_ptr<TMXMap> const &map, rapidxml::xml_node<> *parentNode)
{
	// Vector to hold map settings (version, orientation, width, height etc.)
	std::unordered_map<std::string, std::string> mapData;

    // Push found settings onto back of vector, which are attributes of first xml node "map"
	for (rapidxml::xml_attribute<char> *attr = parentNode->first_attribute(); attr; attr = attr->next_attribute()) //TG: middle arg could be attr != null cuz that's when this will resolve
	{
		mapData[attr->name()] = attr->value();

	}
	//TG:TODO: create a map for name and value above, then complicate the code below

	
    // Background colour is stored in hexadecimal, next few lines coverts to RGB and pushes onto vector
	if (mapData.contains("backgroundcolor"))
	{
		std::string colourString = mapData["backgroundcolor"];
		std::string colourSubstring = colourString.substr(1, colourString.length());

		unsigned int colour = stoi(colourSubstring, 0, 16);

		mapData["backgroundcolor_r"] = std::to_string(colour / 0x10000);
		mapData["backgroundcolor_g"] = std::to_string((colour / 0x100) % 0x100);
		mapData["backgroundcolor_b"] = std::to_string(colour / 0x10000);
	}

	std::unordered_map<std::string, std::string> propertiesMap;

    // Load any user-defined properties
	loadProperties(propertiesMap, parentNode);
    
    map->setMapSettings(mapData, propertiesMap);
}


void TMXLoader::loadTileSets(std::unique_ptr<TMXMap> const &map, rapidxml::xml_node<> *parentNode)
{
	// Create a new node based on the parent node
	rapidxml::xml_node<> *currentNode = parentNode;

	// Check if there is a tileset node
	if (currentNode->first_node("tileset") != nullptr)
	{
		// Move to the first tileset node
		currentNode = currentNode->first_node("tileset");

		// Use a map to hold data for creating a tile set
		std::unordered_map<std::string, std::string> tileSetData;

		// Use a map to hold tileset properties
		std::unordered_map<std::string, std::string> propertiesMap;

		// Use a vector and map for individual tiles that have properties
		std::vector<TMXTile> tileVector;
		std::unordered_map<std::string, std::string> tileProperties;

		// Loop whilst there are tileset nodes found
		while (currentNode != nullptr)
		{
			// Read tileset data 
			for (rapidxml::xml_attribute<char> *attr = currentNode->first_attribute(); attr; attr = attr->next_attribute())
			{
				tileSetData[attr->name()] = attr->value();
			}

			// Check for offset node
			rapidxml::xml_node<> *offsetNode = currentNode;
			if (offsetNode->first_node("tileoffset") != nullptr)
			{
				offsetNode = offsetNode->first_node("tileoffset");

				tileSetData["tileoffsetX"] = offsetNode->first_attribute()->value();
				tileSetData["tileoffsetY"] = offsetNode->first_attribute()->next_attribute()->value();
			}

			// Clear the properties map of the data from the previous tileset
			propertiesMap.clear();
			// Load tileset properties
			loadProperties(propertiesMap, currentNode);

			// Move to the image childnode and read data
			currentNode = currentNode->first_node("image");
			for (rapidxml::xml_attribute<char> *attr = currentNode->first_attribute(); attr; attr = attr->next_attribute())
			{
				if (strcmp(attr->name(), "trans") == 0)
				{
					unsigned int colour = std::stoi(attr->value(), 0, 16);
                    
                    // Convert from hex to RGB
					tileSetData["red"] = std::to_string(colour / 0x10000);
					tileSetData["green"] = std::to_string((colour / 0x100) % 0x100);
					tileSetData["blue"] = std::to_string(colour / 0x10000);
				}
				else
				{
					tileSetData[attr->name()] = attr->value();
				}
			}

			// Check and load if any individual tiles have properties
			tileVector.clear();
			rapidxml::xml_node<> *tileNode = currentNode->parent()->first_node("tile");
			while (tileNode != nullptr)
			{
				unsigned int tileID = atoi(tileNode->first_attribute()->value());
				loadProperties(tileProperties, tileNode);
				tileVector.push_back(TMXTile(tileID, tileProperties));

				tileProperties.clear();

				if (tileNode->next_sibling("tile") != nullptr)
				{
					tileNode = tileNode->next_sibling("tile");
				}
				else
				{
					break;
				}
			}

			// Pass the new tileset data to the map
 			map->addTileSet(TMXTileSet(tileSetData, propertiesMap, tileVector));

			// Move to the next tileset node and increment the counter
			if (currentNode->parent()->next_sibling("tileset") == nullptr)
			{
				break;
			}
			else
			{
				tileSetData.clear();
				currentNode = currentNode->parent()->next_sibling("tileset");
			}
		}
	}
}


void TMXLoader::loadLayers(std::unique_ptr<TMXMap> const &map, rapidxml::xml_node<> *parentNode)
{
	// Create a new node based on the parent node
	rapidxml::xml_node<> *currentNode = parentNode;

	// Move to first layer node
	currentNode = currentNode->first_node("layer");
    
	while (currentNode != nullptr)
	{

		std::unordered_map<std::string, std::string> layerVector;

		unsigned int layerID = 0;
		std::string layerName;
		unsigned int layerWidth = 0;
		unsigned int layerHeight = 0;
		std::unordered_map<std::string, std::string> layerProperties;


		// Clear these both to hold data for the next layer
		layerProperties.clear();
		layerVector.clear();

		// Read data into the current layer vector
		for (rapidxml::xml_attribute<char>* attr = currentNode->first_attribute(); attr; attr = attr->next_attribute())
		{
			layerVector[attr->name()] = attr->value();
		}

		layerVector.contains("id") ? layerID = std::stoi(layerVector.at("id")) : layerID = 0;
		layerVector.contains("name") ? layerName = layerVector.at("name") : layerName;
		layerWidth = std::stoi(layerVector.at("width"));
		layerHeight = std::stoi(layerVector.at("height"));

		// Load any properties for the layer
		loadProperties(layerProperties, currentNode);

		currentNode = currentNode->first_node("data");

		// Create 2D vector to hold tile data
		std::vector<std::vector<unsigned int>> tileVector(layerHeight, std::vector<unsigned int>(layerWidth));
		unsigned int currentTile = 0;
		int currentRow = 0;

		//TG: new branch based on what seems to be csv map format
		if (currentNode->first_attribute() != nullptr)
		{
			if (std::string(currentNode->first_attribute("encoding")->value()) == "csv")
			{
				std::string layerString = std::string(currentNode->first_node()->value());
				std::stringstream ss(layerString);
				std::string tile;


				while (std::getline(ss, tile, ','))
				{
					if (tile.starts_with("\r\n"))
					{
						tile.erase(0, 2);
					}
					if (currentTile < layerWidth - 1)
					{
						tileVector[currentRow][currentTile] = std::stoi(tile);
						currentTile++;
					}
					else
					{
						tileVector[currentRow][currentTile] = std::stoi(tile);
						currentTile = 0;
						currentRow++;
					}
				}
				// Add the newly read layer to the map
				map->addLayer(TMXTileLayer(layerName, layerWidth, layerHeight, layerProperties, tileVector));

				// Move to the next layer
				currentNode = currentNode->parent()->next_sibling("layer");
			}
			else
			{
				std::cout << "Something went wrong with checking if the map format is csv";
			}
		}
		else //original approach, maybe its still needed?
		{
			currentNode = currentNode->first_node("tile");
			while (currentNode != nullptr)
			{
				if (currentTile < layerWidth)
				{
					// Add tile to vector, must be cast from char* to unsigned int
					tileVector[currentRow][currentTile] = (unsigned int)std::stoul(currentNode->first_attribute()->value());

					currentTile++;

					// Determine if there is another tile to be read or not
					if (currentNode->next_sibling("tile") == nullptr)
						break;
					else
						currentNode = currentNode->next_sibling("tile");
				}
				else
				{
					currentTile = 0;
					currentRow++;
				}
			}
			// Add the newly read layer to the map
			map->addLayer(TMXTileLayer(layerName, layerWidth, layerHeight, layerProperties, tileVector));

			// Move to the next layer
			currentNode = currentNode->parent()->parent()->next_sibling("layer");

		
		}
	}
}

void TMXLoader::loadObjectLayers(std::unique_ptr<TMXMap> const& map, rapidxml::xml_node<>* parentNode)
{
	// Create a new node based on the parent node
	rapidxml::xml_node<>* currentNode = parentNode;

	//iterating through all object layers/"object groups"
	for (rapidxml::xml_node<char>* node = currentNode->first_node("objectgroup"); node; node = node->next_sibling("objectgroup"))
	{
		currentNode = node;
		unsigned int ObjectGroup_ID = 0;
		std::string ObjectGroup_Name;
		std::unordered_map<std::string, std::string> ObjectGroupProperties;

		// Clear this to hold data for the next ObjectGroup
		ObjectGroupProperties.clear();


		// Read data into the current ObjectGroup vector
		for (rapidxml::xml_attribute<char>* attr = currentNode->first_attribute(); attr; attr = attr->next_attribute())
		{
			ObjectGroupProperties[attr->name()] = attr->value();
		}

		ObjectGroupProperties.contains("id") ? ObjectGroup_ID = std::stoi(ObjectGroupProperties.at("id")) : ObjectGroup_ID = 0;
		ObjectGroupProperties.contains("name") ? ObjectGroup_Name = ObjectGroupProperties.at("name") : ObjectGroup_Name;

		// Load any properties for the ObjectGroup
		loadProperties(ObjectGroupProperties, currentNode);

		//temp storage for TMXObject qualities
		std::unordered_map<std::string, std::string> objectMap; 
		std::unordered_map<std::string, std::string> objectPropertiesMap;

		//the vector used to build the Object Layer
		std::vector< std::unique_ptr<TMXObject> > objectVector;
		TMXObject_Type type = TMXObject_Type::None;
		

		//elements of a Object
		unsigned int Object_ID = 0;
		std::string Object_Name;
		unsigned int Object_Width;
		unsigned int Object_Height;
		unsigned int Object_X;
		unsigned int Object_Y;

		//start at 1st object, moving through each next object
		for (rapidxml::xml_node<char>* node = currentNode->first_node("object"); node; node = node->next_sibling("object"))
		{
			currentNode = node;
			//work through all object nodes, eg x,y,width,height,etc
			for (rapidxml::xml_attribute<char>* attr = currentNode->first_attribute(); attr; attr = attr->next_attribute())
			{
				objectMap[attr->name()] = attr->value();
			}
			//if object has properties
			if (currentNode->first_node("properties") != nullptr)
			{
				// Move to the properties node
				currentNode = currentNode->first_node("properties");
				// Move to the first property node
				currentNode = currentNode->first_node("property");
				for (rapidxml::xml_attribute<char>* attr = currentNode->first_attribute(); attr; attr = attr->next_attribute())
				{
					objectPropertiesMap[attr->name()] = attr->value();
				}
				//leave property node subset (important to back up a node when there are properties because not every node will have properties)
				currentNode = currentNode->parent()->parent(); //back to object
			}

			
			//check for special object types
			bool found_type = false;
			for (rapidxml::xml_node<char>* child = currentNode->first_node(); child; child = child->next_sibling())
			{
				if (std::string(child->name()) == "ellipse")
				{
					found_type = true;
					type = TMXObject_Type::Ellipse;
				}
			}
			if (!found_type)
			{
				if (type == TMXObject_Type::None &&
					objectMap.contains("width") && objectMap.contains("height"))
				{
					type = TMXObject_Type::Rectangle;
				}
			}

			//move object details from a map to a TMXObject
			//these elements are true for any object type
			objectMap.contains("id") ? Object_ID = std::stoi(objectMap.at("id")) : Object_ID = 0;
			objectMap.contains("name") ? Object_Name = objectMap.at("name") : Object_Name;
			objectMap.contains("x") ? Object_X = std::stoi(objectMap.at("x")) : Object_X = 0;
			objectMap.contains("y") ? Object_Y = std::stoi(objectMap.at("y")) : Object_Y = 0;
			objectMap.contains("width") ? Object_Width = std::stoi(objectMap.at("width")) : Object_Width = 0;
			objectMap.contains("height") ? Object_Height = std::stoi(objectMap.at("height")) : Object_Height = 0;

			//make object element assignments based on type & emplace object into vector
			std::unique_ptr<TMXObject_Rectangle> obj;
			switch (type)
			{
			case TMXObject_Type::Rectangle:
			{
				obj = std::make_unique<TMXObject_Rectangle>(
					Object_Name,
					Object_X, Object_Y,
					Object_Width, Object_Height,
					objectPropertiesMap);

				objectVector.push_back(std::move(obj));
			}
			case TMXObject_Type::Ellipse:
			{
				obj = std::make_unique<TMXObject_Ellipse>(
					Object_Name,
					Object_X, Object_Y,
					Object_Width, Object_Height,
					objectPropertiesMap);

				objectVector.push_back(std::move(obj));
			}

			}

			objectMap.clear();
			objectPropertiesMap.clear();
			found_type = false;
			type = TMXObject_Type::None;
		}
		// Add the newly read layer to the map
		map->addObjectLayer(std::make_unique<TMXObjectLayer>(ObjectGroup_Name, ObjectGroupProperties, std::move(objectVector)));
	} 

}

void TMXLoader::loadProperties(std::unordered_map<std::string, std::string>& propertiesMap, rapidxml::xml_node<> *parentNode)
{
	// Create a new node based on the parent node
	rapidxml::xml_node<> *currentNode = parentNode;

	// Check if there is a properties node
	if (currentNode->first_node("properties") != nullptr)
	{
		// Move to the properties node
		currentNode = currentNode->first_node("properties");
		// Move to the first property node
		currentNode = currentNode->first_node("property");

		// Loop whilst there are property nodes found
		while (currentNode != nullptr)
		{
			propertiesMap[currentNode->first_attribute()->value()] = currentNode->first_attribute()->next_attribute()->value();
			currentNode = currentNode->next_sibling("property");
		}
	}
}


bool TMXLoader::loadFile(std::string filePath, std::string &fileContents)
{
	std::cout << std::filesystem::current_path().string() << std::endl;

    std::ifstream file(filePath, std::ios::in | std::ios::binary);

    if (file)
    {
        file.seekg(0, std::ios::end);
        fileContents.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(&fileContents[0], fileContents.size());
        file.close();
        
        return true;
    }
    return false;
}



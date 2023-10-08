//
//  TMXMap.cpp
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
//  http://github.com/martingrant/tmxloader

#include <iostream>
#include "TMXMap.h"

TMXMap::TMXMap() : m_version(0.0f), m_width(0), m_height(0), m_tileWidth(0), m_tileHeight(0), m_backgroundColour({}) {}

TMXMap::~TMXMap() 
{
    m_propertiesMap.clear();
    std::unordered_map<std::string, std::string>{}.swap(m_propertiesMap);

    m_tileVector.clear();
    std::vector<TMXTileSet>{}.swap(m_tileVector);

    m_layerVector.clear();
    std::vector<TMXTileLayer>{}.swap(m_layerVector);
}

void TMXMap::setMapSettings(std::unordered_map<std::string, std::string> const &mapData, std::unordered_map<std::string, std::string> const &mapProps) 
{
    mapData.contains("version") ? m_version = std::stof(mapData.at("version")) : m_version = 0.0f;
    m_orientation = mapData.at("orientation");
    m_renderOrder = mapData.at("renderorder");
    m_width = std::stoi(mapData.at("width"));
    m_height = std::stoi(mapData.at("height"));
    m_tileWidth = std::stoi(mapData.at("tilewidth"));
    m_tileHeight = std::stoi(mapData.at("tileheight"));
    //TODO add:
    //infinite
    //nextlayerid
    //nextobjectid

    mapData.contains("backgroundcolor_r") ? m_backgroundColour[0] = std::stoi(mapData.at("backgroundcolor_r")) : m_backgroundColour[0] = 0;
    mapData.contains("backgroundcolor_g") ? m_backgroundColour[0] = std::stoi(mapData.at("backgroundcolor_g")) : m_backgroundColour[0] = 0;
    mapData.contains("backgroundcolor_b") ? m_backgroundColour[0] = std::stoi(mapData.at("backgroundcolor_b")) : m_backgroundColour[0] = 0;

    m_propertiesMap = mapProps;
}

float TMXMap::getVersion() const  { return m_version; }

unsigned TMXMap::getWidth() const  { return m_width; }
unsigned TMXMap::getHeight() const  { return m_height; }
unsigned TMXMap::getTileWidth() const  { return m_tileWidth; }
unsigned TMXMap::getTileHeight() const  { return m_tileHeight; }

std::array<unsigned, 3> TMXMap::getBackgroundColor() const  { return m_backgroundColour; }

std::string TMXMap::getOrientation() const  { return m_orientation; }
std::string TMXMap::getRenderOrder() const  { return m_renderOrder; }

void TMXMap::addTileSet(TMXTileSet const &newTileSet)  { m_tileVector.push_back(newTileSet); }

TMXTileSet *TMXMap::getTileset(std::string const &tileSetName) 
{
    for (unsigned idx{0}; idx < m_tileVector.size(); ++idx)
        if (m_tileVector[idx].getName() == tileSetName)
            return &m_tileVector[idx];
    std::cout << "TMXLoader: tileset layer '" << tileSetName << "' could not be found." << std::endl;
    return nullptr;
}

void TMXMap::addLayer(TMXTileLayer const &newLayer)  { m_layerVector.push_back(newLayer); }

TMXTileLayer *TMXMap::getLayer(std::string const &layerName) 
{
    for (unsigned idx{0}; idx < m_layerVector.size(); ++idx)
        if (m_layerVector[idx].getName() == layerName)
            return &m_layerVector[idx];
    std::cout << "TMXLoader: tile layer '" << layerName << "' could not be found." << std::endl;
    return nullptr;
}

void TMXMap::addObjectLayer(std::unique_ptr<TMXObjectLayer> newObjectLayer)  
{ 
    m_objectlayerVector.push_back(std::move(newObjectLayer)); 
}

//TMXObjectLayer* TMXMap::getObjectLayer(std::string const& objectlayerName) 
//{
//    for (unsigned idx{ 0 }; idx < m_objectlayerVector.size(); ++idx)
//        if (m_objectlayerVector[idx].getName() == objectlayerName)
//            return &m_objectlayerVector[idx];
//    std::cout << "TMXLoader: object layer '" << objectlayerName << "' could not be found." << std::endl;
//    return nullptr;
//}

void TMXMap::printData()
{
    std::cout << "\nVersion: " << m_version
              << "\nOrientation: " << m_orientation
              << "\nWidth: " << m_width
              << "\nHeight: " << m_height
              << "\nTile Width: " << m_tileWidth
              << "\nTile Height: " << m_tileHeight
              << "\nBackground Colour: " << m_backgroundColour[0] << "," << m_backgroundColour[1] << "," << m_backgroundColour[2]
              << "\nRender Order: " << m_renderOrder;

    std::cout << "\n\nTest map properties:\n";
    for (auto index = m_propertiesMap.begin(); index != m_propertiesMap.end(); ++index)
    {
        std::cout << index->first << " - " << index->second << std::endl;
    }

    std::cout << "\n\nTest map tilesets:\n";
    for (unsigned int index = 0; index < m_tileVector.size(); ++index)
    {
        m_tileVector[index].printData();
    }

    std::cout << "\n\nTest map layers:\n";
    for (unsigned int index = 0; index < m_layerVector.size(); ++index)
    {
        m_layerVector[index].printData();
    }
    std::cout << "\n\nTest map object layers:\n";
    for (unsigned int index = 0; index < m_objectlayerVector.size(); ++index)
    {
        //m_objectlayerVector[index].printData();
    }
}
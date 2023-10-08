//
//  TMXMap.h
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
//

#pragma once

#include "TMXTileSet.h"
#include "TMXTileLayer.h"
#include "TMXObjectLayer.h"

class TMXMap final
{
public:
    /* Class constructors & destructors */
    TMXMap();
    ~TMXMap();

    /* Setter functions for map settings */
    void setMapSettings(
        std::unordered_map<std::string, std::string> const &mapData,
        std::unordered_map<std::string, std::string> const &mapProps);

    /* Getter functions for map settings */
    float getVersion() const;
    unsigned getWidth() const;
    unsigned getHeight() const;
    unsigned getTileWidth() const;
    unsigned getTileHeight() const;
    std::array<unsigned, 3> getBackgroundColor() const;
    std::string getOrientation() const;
    std::string getRenderOrder() const;

    /* TileSets */
    void addTileSet(TMXTileSet const &newTileSet);
    TMXTileSet *getTileset(std::string const &tileSetName);

    /* Layers */
    void addLayer(TMXTileLayer const &newLayer) ;
    TMXTileLayer *getLayer(std::string const &layerName) ;

    /* Object Layers */
    void addObjectLayer(std::unique_ptr<TMXObjectLayer> newObjectLayer) ;
    //TMXObjectLayer* getObjectLayer(std::string const& objectlayerName) ;

    /* Debug functions */
    void printData();

private:
    /* Map variables */
    float m_version;
    unsigned m_width, m_height;
    unsigned m_tileWidth, m_tileHeight;
    std::array<unsigned, 3> m_backgroundColour;
    std::string m_orientation;
    std::string m_renderOrder;

    /* User-defined properties */
    std::unordered_map<std::string, std::string> m_propertiesMap;

    std::vector<TMXTileLayer> m_layerVector;
    std::vector<std::unique_ptr<TMXObjectLayer>> m_objectlayerVector;
    std::vector<TMXTileSet> m_tileVector;
};

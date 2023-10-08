//
//  TMXTileSet.h
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

#include <array>
#include <vector>
#include "TMXTile.h"

class TMXTileSet final
{
public:
	/* Class constructors & destructors */
	TMXTileSet(
		std::unordered_map<std::string, std::string> const&tileSetData,
		std::unordered_map<std::string, std::string> const &propertiesMap,
		std::vector<TMXTile> const &tileVector);
	~TMXTileSet() ;

	/* Getter functions */
	unsigned getFirstGID() const ;
	unsigned getImageWidth() const ;
	unsigned getImageHeight() const ;
	unsigned getTileWidth() const ;
	unsigned getTileHeight() const ;
	unsigned getSpacing() const ;
	unsigned getMargin() const ;
	unsigned getOffsetX() const ;
	unsigned getOffsetY() const ;
	unsigned getTileCount() const ;
	std::array<unsigned, 3> getTransparentColour() const ; // (maybe) return a SDL_Color ??
	std::string getName() const ;
	std::string getSource() const ;
	std::string getProperty(std::string const &propertyName) ;
	TMXTile *getTile(unsigned tileID) ;

	/* Debug functions */
	void printData();

private:
	unsigned m_firstGID;
	unsigned m_imageWidth, m_imageHeight;
	unsigned m_tileWidth, m_tileHeight;
	unsigned m_spacing;
	unsigned m_margin;
	unsigned m_offsetX;
	unsigned m_offsetY;
	unsigned m_tileCount;
	std::array<unsigned, 3> m_transparentColour;
	std::string m_name;
	std::string m_source;
	std::unordered_map<std::string, std::string> m_propertiesMap;
	std::vector<TMXTile> m_tileVector;
};

//
//  TMXTileLayer.cpp
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

#include <iostream>
#include "TMXTileLayer.h"

TMXTileLayer::TMXTileLayer(
    std::string const &name,
    unsigned width, unsigned height,
    std::unordered_map<std::string, std::string> const &layerProperties,
    std::vector<std::vector<unsigned>> const &tileVector)
    : 
    m_name{name}, 
    m_width{width}, 
    m_height{height},
    m_layerProperties{layerProperties}, 
    m_tileVector{tileVector} 
{}

TMXTileLayer::~TMXTileLayer() 
{
    m_tileVector.clear();
    std::vector<std::vector<unsigned>>{}.swap(m_tileVector);

    m_layerProperties.clear();
    std::unordered_map<std::string, std::string>{}.swap(m_layerProperties);
}

unsigned TMXTileLayer::getWidth() const  { return m_width; }
unsigned TMXTileLayer::getHeight() const  { return m_height; }

std::string TMXTileLayer::getName() const  { return m_name; }
std::vector<std::vector<unsigned>> TMXTileLayer::getTiles() const  { return m_tileVector; }

std::string TMXTileLayer::getProperty(std::string const &propertyName) 
{
    
    if (std::unordered_map<std::string, std::string>::const_iterator
            it{m_layerProperties.find(propertyName)};
        it != m_layerProperties.end())
        return it->second;
    std::cout << "TMXLoader: property '" << propertyName << "' not found." << std::endl;
    
    return {};
}

void TMXTileLayer::printData()
{
    std::cout << "\nName: " << m_name
              << "\nWidth: " << m_width
              << "\nHeight: " << m_height
              << "\nLayer Properties:";

    for (auto index = m_layerProperties.begin(); index != m_layerProperties.end(); ++index)
        std::cout << "\n"
                  << index->first << " - " << index->second << std::endl;

    std::cout << "\nTiles: ";
    std::cout << std::endl;
    for (unsigned int row = 0; row < m_height; ++row)
    {
        for (unsigned int column = 0; column < m_width; ++column)
        {
            std::cout << m_tileVector[row][column] << " ";

            if (column == m_width)
            {
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;
    }
}
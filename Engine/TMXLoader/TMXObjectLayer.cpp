#include <iostream>
#include "TMXObjectLayer.h"

TMXObjectLayer::TMXObjectLayer(
	std::string const& name, 
	std::unordered_map<std::string, std::string> const& layerProperties, 
	std::vector< std::unique_ptr<TMXObject> >&& objectVector)
	:
	m_name{ name },
	m_layerProperties{ layerProperties },
	m_objectVector( std::move(objectVector) )
{}



std::string TMXObjectLayer::getName() const 
{
	return m_name;
}

std::string TMXObjectLayer::getProperty(std::string const& propertyName) 
{
	if (std::unordered_map<std::string, std::string>::const_iterator
		it{ m_layerProperties.find(propertyName) };
		it != m_layerProperties.end())
		return it->second;
	std::cout << "TMXLoader: property '" << propertyName << "' not found." << std::endl;

	return {};
}

//std::vector< std::unique_ptr<TMXObject> > TMXObjectLayer::getObjects()  { return m_objectVector; }

void TMXObjectLayer::printData()
{
	std::cout << "\nName: " << m_name
			<< "\nLayer Properties:";

	for (auto index = m_layerProperties.begin(); index != m_layerProperties.end(); ++index)
	{
		std::cout << "\n"
			<< index->first << " - " << index->second << std::endl;
	}

	std::cout << "\nObjects:";
	for (const auto& object : m_objectVector)
	{
		object->printData();
	}
}

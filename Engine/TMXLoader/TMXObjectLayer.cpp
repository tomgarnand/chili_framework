#include <iostream>
#include "TMXObjectLayer.h"

TMXObjectLayer::TMXObjectLayer(
	std::string const& name, 
	std::unordered_map<std::string, std::string> const& layerProperties, 
	std::vector<TMXObject> const& objectVector)
	:
	m_name{ name },
	m_layerProperties{ layerProperties },
	m_objectVector{ objectVector }
{}

TMXObjectLayer::~TMXObjectLayer() noexcept
{
	m_objectVector.clear();
	std::vector<TMXObject>{}.swap(m_objectVector);
	m_layerProperties.clear();
	std::unordered_map<std::string, std::string>{}.swap(m_layerProperties);
}

std::string TMXObjectLayer::getName() const noexcept
{
	return m_name;
}

std::string TMXObjectLayer::getProperty(std::string const& propertyName) noexcept
{
	if (std::unordered_map<std::string, std::string>::const_iterator
		it{ m_layerProperties.find(propertyName) };
		it != m_layerProperties.end())
		return it->second;
	std::cout << "TMXLoader: property '" << propertyName << "' not found." << std::endl;

	return {};
}

std::vector<TMXObject> TMXObjectLayer::getObjects() noexcept { return m_objectVector; }

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
	for (auto& object : m_objectVector)
	{
		object.printData();
	}
}

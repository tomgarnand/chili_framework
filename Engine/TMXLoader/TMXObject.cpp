
#include "TMXObject.h"
#include <iostream>

TMXObject::TMXObject(std::string const& name, unsigned x, unsigned y, std::unordered_map<std::string, std::string> const& propertiesMap)
	:
	m_x (x),
	m_y (y),
	m_object_name(name)
{
}



unsigned TMXObject::getX() const 
{
	return m_x;
}

unsigned TMXObject::getY() const 
{
	return m_y;
}

std::string TMXObject::getProperty(std::string const& propertyName) 
{
	if (auto it{ m_propertiesMap.find(propertyName) }; it != m_propertiesMap.end())
		return it->second;

	return {};
}

void TMXObject::printData()
{
	std::cout << "Object ID: " << m_object_ID;
	std::cout << "\n Object Name: " << m_object_name;
	std::cout << "\n Object Type: " << TMXObject_TypeFunc::getString(m_object_type);
	std::cout << "\n X" << m_x;
	std::cout << "\n Y" << m_y;
	if (!m_propertiesMap.empty())
	{

		std::cout << "\n Object Properties: " << std::endl;
		for (auto index = m_propertiesMap.begin(); index != m_propertiesMap.end(); ++index)
		{
			std::cout << "\n" << index->first << " - " << index->second << std::endl;
		}
	}

}

TMXObject_Rectangle::TMXObject_Rectangle(std::string const& name, unsigned x, unsigned y, 
	unsigned width, unsigned height, 
	std::unordered_map<std::string, std::string> const& propertiesMap)
	:
	TMXObject(name, x, y, propertiesMap),
	m_width(width),
	m_height(height)
{
	m_object_type = TMXObject_Type::Rectangle;
}

unsigned TMXObject_Rectangle::getWidth() const 
{
	return m_width;
}

unsigned TMXObject_Rectangle::getHeight() const 
{
	return m_height;
}

void TMXObject_Rectangle::printData()
{
	TMXObject::printData();
	std::cout << "\n Width: " << m_width;
	std::cout << "\n Height: " << m_height;
}

TMXObject_Ellipse::TMXObject_Ellipse(std::string const& name, unsigned x, unsigned y,
	unsigned width, unsigned height,
	std::unordered_map<std::string, std::string> const& propertiesMap)
	:
	TMXObject(name, x, y, propertiesMap),
	m_width(width),
	m_height(height)
{
	m_object_type = TMXObject_Type::Ellipse;
}

unsigned TMXObject_Ellipse::getWidth() const
{
	return m_width;
}

unsigned TMXObject_Ellipse::getHeight() const
{
	return m_height;
}

void TMXObject_Ellipse::printData()
{
	TMXObject::printData();
	std::cout << "\n Width: " << m_width;
	std::cout << "\n Height: " << m_height;
}
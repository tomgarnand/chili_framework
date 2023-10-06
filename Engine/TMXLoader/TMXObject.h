#pragma once

#include <string>
#include <unordered_map>

enum class TMXObject_Type {Rectangle, Point, Polygon, Ellipse, Tile, Text, None};

struct TMXObject_TypeFunc {
    static std::string getString(TMXObject_Type type)
    {
        switch (type)
        {
        case TMXObject_Type::Rectangle:
            return "Rectangle";
        case TMXObject_Type::Point:
            return "Point";
        case TMXObject_Type::Polygon:
            return "Polygon";
        case TMXObject_Type::Ellipse:
            return "Ellipse";
        case TMXObject_Type::Tile:
            return "Tile";
        case TMXObject_Type::Text:
            return "Text";
        case TMXObject_Type::None:
            return "None";
        }
    }
};

class TMXObject
{
public:
    /* Class constructors & destructors */
    TMXObject(std::string const& name, 
        unsigned x, unsigned y, 
        std::unordered_map<std::string, std::string> const& propertiesMap);
    ~TMXObject() noexcept;

    /* Getter functions */
    unsigned getX() const noexcept;
    unsigned getY() const noexcept;
    std::string getProperty(std::string const& propertyName) noexcept;

    /* Debug functions */
    void printData();

protected:
    /* TMXObject variables */
    unsigned m_x, m_y;
    std::string m_object_ID;
    std::string m_object_name;
    TMXObject_Type m_object_type = TMXObject_Type::None;

    /* User-defined properties */
    std::unordered_map<std::string, std::string> m_propertiesMap;
};

class TMXObject_Rect : TMXObject
{
public:
    TMXObject_Rect(std::string const& name,
        unsigned x, unsigned y,
        unsigned width, unsigned height,
        std::unordered_map<std::string, std::string> const& propertiesMap);

    unsigned getWidth() const noexcept;
    unsigned getHeight() const noexcept;
    void printData();

private:
    unsigned m_width, m_height;

};
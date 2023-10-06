#pragma once

#include <vector>
#include <unordered_map>
#include "TMXObject.h"

class TMXObjectLayer final
{
public:
    /* Class constructors & destructors */
    TMXObjectLayer(std::string const& name, 
        std::unordered_map<std::string, std::string> const& layerProperties,
        std::vector<TMXObject> const& objectVector);
    ~TMXObjectLayer() noexcept;

    /* Getter functions  */
    std::string getName() const noexcept;
    std::string getProperty(std::string const& propertyName) noexcept;
    std::vector<TMXObject> getObjects() noexcept;

    /* Debug functions */
    void printData();

private:
    /* Layer variables */
    std::string m_name;
    std::vector<TMXObject> m_objectVector;
    std::unordered_map<std::string, std::string> m_layerProperties;
};

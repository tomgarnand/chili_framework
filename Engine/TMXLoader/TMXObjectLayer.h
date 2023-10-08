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
        std::vector< std::unique_ptr<TMXObject> >&& objectVector);


    /* Getter functions  */
    std::string getName() const ;
    std::string getProperty(std::string const& propertyName) ;
    //std::vector< std::unique_ptr<TMXObject> > getObjects() ;

    /* Debug functions */
    void printData();

private:
    /* Layer variables */
    std::string m_name;
    std::vector< std::unique_ptr<TMXObject> > m_objectVector;
    std::unordered_map<std::string, std::string> m_layerProperties;
};

#pragma once
#include <vector>
#include "SelectionMenu.h"


class Collection //game container
{
public:
	class Element //game object
	{
	public:
		Element()
		{

		}
		std::string GetString() { return name; }
		void Use()
		{

		}
	protected:
		std::string name;

	};
	class Item : public Element
	{
		Item()
			:
			confirm(confirm)
		{

		}
		bool CreateMenu()
		{

		}
		void Use()
		{
			
		}
	private:
		SelectionMenu* confirm = nullptr;
	};


public:
	Collection() = default;
	Collection(SelectionMenu* confirm)
		:
		confirm(confirm)
	{

	}
	void AddElement(Element element)
	{
		
		collection.emplace_back(element);
	}

	std::vector<std::string> GetStrings()
	{
		std::vector<std::string> strings;

		for (auto& c : collection)
		{
			strings.emplace_back(c.GetString());
		}
		return strings;
	}
	std::vector<Element> GetElements()
	{
		return collection;
	}
	void Use(int i)
	{
		collection[i].Use();
	}

private:
	std::vector<Element> collection;
	SelectionMenu* confirm = nullptr;
};

#pragma once
#include <vector>




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
		virtual void Use() = 0;
	protected:
		std::string name;

	};
	class Item : public Element
	{
		Item()
		{

		}
		bool CreateMenu()
		{

		}
		void Use() override
		{
			if (need_target)
			{
				if (target_self_only)
				{
					confirmation->Open();
					//pretty much any choices pertainate to the item would be made via a selectionmenu
					//maybe an individual item has its own custom menu, that could be stored in the item
				}
			}
			//then we need to return a pointer to the choice select to use a function to modify the selection
		}
	private:
		bool need_target;
		bool target_self_only;
		bool target_other_only;
		bool target_any_unit;
		bool target_enviornment;
		
		SelectionMenu* confirmation;
		//Target target;

	};
public:
	Collection() = default;
	Collection()
	{

	}
	void AddElement(Element element)
	{
		
		collection.emplace_back(element);
	}
	void AddElement(Item item)
	{

		collection.emplace_back(item);
	}
	void InitPairedCollection(std::vector<Element>* pair)
	{
		paired_collection = pair;
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
	std::vector<Element>* pGetCollection()
	{
		return &collection;
	}
	void Use(int i)
	{
		collection[i].Use();
	}
private:
	std::vector<Element> collection;
	std::vector<Element>* paired_collection = nullptr;
};

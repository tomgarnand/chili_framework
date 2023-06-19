#pragma once
#include <vector>
#include <string>

class SelectionMenu; //forward declaration

class Collection //game container
{
public:
	class Element //game object
	{
	public:
		Element(std::string name, SelectionMenu* menu)
			:
			name(name),
			menu(menu)
		{
			remove_after_use = true;
		}
		std::string GetString() { return name; }
		virtual void Use() = 0;
		SelectionMenu* pGetMenu()
		{
			return menu;
		}
		bool CheckRemove()
		{
			return remove_after_use;
		}
	protected:
		std::string name;
		SelectionMenu* menu;
		bool remove_after_use;

	};
	class Item : public Element
	{
	public:
		Item(std::string name, SelectionMenu* menu)
			:
			Element(name, menu)
		{
			remove_after_use = true;
		}
		void Use() override
		{
			if (need_target)
			{
				if (target_self_only)
				{
					//pretty much any choices pertainate to the item would be made via a selectionmenu
					//maybe an individual item has its own custom menu, that could be stored in the item
				}
			}
			//then we need to return a pointer to the choice select to use a function to modify the selection
		}
	private:
		bool need_target = false;
		bool target_self_only = false;
		bool target_other_only = false;
		bool target_any_unit = false;
		bool target_enviornment = false;
		//Target target;
	};
	class Equipment : public Element
	{
	public:
		Equipment(std::string name, SelectionMenu* menu)
			:
			Element(name, menu)
		{
			remove_after_use = true;
		}
		void Use() override
		{
			paired_collection->AddElement(this);
			paired_collection->QueueUpdate();
			
		}
		void InitPairedCollection(Collection* pair)
		{
			paired_collection = pair;
		}
	private:
		Collection* paired_collection = nullptr;
	};
public:
	Collection() = default;
	
	

	void AddElement(Element* element)
	{
		collection.emplace_back(element);
		if (Equipment* equipment = dynamic_cast<Equipment*>(element))
		{
			equipment->InitPairedCollection(paired_collection);
			paired_collection->QueueUpdate();
		}
		QueueUpdate(); //this
	}
	void RemoveElement(Element* element)
	{
		if (element->CheckRemove() == true)
		{
			collection.erase(std::find(collection.begin(), collection.end(), element));
			QueueUpdate();
		}
	}
	void QueueUpdate()
	{
		queue_update = true;
	}
	bool IsQueueUpdate()
	{
		return queue_update;
	}
	void ResetQueueUpdate()
	{
		queue_update = false;
	}
	void InitPairedCollection(Collection* pair)
	{
		paired_collection = pair;
	}
	Collection* pGetPairedCollection()
	{
		return paired_collection;
	}
	std::vector<std::string> GetStrings()
	{
		std::vector<std::string> strings;

		for (auto& c : collection)
		{
			strings.emplace_back(c->GetString());
		}
		return strings;
	}
	std::vector<Element*>& pGetpElements()
	{
		return collection;
	}

	Element* pGetElement(size_t i)
	{
		return collection[i];
	}

private:
	std::vector<Element*> collection;
	Collection* paired_collection = nullptr;
	bool queue_update = false;
};

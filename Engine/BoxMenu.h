#pragma once

#include "Graphics.h"
#include <vector>

class BoxMenu
{
public:
	//The big difference between a BoxItem and SelectionItem, is that BoxItems are fixed based on the available space on the screen, and SelectionItems are moved to different assigned BoxItems
	class BoxItem
	{
	public:
		BoxItem(RectI rect, int iter)
			:
			rect(rect),
			iter(iter)
		{}
		void ResetHighlight() { highlighted = false; }
		void SetHighlight() { highlighted = true; }
		bool IsHighlighted() const { return highlighted; }

		void SetSelectedTab() { SelectedTab = true; }
		void ResetSelectedTab() { SelectedTab = false; }
		bool IsSelectedTab() const { return SelectedTab; }

		RectI GetRect() const { return rect; }
		int GetIter() const { return iter; }

		bool IsHit(const Vei2& pt) const { return rect.left < pt.x&& rect.right > pt.x && rect.top < pt.y&& rect.bottom > pt.y; }
		static constexpr Color highlightColor = Colors::Yellow;
	private:
		
		RectI rect;
		int iter;

		bool highlighted = false;
		bool SelectedTab = false;
	};

	//This is the container for one variant of BoxMenu to be paired with a SelectionMenu
	BoxMenu(const RectI menuRect, int rows, int height, bool center)
		:
		menuRect(menuRect),
		rows(rows),
		centered(center),
		height(height)
	{

		RectI adjustedRect = RectI(Vei2(menuRect.left, menuRect.top) + GetTopOffsetMenu(), (Vei2(menuRect.right, menuRect.bottom) - GetTopOffsetMenu()));
		boxRectIs = SelectionRects(adjustedRect, rows, height);
		for (int i = 0; i < boxRectIs.size(); i++)
		{
			boxItems.emplace_back(boxRectIs[i], i);
		}
	}
	std::vector<BoxItem> GetBoxItems(){return boxItems;}
	BoxItem* pGetBoxItems() { return boxItems.data(); } 
	RectI GetMenuRect() const { return menuRect; }
	std::vector<RectI> GetBoxItemRects() const { return boxRectIs;  }
	Color GetColor() const { return color;  }
	bool IsCentered() const { return centered; }
	int GetNumRows() const { return rows;  }
	std::vector<RectI> SelectionRects(RectI space, int rows, int height)
	{

		int numRects = ((space.bottom - space.top) / height) * rows;
		int x0 = space.left;
		int y0 = space.top;
		//(x1,y1) is the bottom right of the first subRect
		int x1 = space.left + ((space.right - space.left) / rows);
		int y1 = space.top + height;

		int x = x1 - x0;
		int y = y1 - y0;

		std::vector<RectI> grid( numRects );

		int r = 0;
		for (int i = 0; i < numRects; i++)
		{
			if (r >= rows)
			{
				x0 = space.left;
				x1 = space.left + ((space.right - space.left) / rows);
				y0 += y;
				y1 += y;
				r = 0;
			}
			grid[i] = (RectI(x0 + xOff, x1 - xOff, y0 + yOff, y1 - yOff));
			x0 += x;
			x1 += x;
			r++;
		}
		return grid;
	}

	bool MenuIsHit(const Vei2& pt) const
	{
		return menuRect.left + GetTopOffsetMenu().x < pt.x&& menuRect.right - GetTopOffsetMenu().x > pt.x && menuRect.top + GetTopOffsetMenu().y < pt.y&& menuRect.bottom - GetTopOffsetMenu().y > pt.y;
	}
	void ResetHighlights()
	{
		for (auto& i : boxItems)
		{
			i.ResetHighlight();
		}
	}
	void ResetSelectedTab()
	{
		for (auto& i : boxItems)
		{
			i.ResetSelectedTab();
		}
	}
	static Vei2 GetTopOffsetMenu()
	{
		return { 20, 20 };
	}

private:
	static constexpr Color color = Colors::Blue;
	std::vector<BoxItem> boxItems;
	std::vector<RectI> boxRectIs;
	const RectI menuRect;
	int height;//height of one single line
	int rows;//total number of rows
	int xOff = 0;
	int yOff = 0;
	bool centered = false;
};

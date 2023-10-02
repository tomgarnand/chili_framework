#pragma once
#include <tuple>

enum class ApplicationType
{
	PhysicalDamage,
	Burn
};

class Action
{
	Action() = default;

	int GetMaxTicks() { return maxTicks; }
	std::vector<std::pair<ApplicationType, int>> GetApplicationByTick(int tick)
	{
		std::vector<std::pair<ApplicationType, int>> tempVec;
		for (auto& apply : Applications)
		{
			if (std::get<0>(apply) == tick)
			{
				tempVec.emplace_back(std::get<1>(apply), std::get<2>(apply));
			}
		}
		return tempVec;
	}

	

private:
	int maxTicks;
	std::vector<std::tuple<int, ApplicationType, int>> Applications;



};

static Action Idle;
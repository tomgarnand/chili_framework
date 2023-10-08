#pragma once
#include <map>

enum class ApplicationType
{
	Physical,
	Burn,
	Stun
};

class Application
{
public:
	Application(int tick, ApplicationType type, int effectiveness)
		:
		tick(tick),
		type(type),
		effectiveness(effectiveness)
	{}
	int GetTick() { return tick; }
	ApplicationType GetApplicationType() { return type; }
	int GetEffectiveness() { return effectiveness; }
private:
	int tick;
	ApplicationType type;
	int effectiveness; //damage
};

class Action
{
	//Action() = default;
	Action(std::vector<Application> ApplicationVector, int maxTicks)
		:
		ApplicationVector(ApplicationVector),
		maxTicks(maxTicks)
	{

	}

	int GetMaxTicks() { return maxTicks; }

	std::vector<Application> GetApplicationByTick(int tick)
	{
		std::vector<Application> temp;
		for (auto& apply : ApplicationVector)
		{
			if (apply.GetTick() == tick)
			{
				temp.emplace_back(apply);
			}
		}
		return temp;
	}

private:
	int maxTicks;
	std::vector<Application> ApplicationVector;
};

static Action Idle;
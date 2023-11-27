#pragma once
#include <unordered_map>

enum class Stat
{
	Charisma,     //social ability
	Constitution, //special resistance, hitpoints, mental & physical fortitude, endurance
	Strength,	  //physical strength
	Dexterity,	  //quickness, sleight of hand, agility. Ability to complete complicated tasks and avoid 
	Intelligence, //knowledge, aptitude
	Wisdom	  //environmental awareness
};

class Attributes
{
public:
	Attributes()
	{
		//default values
		stats[Stat::Charisma] = 10;
		stats[Stat::Constitution] = 10;
		stats[Stat::Strength] = 10;
		stats[Stat::Dexterity] = 10;
		stats[Stat::Intelligence] = 10;
		stats[Stat::Wisdom] = 10;
	}
	Attributes(int cha, int con, int str, int dex, int intel, int wis)
	{
		stats[Stat::Charisma] = cha;
		stats[Stat::Constitution] = con;
		stats[Stat::Strength] = str;
		stats[Stat::Dexterity] = dex;
		stats[Stat::Intelligence] = intel;
		stats[Stat::Wisdom] = wis;
	}

	int getStat(Stat statName) const {
		auto it = stats.find(statName);
		if (it != stats.end()) {
			return it->second;
		}
		// Return a default value (you can customize this)
		return 0;
	}

	void setStat(Stat statName, int value) {
		stats[statName] = value;
	}
public:
	std::unordered_map<Stat, int> stats;

};
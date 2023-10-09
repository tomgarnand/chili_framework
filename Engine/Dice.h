#pragma once
#include <random>

enum class Outcome : int
{
	CriticalHit = 2,
	CriticalMiss = 0,
	Hit = 1,
	Miss = 0,
	NotApplicable = 0
};

class Dice
{
private:
	static std::random_device rd;       // Random device
	static std::mt19937 gen;            // Mersenne twister engine
	std::uniform_int_distribution<> dis;// Uniform distribution

public:
	Dice(int min, int max) : dis(min, max) {}

	int roll() {
		return dis(gen);
	}
}; 



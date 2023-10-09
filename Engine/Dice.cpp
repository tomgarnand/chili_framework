#include "Dice.h"

// Static member variable definitions
std::random_device Dice::rd;
std::mt19937 Dice::gen(Dice::rd());

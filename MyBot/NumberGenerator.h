#pragma once
#pragma once
#include <random>
#include <array>
#include <string>
#include <sstream>
#include "Log.h"
 

class NumberGenerator
{
public:
	NumberGenerator();
	std::vector<int> generateNumbers(int number, int sides);

	std::string rollDice(const std::string& input);

	int num_dice(const std::string& input);

private:
	std::mt19937 mt;
	const int MAX_DICE = 750;
	const int MAX_SIDES = 1000;

};
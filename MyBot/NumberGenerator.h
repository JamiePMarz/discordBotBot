#pragma once
#pragma once
#include <random>
#include <array>


class NumberGenerator
{
public:
	NumberGenerator();
	std::vector<int> generateNumbers(int number, int sides);

private:
	std::mt19937 mt;

};
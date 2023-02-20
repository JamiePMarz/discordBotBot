#include "NumberGenerator.h"


NumberGenerator::NumberGenerator()
{
	std::array<int, 624> seed_data;
	std::random_device r_device;
	std::generate_n(seed_data.data(), seed_data.size(), std::ref(r_device));
	std::seed_seq seq(std::begin(seed_data), std::end(seed_data));

	mt.seed(seq);

}

std::vector<int> NumberGenerator::generateNumbers(int number, int sides)
{
	std::uniform_int_distribution<int> range = std::uniform_int_distribution<int>(1, sides);
	std::vector<int> rolls;
	for (int i = 0; i < number; i++) {
		rolls.push_back(range(mt));
	}
	return rolls;
}
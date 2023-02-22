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



std::string NumberGenerator::rollDice(const std::string& input)
{
	int dice, sides = 0;

	std::string sub;
	std::istringstream input_ss(input);
	std::getline(input_ss, sub, 'd');
	//LOG("sub: " << sub);
	if (sub.size() > 7) return  { "invalid" };

	//dice
	dice = num_dice(sub);
	if (!dice || dice > 2000000) return { "invalid" };

	//sides
	std::getline(input_ss, sub);
	//LOG("sub: " << sub);
	if (!sub.size() || sub.size() > 9) return  { "invalid" };
	size_t pos = 0;
	while (pos < sub.size()) {
		if (sub[pos] == 'd' || sub[pos] == '-' || sub[pos] == '+') {
			if (!sides)  return { "invalid" };
			break;
		}
		if (!std::isdigit(sub[pos])) return { "invalid" };

		sides *= 10;
		sides += sub[pos] - '0';
		pos++;
	}
	//LOG("sides: " << sides);
	if (sides < 0 || dice * sides < 0) return { "invalid" };

	//drop
	sub = sub.substr(pos, sub.size() - pos);
	//LOG("sub: " << sub);
	pos = 0;
	int drop_num = 0;
	if (sub[pos] == 'd') {
		pos++;
		while (pos < sub.size()) {
			if (sub[pos] == '-' || sub[pos] == '+') {
				if (!drop_num)  return { "invalid" };
				break;
			}
			if (!std::isdigit(sub[pos])) return { "invalid" };

			drop_num *= 10;
			drop_num += sub[pos] - '0';
			pos++;
		}
		//LOG("drop_num: " << drop_num);
		if (!drop_num || drop_num >= dice) return { "invalid" };
	}

	//mod
	sub = sub.substr(pos, sub.size() - pos);
	//LOG("sub: " << sub);
	pos = 0;
	int mod = 0, mod_sign;
	if (sub[pos] == '-' || sub[pos] == '+') {
		if (sub[pos] == '-') mod_sign = -1;
		else mod_sign = 1;
		pos++;
		while (pos < sub.size()) {
			if (!std::isdigit(sub[pos])) return { "invalid" };
			mod *= 10;
			mod += sub[pos] - '0';
			pos++;
		}
		mod *= mod_sign;
		//LOG("mod: " << mod);
	}


	//return
	std::vector<int> rolls = generateNumbers(dice, sides);
	std::string rString = "{";
	int result = 0;
	for (const int& i : rolls) {
		rString += " " + std::to_string(i);
		result += i;
	}
	rString += " }";

	if (drop_num) {
		std::vector<int> temp = rolls;
		std::sort(temp.begin(), temp.end());
		for (int i = 0; i < drop_num; i++)
			result -= temp[i];
	}

	if (mod) {
		result += mod;
		if (mod < 0)
			rString += " " + std::to_string(mod);
		else
			rString += " +" + std::to_string(mod);
	}

	rString += "   Score: " + std::to_string(result);
	LOG("score: " << result);
	if (rString.size() > 1800)
		return { "Score: " + std::to_string(result) };
	return rString;
}


int NumberGenerator::num_dice(const std::string& input) {
	if (!input.size()) return 1;
	//LOG("dice input: " << input);
	for (const char& c : input)
		if (!std::isdigit(c)) return 0;

	int dice = std::stoi(input);
	//LOG("dice = " << dice);
	return dice;
}

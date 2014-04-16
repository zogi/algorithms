#include <iostream>

#include "min_cost_max_flow.h"

using mcmf::matrix;
using mcmf::min_cost_max_match;

int main()
{
	int na = 3, nb = 2;
	auto match_possible = [](int,int) { return true; };
	auto cost = matrix<double>(na, nb);
	cost(0, 0) = 11.123;
	cost(0, 1) = 8.7;
	cost(1, 0) = 104.7;
	cost(1, 1) = 6.42;;
	cost(2, 0) = 65.93;
	cost(2, 1) = 2.42;
	std::vector<int> matching;
	min_cost_max_match(na, nb, match_possible, cost, matching);
	for (int i = 0; i < na; ++i) {
		std::cout << matching[i] << std::endl;
	}
	return 0;
}

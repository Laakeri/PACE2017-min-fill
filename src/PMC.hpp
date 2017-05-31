#ifndef MINFILL_PMC_HPP
#define MINFILL_PMC_HPP

#include "graph.hpp"

#include <vector>
#include <cstdint>

namespace PMC {
	std::vector<std::pair<int, int> > solve(const Graph& G, int lowerBound, int upperBound, int PROBLEM);
	bool hasMoreMinSeps(const Graph& G, int64_t limit);
	void setPrintProcess(bool printProcess);
};

#endif
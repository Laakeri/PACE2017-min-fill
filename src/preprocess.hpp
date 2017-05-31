#ifndef MINFILL_PREPROCESS_HPP
#define MINFILL_PREPROCESS_HPP

#include "graph.hpp"

#include <ostream>

namespace Preprocess {
	void addG(const Graph& G);
	void addVal(int a);
	void print(std::ostream& out);
}

#endif
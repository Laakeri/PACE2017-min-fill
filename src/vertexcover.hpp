#ifndef VC_VERTEXCOVER_HPP
#define VC_VERTEXCOVER_HPP

#include "graph.hpp"

#include <vector>

namespace VertexCover {
	std::vector<int> exactSol(const Graph& G);
	double getTimeUsed();
}

#endif
#ifndef MINFILL_MCS_HPP
#define MINFILL_MCS_HPP

#include "graph.hpp"

#include <vector>

namespace MCS {
	// returns minimal triangulation and the elimination order
	// O(n*m), also O(n*m) in practice
	std::pair<std::vector<std::pair<int, int> >, std::vector<std::pair<int, int> > > MCS_MP(const Graph& G);
	// returns the atoms of the graph, given the output of MCP_MP
	// O(n*m)
	std::pair<int, std::vector<Graph> > getAtoms(const Graph& G, const Graph& gFill, const std::pair<std::vector<std::pair<int, int> >, std::vector<std::pair<int, int> > >& fill);
	std::vector<int> MCS_L(const Graph& G, int sv, const std::vector<int>& rmV);
	std::vector<int> getChordlessCycle(const Graph& G, int sv, std::vector<int>& rmV);
	std::vector<int> getChordlessCycle(const Graph& G, int sv = 0);
	int getTreeWidth(const Graph& G);
	int maxCliqueSeparator(const Graph& G, const Graph& gFill, const std::pair<std::vector<std::pair<int, int> >, std::vector<std::pair<int, int> > >& fill);
};

#endif
#include "graph.hpp"
#include "preprocess.hpp"

#include <vector>
#include <cassert>

#define F first
#define S second

using namespace std;

namespace Preprocess {

int val = 0;
int n = 0;
vector<pair<int, int> > outE;
void addG(const Graph& G) {
	vector<pair<int, int> > es;
	for (int i = 0; i < G.n; i++) {
		for (int nx : G.g[i]) {
			if (nx > i) {
				es.push_back({i, nx});
			}
		}
	}
	Graph GG(es);
	for (int i = 0; i < GG.n; i++) {
		for (int nx : GG.g[i]) {
			if (nx > i) outE.push_back({n + i, n + nx});
		}
	}
	n += GG.n;
}
void addVal(int a) {
	if (a == 0) return;
	assert(a > 0);
	outE.push_back({n + a + 2, n}); 
	for (int i = 1; i < a+3; i++) {
		outE.push_back({n + i - 1, n + i});
	}
	n += a+3;
}
void print(ostream& out) {
	for (auto e : outE) {
		out<<e.F<<" "<<e.S<<'\n';
	}
	out.flush();
}

}
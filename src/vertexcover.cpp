#include "vertexcover.hpp"
#include "graph.hpp"
#include "timer.hpp"
#include "global.hpp"

#include <vector>
#include <queue>
#include <cassert>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <map>

#define F first
#define S second

using namespace std;

namespace VertexCover {

int uu[maxN];
int u[maxN];
Timer VCtimer;

int m1[maxN];
int m2[maxN];
int layer[2*maxN];
int bfs[2*maxN];
int z[2*maxN];
int v[2*maxN];

int dQ[maxN];

void findZ(const Graph& G, int x) {
	if (z[x]) return;
	z[x] = 1;
	assert(x%2 == 0);
	for (int nx : G.g[x/2]) {
		if (nx != m1[x/2]) {
			z[nx*2+1] = 1;
			if (m2[nx] != -1) findZ(G, m2[nx]*2);
		}
	}
}
int hkdfs(const Graph& G, int x) {
	int la = layer[x];
	assert(la > 0);
	layer[x] = 0;
	assert(la%2 != x%2);
	if (x%2 == 1) {
		for (int nx : G.g[x/2]) {
			if (nx != m2[x/2] && layer[nx*2] == la-1) {
				int ff = hkdfs(G, nx*2);
				if (ff) {
					m2[x/2] = nx;
					m1[nx] = x/2;
					return 1;
				}
			}
		}
		return 0;
	}
	else {
		if (la == 1) {
			return 1;
		}
		if (m1[x/2] != -1 && layer[m1[x/2]*2+1] == la-1) {
			int ff = hkdfs(G, m1[x/2]*2+1);
			if (ff) return 1;
		}
		return 0;
	}
}
pair<int, vector<int> > LPVC(const Graph& G) {
	fill(m1, m1 + G.n, -1);
	fill(m2, m2 + G.n, -1);
	int iter = 0;
	while (1) {
		iter++;
		fill(layer, layer + G.n*2, 0);
		int bfsi1 = 0;
		int bfsi2 = 0;
		for (int i = 0; i < G.n; i++) {
			if (m1[i] == -1 && G.g[i].size() > 0) {
				layer[i*2] = 1;
				bfs[bfsi2++] = i*2;
			}
		}
		int f = 0;
		while (bfsi1 < bfsi2) {
			int x = bfs[bfsi1++];
			if (x%2 == 1) {
				if (m2[x/2] == -1) {
					f = layer[x];
					break;
				}
				else {
					if (layer[m2[x/2]*2] == 0) {
						layer[m2[x/2]*2] = layer[x]+1;
						bfs[bfsi2++] = m2[x/2]*2;
					}
				}
			}
			else {
				for (int nx : G.g[x/2]) {
					if (nx != m1[x/2] && layer[nx*2+1] == 0) {
						layer[nx*2+1] = layer[x]+1;
						bfs[bfsi2++] = nx*2+1;
					}
				}
			}
		}
		if (f == 0) break;
		assert(f>1);
		assert(f%2 == 0);
		int fo = 0;
		for (int i = 0; i < G.n; i++) {
			if (layer[i*2+1] == f && m2[i] == -1) {
				fo += hkdfs(G, i*2+1);
			}
		}
		assert(fo > 0);
	}
	assert(iter*iter <= 16*G.n);
	int val = 0;
	for (int i = 0; i < G.n; i++) {
		if (m1[i] != -1) val++;
	}
	fill(z, z + G.n*2, 0);
	for (int i = 0; i < G.n; i++) {
		if (m1[i] == -1) {
			findZ(G, i*2);
		}
	}
	fill(v, v + G.n*2, 0);
	for (int i = 0; i < 2*G.n; i++) {
		if (i%2 == 0) {
			if (!z[i]) {
				v[i] = 1;
			}
		}
		else {
			if (z[i]) {
				v[i] = 1;
			}
		}
	}
	int cval = 0;
	for (int i = 0; i < 2*G.n; i++) {
		cval += v[i];
		if (v[i] == 0) {
			for (int nx : G.g[i/2]) {
				if (i%2 == 0) {
					assert(v[nx*2+1] == 1);
				}
				else {
					assert(v[nx*2] == 1);
				}
			}
		}
	}
	assert(cval == val);
	vector<int> ret;
	for (int i = 0; i < G.n; i++) {
		if (v[i*2] && v[i*2+1]) ret.push_back(i);
	}
	return {val, ret};
}
int lowerBound2A(Graph G) {
	int bound = 0;
	queue<int> d1;
	for (int i = 0; i < G.n; i++) {
		assert(G.g[i].size() > 0);
		if (G.g[i].size() == 1) d1.push(i);
	}
	for (int i = 0; i < G.n; i++) {
		while (!d1.empty()) {
			int x = d1.front();
			d1.pop();
			if (G.g[x].size() == 0) continue;
			assert(G.g[x].size() == 1);
			int t = G.g[x][0];
			bound++;
			for (int nx : G.g[t]) {
				if (G.g[nx].size() == 2) {
					d1.push(nx);
				}
			}
			G.remIncident(t);
			assert(G.g[x].size() == 0);
			assert(G.g[t].size() == 0);
		}
		if (G.g[i].size() == 0) continue;
		assert(G.g[i].size() > 1);
		int t = G.g[i][0];
		for (int nx : G.g[i]) {
			if (G.g[nx].size() < G.g[t].size()) {
				t = nx;
			}
		}
		for (int nx : G.g[i]) {
			if (nx != t && G.g[nx].size() == 2) {
				d1.push(nx);
			}
			else if (nx != t && G.g[nx].size() == 3 && G.hasEdge(t, nx)) {
				d1.push(nx);
			}
		}
		for (int nx : G.g[t]) {
			if (nx != i && G.g[nx].size() == 2) {
				d1.push(nx);
			}
		}
		bound++;
		G.remIncident(t);
		G.remIncident(i);
		assert(G.g[t].size() == 0);
		assert(G.g[i].size() == 0);
	}
	return bound;
}
int dfs(const Graph& G, int x) {
	if (uu[x]) return 0;
	uu[x] = 1;
	int r = 1;
	for (int nx : G.g[x]) {
		r += dfs(G, nx);
	}
	return r;
}
void getCC(const Graph& G, int x, vector<pair<int, int> >& es) {
	if (u[x]) return;
	u[x] = 1;
	for (int nx : G.g[x]) {
		if (nx > x) es.push_back({x, nx});
		getCC(G, nx, es);
	}
}
// find the best solution that is at most upperBound or report that it does not exist
bool optSolve(Graph& G, int upperBound, vector<int>& sol) {
	if (upperBound < 0) return false;
	if (upperBound == 0 && G.m > 0) return false;
	if (G.m == 0) return true;
	assert(upperBound > 0);
	int dQ1 = 0;
	int dQ2 = 0;
	for (int i = 0; i < G.n; i++) {
		if (G.g[i].size() == 1) dQ[dQ2++] = G.g[i][0];
		if ((int)G.g[i].size() > upperBound) dQ[dQ2++] = i;
	}
	bool fr = true;
	vector<pair<int, int> > remd;
	while (dQ1 < dQ2 || fr) {
		fr = false;
		while (dQ1 < dQ2) {
			while (dQ1 < dQ2) {
				int x = dQ[dQ1++];
				if (G.g[x].size() == 0) continue;
				if (upperBound == 0) {
					G.addEdges(remd);
					return false;
				}
				sol.push_back(x);
				G.remIncident(x, remd);
				assert(G.g[x].size() == 0);
				upperBound--;
			}
			for (int i = 0; i < G.n; i++) {
				if (G.g[i].size() == 1) dQ[dQ2++] = G.g[i][0];
				if ((int)G.g[i].size() > upperBound) dQ[dQ2++] = i;
			}
		}
		auto LPV = LPVC(G);
		if (LPV.F > 2*upperBound) {
			G.addEdges(remd);
			return false;
		}
		assert((int)LPV.S.size() <= upperBound);
		for (int x : LPV.S) {
			sol.push_back(x);
			assert(G.g[x].size() > 0);
			G.remIncident(x, remd);
			assert(G.g[x].size() == 0);
			upperBound--;
		}
		assert(upperBound >= 0);
		for (int i = 0; i < G.n; i++) {
			if (G.g[i].size() == 1) dQ[dQ2++] = G.g[i][0];
			if ((int)G.g[i].size() > upperBound) dQ[dQ2++] = i;
		}
	}
	if (G.m == 0) {
		G.addEdges(remd);
		return true;
	}
	if (upperBound == 0) {
		G.addEdges(remd);
		return false;
	}
	assert(upperBound > 0);
	int maxD = 0;
	int maxDx = -1;
	int fo = 0;
	for (int i = 0; i < G.n; i++) {
		assert(G.g[i].size() != 1);
		if ((int)G.g[i].size() > maxD) {
			maxD = G.g[i].size();
			maxDx = i;
		}
		if ((int)G.g[i].size() > 0) fo++;
	}
	assert(maxD <= upperBound);
	assert(maxD > 0);
	if (maxD == 2) {
		fill(uu, uu + G.n, 0);
		for (int i = 0; i < G.n; i++) {
			if (uu[i] == 0 && G.g[i].size() > 0) {
				int sz = dfs(G, i);
				assert(sz >= 3);
				upperBound -= (sz+1)/2;
				if (upperBound < 0) {
					G.addEdges(remd);
					return false;
				}
			}
		}
		for (int i = 0; i < G.n; i++) {
			if (uu[i] == 1) {
				assert(G.g[i].size() == 2);
				int sz = 0;
				int x = i;
				while (uu[x] == 1) {
					sz++;
					if (sz%2 == 1) {
						sol.push_back(x);
					}
					uu[x] = 2;
					for (int nx : G.g[x]) {
						if (uu[nx] == 1) {
							x = nx;
							break;
						}
					}
				}
			}
		}
		G.addEdges(remd);
		return true;
	}
	assert(maxD >= 3);
	vector<int> nb = G.g[maxDx];
	
	vector<pair<int, int> > co1;
	memset(u, 0, sizeof(int)*G.n);
	for (int i = 0; i < G.n; i++) {
		if (G.g[i].size() > 0) {
			getCC(G, i, co1);
			assert(co1.size() > 0);
			break;
		}
	}
	assert((int)co1.size() <= G.m);
	if ((int)co1.size() < G.m || G.m*2 < G.n) {
		vector<vector<pair<int, int> > > cos = {co1};
		for (int i = 0; i < G.n; i++) {
			if (G.g[i].size() > 0 && !u[i]) {
				cos.push_back(vector<pair<int, int> >());
				getCC(G, i, cos.back());
				assert(cos.back().size() > 0);
			}
		}
		G.addEdges(remd);
		vector<int> soltt;
		for (auto& co : cos) {
			Graph CC(co);
			vector<int> solt;
			bool s = optSolve(CC, upperBound - (int)soltt.size(), solt);
			if (!s) return false;
			solt = CC.mapBack(solt);
			soltt.insert(soltt.end(), solt.begin(), solt.end());
		}
		sol.insert(sol.end(), soltt.begin(), soltt.end());
		assert((int)soltt.size() <= upperBound);
		return true;
	}
	
	vector<int> sol1;
	G.remIncident(maxDx, remd);
	sol1.push_back(maxDx);
	bool s1 = optSolve(G, upperBound - (int)sol1.size(), sol1);
	if (s1) {
		assert((int)sol1.size() <= upperBound);
		upperBound = (int)sol1.size()-1;
	}
	
	vector<int> sol2;
	for (int x : nb) {
		sol2.push_back(x);
		G.remIncident(x, remd);
	}
	bool s2 = optSolve(G, upperBound - (int)sol2.size(), sol2);
	G.addEdges(remd);
	if (s2) {
		if (s1) assert(sol2.size() < sol1.size());
		sol.insert(sol.end(), sol2.begin(), sol2.end());
		return true;
	}
	else if (s1) {
		sol.insert(sol.end(), sol1.begin(), sol1.end());
		return true;
	}
	else{
		return false;
	}
}
vector<int> exactSol(const Graph& G) {
	if (G.m == 0) return {};
	VCtimer.start();
	vector<int> solv;
	Graph GG = G;
	bool ok = optSolve(GG, G.n-1, solv);
	assert(ok);
	VCtimer.stop();
	return solv;
}
double getTimeUsed() {
	return VCtimer.getTime().count();
}

}
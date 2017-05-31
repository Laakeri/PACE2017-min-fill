#include "PMC.hpp"
#include "MCS.hpp"
#include "global.hpp"

#include <cassert>
#include <vector>
#include <algorithm>
#include <set>
#include <iostream>
#include <map>
#include <queue>

#define F first
#define S second

using namespace std;

namespace PMC {

int u[maxN];
int u2[maxN];
int it;

int inSep[maxN];

bool printProcess = false;

void setPrintProcess(bool pp) {
	printProcess = pp;
}

int dfs1(const Graph& G, int x) {
	assert(u[x] == 0);
	u[x] = 1;
	int r = 0;
	for (int nx : G.g[x]) {
		if (u[nx] == 0) r += dfs1(G, nx);
		else if (u[nx] > 1 && u[nx] < it) {
			u[nx] = it;
			r++;
		}
	}
	return r;
}

bool hasFullComponent(const Graph& G, const vector<int>& separator) {
	for (int i = 0; i < G.n; i++) {
		u[i] = 0;
	}
	it = 2;
	for (int x : separator) {
		assert(u[x] == 0);
		u[x] = it;
	}
	for (int i = 0; i < G.n; i++) {
		if (u[i] == 0) {
			it++;
			int c = dfs1(G, i);
			assert(c <= (int)separator.size());
			if (c == (int)separator.size()) {
				return true;
			}
		}
	}
	return false;
}

void dfs2(const Graph& G, int x) {
	assert(u[x] == 0);
	u[x] = 1;
	for (int nx : G.g[x]) {
		if (u[nx] == 0) dfs2(G, nx);
		else if (u[nx] == 2) {
			u[nx] = 3;
		}
	}
}

bool extend2(const Graph& G, const vector<int>& pmc, int a) {
	assert(a < G.n);
	for (int i = 0; i < G.n; i++) {
		u[i] = 0;
	}
	for (int x : pmc) {
		u[x] = 2;
	}
	dfs2(G, a);
	for (int x : pmc) {
		if (u[x] != 3) return false;
	}
	return true;
}

bool isPMCbrute(const Graph& G, const vector<int>& pmc) {
	for (int x : pmc) {
		assert(x >= 0 && x < G.n);
	}
	if (hasFullComponent(G, pmc)) return false;
	for (int x : pmc) {
		for (int i = 0; i < G.n; i++) {
			u[i] = 0;
		}
		for (int y : pmc) {
			if (y != x) {
				u[y] = 2;
			}
		}
		dfs2(G, x);
		for (int y : pmc) {
			if (y != x) {
				if (u[y] != 3) return false;
			}
		}
	}
	return true;
}
int dfs3(const Graph& G, int x, vector<int>& co) {
	assert(u[x] == 0);
	u[x] = 1;
	co.push_back(x);
	int r = 0;
	for (int nx : G.g[x]) {
		if (u[nx] == 0) r += dfs3(G, nx, co);
		else if (u[nx] > 1 && u[nx] < it) {
			u[nx] = it;
			r++;
		}
	}
	return r;
}
vector<vector<int> > getFullComponents(const Graph& G, const vector<int>& separator) {
	for (int i = 0; i < G.n; i++) {
		u[i] = 0;
	}
	it = 2;
	for (int x : separator) {
		assert(u[x] == 0);
		u[x] = it;
	}
	vector<vector<int> > ret;
	for (int i = 0; i < G.n; i++) {
		if (u[i] == 0) {
			it++;
			vector<int> co;
			int c = dfs3(G, i, co);
			assert(c <= (int)separator.size());
			if (c == (int)separator.size()) {
				sort(co.begin(), co.end());
				ret.push_back(co);
			}
		}
	}
	return ret;
}
vector<vector<int> > getComponents(const Graph& G, const vector<int>& separator) {
	for (int i = 0; i < G.n; i++) {
		u[i] = 0;
	}
	it = 2;
	for (int x : separator) {
		assert(u[x] == 0);
		u[x] = it;
	}
	vector<vector<int> > ret;
	for (int i = 0; i < G.n; i++) {
		if (u[i] == 0) {
			it++;
			vector<int> co;
			int c = dfs3(G, i, co);
			assert(c <= (int)separator.size());
			sort(co.begin(), co.end());
			ret.push_back(co);
		}
	}
	return ret;
}

bool canFill(const Graph& G, const vector<int>& pmc, int upperBound, int PROBLEM) {
	if (PROBLEM == PROBLEM_MINFILL) {
		if ((int64_t)pmc.size()*((int64_t)pmc.size()-1ll)/2ll - (int64_t)G.m > (int64_t)upperBound) return false;
		for (int i = 0; i < (int)pmc.size(); i++) {
			for (int ii = i+1; ii < (int)pmc.size(); ii++) {
				if (!G.hasEdge(pmc[i], pmc[ii])) {
					upperBound--;
					if (upperBound < 0) return false;
				}
			}
		}
		return true;
	}
	else {
		abort();
	}
}

// suppose that oldMinSeps and minSeps are ordered
vector<vector<int> > ONE_MORE_VERTEX(const Graph& G, const vector<vector<int> >& oldPMCs, const vector<vector<int> >& oldMinSeps, const vector<vector<int> >& minSeps, int a, int upperBound, int PROBLEM) {
	vector<vector<int> > PMCs;
	for (const auto& pmc : oldPMCs) {
		if (!hasFullComponent(G, pmc)) {
			PMCs.push_back(pmc);
		}
		else {
			if (extend2(G, pmc, a)) {
				PMCs.push_back(pmc);
				PMCs.back().push_back(a);
				if (!canFill(G, PMCs.back(), upperBound, PROBLEM)) {
					PMCs.pop_back();
				}
			}
		}
	}
	int i2 = 0;
	for (const auto& minSep : minSeps) {
		if (find(minSep.begin(), minSep.end(), a) != minSep.end()) continue;
		if (!canFill(G, minSep, upperBound, PROBLEM)) continue;
		if (extend2(G, minSep, a)) {
			PMCs.push_back(minSep);
			PMCs.back().push_back(a);
			if (hasFullComponent(G, PMCs.back())) {
				PMCs.pop_back();
			}
			else if (!canFill(G, PMCs.back(), upperBound, PROBLEM)) {
				PMCs.pop_back();
			}
		}
		while (i2 < (int)oldMinSeps.size() && oldMinSeps[i2] < minSep) i2++;
		if (i2 == (int)oldMinSeps.size() || oldMinSeps[i2] != minSep) {
			vector<vector<int> > components = getComponents(G, minSep);
			assert(components.size() >= 2);
			set<vector<int> > tried;
			for (const auto& T : minSeps) {
				for (const auto& C : components) {
					vector<int> npmc = minSep;
					bool fo = false;
					for (int x : T) {
						if (binary_search(C.begin(), C.end(), x)) {
							if (!binary_search(minSep.begin(), minSep.end(), x)) {
								npmc.push_back(x);
								fo = true;
							}
						}
					}
					if (fo) {
						if (tried.count(npmc)) continue;
						tried.insert(npmc);
						if (canFill(G, npmc, upperBound, PROBLEM) && isPMCbrute(G, npmc)) {
							PMCs.push_back(npmc);
						}
					}
				}
			}
		}
	}
	return PMCs;
}

void dfs4(const Graph& G, int x, vector<int>& nb) {
	assert(u[x] == 0);
	u[x] = 1;
	for (int nx : G.g[x]) {
		if (u[nx] == 0) dfs4(G, nx, nb);
		else if (u[nx] == 2) {
			nb.push_back(nx);
			u[nx] = 3;
		}
	}
}

vector<vector<int> > findMinSeps(const Graph& G, int64_t UBcount = -1) {
	vector<vector<int> > minSeps;
	set<vector<int> > ff;
	for (int i = 0; i < G.n; i++) {
		if (G.g[i].size() == 0) continue;
		for (int j = 0; j < G.n; j++) {
			u[j] = 0;
		}
		for (int nx : G.g[i]) {
			u[nx] = 2;
		}
		u[i] = 2;
		for (int nx : G.g[i]) {
			for (int nnx : G.g[nx]) {
				if (u[nnx] == 0) {
					vector<int> nb;
					dfs4(G, nnx, nb);
					assert(nb.size() > 0);
					for (int a : nb) {
						u[a] = 2;
					}
					sort(nb.begin(), nb.end());
					if (!ff.count(nb)) {
						ff.insert(nb);
						minSeps.push_back(nb);
						if (UBcount != -1 && (int64_t)minSeps.size() >= UBcount) {
							sort(minSeps.begin(), minSeps.end());
							return minSeps;
						}
					}
				}
			}
		}
	}
	for (int i = 0; i < (int)minSeps.size(); i++) {
		auto S = minSeps[i];
		for (int x : S) {
			for (int j = 0; j < G.n; j++) {
				u[j] = 0;
			}
			for (int y : S) {
				u[y] = 2;
			}
			for (int nx : G.g[x]) {
				u[nx] = 2;
			}
			for (int nx : G.g[x]) {
				for (int nnx : G.g[nx]) {
					if (u[nnx] == 0) {
						vector<int> nb;
						dfs4(G, nnx, nb);
						assert(nb.size() > 0);
						for (int a : nb) {
							u[a] = 2;
						}
						sort(nb.begin(), nb.end());
						if (!ff.count(nb)) {
							ff.insert(nb);
							minSeps.push_back(nb);
							if (UBcount != -1 && (int64_t)minSeps.size() >= UBcount) {
								sort(minSeps.begin(), minSeps.end());
								return minSeps;
							}
						}
					}
				}
			}
		}
	}
	sort(minSeps.begin(), minSeps.end());
	return minSeps;
}

bool hasMoreMinSeps(const Graph& G, int64_t limit) {
	auto ms = findMinSeps(G, limit+1);
	return (int64_t)ms.size() > limit;
}

bool isMinSep(const Graph& G, const vector<int>& separator) {
	auto cc = getFullComponents(G, separator);
	return cc.size() > 1;
}

vector<vector<int> > findPMCs(const Graph& G, int upperBound, int PROBLEM) {
	assert(G.isConnected());
	vector<int> rmV(G.n);
	vector<int> order = MCS::MCS_L(G, 0, rmV);
	reverse(order.begin(), order.end());
	vector<int> ord(G.n);
	assert((int)order.size() == G.n);
	for (int i = 0; i < G.n; i++) {
		ord[order[i]] = i;
	}
	Graph nG(G.n, true);
	vector<vector<int> > PMCs = {{order[0]}};
	vector<vector<int> > minSeps = {};
	if (VERB >= 1 || printProcess) {
		cerr<<"Computing potential maximal cliques..."<<endl;
	}
	for (int i = 1; i < G.n; i++) {
		int x = order[i];
		for (int nx : G.g[x]) {
			if (ord[nx] < ord[x]) {
				nG.addEdge(nx, x);
			}
		}
		assert(nG.isConnectedOrIsolated());
		auto newMinSeps = findMinSeps(nG);
		for (auto& s : newMinSeps) {
			if (!isMinSep(nG, s)) {
				cerr<<"FAIL sep"<<endl;
				for (int xx : s) {
					cerr<<xx<<endl;
				}
				nG.print(cerr);
			}
			assert(isMinSep(nG, s));
		}
		PMCs = ONE_MORE_VERTEX(nG, PMCs, minSeps, newMinSeps, x, upperBound, PROBLEM);
		minSeps = newMinSeps;
		for (auto& pmc : PMCs) {
			assert(canFill(G, pmc, upperBound, PROBLEM));
			sort(pmc.begin(), pmc.end());
		}
		sort(PMCs.begin(), PMCs.end());
		PMCs.erase(unique(PMCs.begin(), PMCs.end()), PMCs.end());
		if (VERB >= 1 || printProcess) {
			cerr<<"Added "<<i+1<<" of "<<G.n<<" vertices. PMCs: "<<PMCs.size()<<" minseps: "<<minSeps.size()<<endl;
		}
	}
	if (VERB >= 1 || printProcess) {
		cerr<<"Potential maximal cliques computed"<<endl;
	}
	return PMCs;
}

int dfs5(const Graph& G, int x, vector<int>& co) {
	assert(u2[x] == 0);
	assert(u[x] != 3);
	u2[x] = 1;
	co.push_back(x);
	int r = 0;
	if (u[x] == 2) r++;
	for (int nx : G.g[x]) {
		if (!u2[nx] && u[nx] != 3) {
			r += dfs5(G, nx, co);
		}
	}
	return r;
}

void dfs6(const Graph& G, int x, vector<int>& co, vector<int>& nb) {
	assert(u[x] == 0);
	u[x] = 1;
	co.push_back(x);
	for (int nx : G.g[x]) {
		if (u[nx] == 0) dfs6(G, nx, co, nb);
		if (u[nx] >= 2 && u[nx] < it) {
			nb.push_back(nx);
			u[nx] = it;
		}
	}
}

vector<pair<int, int> > solve(const Graph& G, int lowerBound, int upperBound, int PROBLEM) {
	assert(PROBLEM == PROBLEM_MINFILL);
	if (lowerBound > upperBound) return FAILSOLUTION;
	vector<vector<int> > PMCs = findPMCs(G, upperBound, PROBLEM);
	map<vector<int>, int> sepsMap, ccsMap;
	vector<vector<int> > seps, ccs;
	vector<pair<pair<int, int>, int> > niceTrp;
	assert(G.isConnected());
	for (int i = 0; i < (int)PMCs.size(); i++) {
		for (int j = 0; j < G.n; j++) {
			u[j] = 0;
		}
		for (int x : PMCs[i]) {
			u[x] = 2;
		}
		for (int ii = 0; ii < G.n; ii++) {
			if (u[ii] == 0) {
				vector<int> nb;
				dfs4(G, ii, nb);
				assert(nb.size() > 0 && nb.size() < PMCs[i].size());
				for (int j = 0; j < G.n; j++) {
					u2[j] = 0;
				}
				int ff = 0;
				vector<int> co;
				for (int x : PMCs[i]) {
					if (u[x] == 2) {
						ff = dfs5(G, x, co);
						break;
					}
				}
				assert(ff > 0 && ff + (int)nb.size() == (int)PMCs[i].size());
				for (int a : nb) {
					u[a] = 2;
				}
				sort(nb.begin(), nb.end());
				if (!sepsMap.count(nb)) {
					sepsMap[nb] = seps.size();
					seps.push_back(nb);
				}
				sort(co.begin(), co.end());
				if (!ccsMap.count(co)) {
					ccsMap[co] = ccs.size();
					ccs.push_back(co);
				}
				niceTrp.push_back({{sepsMap[nb], ccsMap[co]}, i});
			}
		}
	}
	vector<int> all;
	for (int i = 0; i < G.n; i++) all.push_back(i);
	assert(!sepsMap.count({}));
	assert(!ccsMap.count(all));
	sepsMap[{}] = seps.size();
	ccsMap[all] = ccs.size();
	seps.push_back({});
	ccs.push_back(all);
	pair<int, int> rootState = {(int)seps.size()-1, (int)ccs.size()-1};
	for (int i = 0; i < (int)PMCs.size(); i++) {
		niceTrp.push_back({rootState, i});
	}
	auto cmp = [&](pair<pair<int, int>, int> a, pair<pair<int, int>, int> b) {
		if (seps[a.F.F].size() + ccs[a.F.S].size() != seps[b.F.F].size() + ccs[b.F.S].size()) {
			return seps[a.F.F].size() + ccs[a.F.S].size() < seps[b.F.F].size() + ccs[b.F.S].size();
		}
		else {
			return a<b;
		}
	};
	sort(niceTrp.begin(), niceTrp.end(), cmp);
	map<pair<int, int>, pair<int, int> > dp;
	for (auto trp : niceTrp) {
		assert(trp.F.F < (int)seps.size());
		assert(trp.F.S < (int)ccs.size());
		vector<int> sep = seps[trp.F.F];
		vector<int> co = ccs[trp.F.S];
		vector<int> uni = sep;
		uni.insert(uni.end(), co.begin(), co.end());
		sort(uni.begin(), uni.end());
		// just assert things
		for (int i = 1; i < (int)uni.size(); i++) {
			assert(uni[i] != uni[i-1]);
		}
		for (int x : PMCs[trp.S]) {
			assert(binary_search(uni.begin(), uni.end(), x));
		}
		for (int x : sep) {
			assert(binary_search(PMCs[trp.S].begin(), PMCs[trp.S].end(), x));
		}
		// assert ends
		
		for (int x : sep) {
			inSep[x] = 1;
		}
		int fill = 0;
		if (PROBLEM == PROBLEM_MINFILL) {
			for (int i = 0; i < (int)PMCs[trp.S].size(); i++) {
				for (int ii = i+1; ii < (int)PMCs[trp.S].size(); ii++) {
					int a = PMCs[trp.S][i];
					int b = PMCs[trp.S][ii];
					if (!G.hasEdge(a, b)) {
						if (!inSep[a] || !inSep[b]) {
							fill++;
						}
					}
				}
			}
		}
		else {
			abort();
		}
		assert(fill <= upperBound);
		for (int x : sep) {
			inSep[x] = 0;
		}
		int tval = 0;
		if (PMCs[trp.S].size() == uni.size()) {
			tval = fill;
		}
		else {
			tval = fill;
			for (int i = 0; i < G.n; i++) {
				u[i] = -1;
			}
			for (int x : uni) {
				u[x] = 0;
			}
			for (int x : PMCs[trp.S]) {
				u[x] = 2;
			}
			it = 2;
			for (int x : uni) {
				if (u[x] == 0) {
					it++;
					vector<int> nco;
					vector<int> nsep;
					dfs6(G, x, nco, nsep);
					assert(nco.size() > 0 && nco.size() < co.size());
					assert(nsep.size() > 0 && nsep.size() < PMCs[trp.S].size());
					sort(nco.begin(), nco.end());
					sort(nsep.begin(), nsep.end());
					assert(nco.size() + nsep.size() < co.size() + sep.size());
					
					if (!ccsMap.count(nco) || !sepsMap.count(nsep)) {
						tval = -1;
						break;
					}
					else {
						pair<int, int> id = {sepsMap[nsep], ccsMap[nco]};
						if (!dp.count(id)) {
							tval = -1;
							break;
						}
						else {
							if (PROBLEM == PROBLEM_MINFILL) {
								tval += dp[id].F;
							}
							else {
								abort();
							}
						}
					}
				}
			}
		}
		
		if (tval != -1 && tval <= upperBound) {
			if (!dp.count(trp.F)) {
				dp[trp.F] = {tval, trp.S};
			}
			else {
				if (tval < dp[trp.F].F) {
					dp[trp.F] = {tval, trp.S};
				}
			}
		}
	}
	if (!dp.count(rootState)) return FAILSOLUTION;
	vector<pair<int, int> > sol;
	queue<pair<int, int> > reconstruct;
	reconstruct.push(rootState);
	while (!reconstruct.empty()) {
		pair<int, int> state = reconstruct.front();
		reconstruct.pop();
		assert(dp.count(state));
		pair<pair<int, int>, int> trp = {state, dp[state].S};
		
		assert(trp.F.F < (int)seps.size());
		assert(trp.F.S < (int)ccs.size());
		vector<int> sep = seps[trp.F.F];
		vector<int> co = ccs[trp.F.S];
		vector<int> uni = sep;
		uni.insert(uni.end(), co.begin(), co.end());
		sort(uni.begin(), uni.end());
		// just assert things
		for (int i = 1; i < (int)uni.size(); i++) {
			assert(uni[i] != uni[i-1]);
		}
		for (int x : PMCs[trp.S]) {
			assert(binary_search(uni.begin(), uni.end(), x));
		}
		for (int x : sep) {
			assert(binary_search(PMCs[trp.S].begin(), PMCs[trp.S].end(), x));
		}
		// assert ends
		
		for (int x : sep) {
			inSep[x] = 1;
		}
		for (int i = 0; i < (int)PMCs[trp.S].size(); i++) {
			for (int ii = i+1; ii < (int)PMCs[trp.S].size(); ii++) {
				int a = PMCs[trp.S][i];
				int b = PMCs[trp.S][ii];
				if (!G.hasEdge(a, b)) {
					if (!inSep[a] || !inSep[b]) {
						sol.push_back({a, b});
					}
				}
			}
		}
		for (int x : sep) {
			inSep[x] = 0;
		}
		
		if (PMCs[trp.S].size() != uni.size()) {
			for (int i = 0; i < G.n; i++) {
				u[i] = -1;
			}
			for (int x : uni) {
				u[x] = 0;
			}
			for (int x : PMCs[trp.S]) {
				u[x] = 2;
			}
			it = 2;
			for (int x : uni) {
				if (u[x] == 0) {
					it++;
					vector<int> nco;
					vector<int> nsep;
					dfs6(G, x, nco, nsep);
					assert(nco.size() > 0 && nco.size() < co.size());
					assert(nsep.size() > 0 && nsep.size() < PMCs[trp.S].size());
					sort(nco.begin(), nco.end());
					sort(nsep.begin(), nsep.end());
					assert(nco.size() + nsep.size() < co.size() + sep.size());
					
					assert(ccsMap.count(nco) && sepsMap.count(nsep));
					pair<int, int> id = {sepsMap[nsep], ccsMap[nco]};
					reconstruct.push(id);
				}
			}
		}
	}
	if (PROBLEM == PROBLEM_MINFILL) assert((int)sol.size() == dp[rootState].F);
	sort(sol.begin(), sol.end());
	for (int i = 1; i < (int)sol.size(); i++) {
		assert(sol[i] != sol[i-1]);
	}
	if (PROBLEM == PROBLEM_MINFILL) assert((int)sol.size() >= lowerBound);
	return sol;
}

}
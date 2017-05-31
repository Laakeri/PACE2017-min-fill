#include "graph.hpp"
#include "IO.hpp"
#include "timer.hpp"
#include "global.hpp"
#include "minfill.hpp"
#include "MCS.hpp"
#include "vertexcover.hpp"
#include "preprocess.hpp"
#include "PMC.hpp"

#include <iostream>
#include <vector>
#include <cassert>

#define F first
#define S second

using namespace std;

int main(int argc, char** argv) {
	int upperBound = -1;
	bool preprocess = false;
	int problem = PROBLEM_DEFAULT;
	bool printInfo = false;
	for (int i = 1; i < argc; i++) {
		string s(argv[i]);
		if (s == "-pp") {
			preprocess = true;
		}
		else if (s == "-minfill") {
			problem = PROBLEM_MINFILL;
		}
		else if (s == "-s") {
			// ignore random seed
			assert(i+1 < argc);
			i++;
		}
		else if (s == "-info") {
			printInfo = true;
		}
		else if (s == "-pmcprogress") {
			PMC::setPrintProcess(true);
		}
		else if (s.size() > 3 && s.substr(0, 3) == "-k=") {
			upperBound = 0;
			for (int j = 3; j < (int)s.size(); j++) {
				assert(isdigit(s[j]));
				upperBound *= 10;
				upperBound += (s[j] - '0');
			}
			if (VERB >= 1) cerr<<"Search upper bound "<<upperBound<<endl;
		}
		else {
			cerr<<"invalid arg"<<endl;
			abort();
		}
	}
	Timer timer;
	timer.start();
	set<pair<int, int> > edges;
	IO io;
	int n = io.readInput(cin, edges);
	int m = edges.size();
	if (VERB >= 1) cerr<<"n = "<<n<<endl;
	if (VERB >= 1) cerr<<"m = "<<m<<endl;
	assert(n <= maxN);
	Graph G(edges);
	
	if (upperBound == -1) {
		upperBound = (int)1e9;
	}
	if (problem == PROBLEM_MINFILL) {
		if (n == 0) return 0;
		vector<pair<int, int> > sol = MinFill::SolveGraph::solveGraph(G, 0, upperBound, preprocess, 0);
		
		if (preprocess) {
			Preprocess::print(cout);
		}
		else {
			if (sol == FAILSOLUTION) {
				assert(upperBound < (int)1e9);
				cout<<"No fill in with <= "<<upperBound<<" edges"<<endl;
				cerr<<"No fill in with <= "<<upperBound<<" edges"<<endl;
			}
			else {
				// check correctness
				G.addEdges(sol);
				auto fill0 = MCS::MCS_MP(G);
				assert(fill0.F.size() == 0);
				
				sol = G.mapBack(sol);
				
				io.printFill(cout, sol);
				
				if (VERB >= 1 || printInfo) {
					cerr<<"Fill size: "<<sol.size()<<endl;
					cerr<<"Treewidth: "<<MCS::getTreeWidth(G)<<endl;
					cerr<<"Time: "<<timer.getTime().count()<<endl;
				}
			}
		}
	}
	else {
		abort();
	}
}
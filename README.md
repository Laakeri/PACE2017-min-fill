# Minimum fill-in solver

Implementation of exact minimum fill-in solver for PACE challenge 2017 https://pacechallenge.wordpress.com/

## How to use

Build the code with the makefile provided.

Run with `./main`

The solver reads the input from the standard input and outputs the solution to the standard output. The output format is always the PACE 2017 format: each line contains description of one fill edge. Description of an edge is its endpoints separated by a space. The solver supports multiple input formats. All input formats present in the instances directory are supported.

### Flags

Some flags that can be used:

`-info` Prints the optimal fill size, the resulting treewidth (not necessary optimal treewidth) and the time used to the standard error after outputting the solution.

`-k=X` Sets upper bound X for the search. If the size of the smallest solution is > X, the solver will output that there is no solution. Otherwise it will output the optimal solution.

`-pp` The solver will preprocess the graph. The preprocessor will output a graph with the same size minimum fill-in as the graph given as input. Other properties of the graph are not preserved.

`-pmcprogress` The solver will print some info about the progress of the PMC algorithm to the standard error.

## Algorithms

The solver first decomposes the graph by its clique separators [8] and solves the problem in each of them separately. After this some other preprocessing rules [3] are applied to the graph. The maximum cardinality search- algorithm [1, 9] is used for obtaining upper bounds for the solution and for the clique separator decomposition. Kernelization with O(k^2) kernel size [7] is applied after this. When the graph cannot be reduced anymore by these methods, the solver starts the actual search. We have implemented three different algorithms for this.

##### The potential maximal cliques algorithm.

The algorithm using potential maximal cliques is the most efficient of the implemented algorithms in most of the cases. The algorithm lists all minimal separators of the graph in O(|minseps| n^3) time [2], then lists all potential maximal cliques of the graph in O(|minseps|^2 n^2 m) time [4] and then computes the minimum fill-in using the O(|PMCs| n^3) dynamic programming algorithm over potential maximal cliques [5, 6]. The bottleneck in this algorithm is listing all potential maximal cliques. An algorithm for enumerating potential maximal cliques in O(|PMCs| poly(n)) time could improve the running time of the implementation significantly, since the number of potential maximal cliques seems to be much less than O(|minseps|^2).

##### Branching from the vertex cover instance

The minimum fill in problem in dense graphs generates a vertex cover problem that has to be satisfied: each chordless 4-cycle has to be filled with either of the possible fill edges. We compute lower bounds by solving the vertex cover problem exactly and we branch from high degree vertices of the vertex cover instance. This approach is used if the lower bound given by the vertex cover instance matches the upper bound or if it there are too many minimal separators for the PMC algorithm.

##### Branching from chordless cycle

It is also possible to branch directly from chordless cycles. This approach is too slow to solve anything meaningful.


[1] Berry, Anne, Jean RS Blair, and Pinar Heggernes. "Maximum cardinality search for computing minimal triangulations." International Workshop on Graph-Theoretic Concepts in Computer Science. Springer Berlin Heidelberg, 2002.

[2] Berry, Anne, Jean-Paul Bordat, and Olivier Cogis. "Generating all the minimal separators of a graph." International Workshop on Graph-Theoretic Concepts in Computer Science. Springer Berlin Heidelberg, 1999.

[3] Bodlaender, Hans L., and Arie MCA Koster. "Safe separators for treewidth." Discrete Mathematics 306.3 (2006): 337-350.

[4] Bouchitté, Vincent, and Ioan Todinca. "Listing all potential maximal cliques of a graph." Theoretical Computer Science 276.1 (2002): 17-32.

[5] Bouchitté, Vincent, and Ioan Todinca. "Treewidth and minimum fill-in: Grouping the minimal separators." SIAM Journal on Computing 31.1 (2001): 212-232.

[6] Fomin, Fedor V., Dieter Kratsch, and Ioan Todinca. "Exact (exponential) algorithms for treewidth and minimum fill-in." International Colloquium on Automata, Languages, and Programming. Springer Berlin Heidelberg, 2004.

[7] Natanzon, Assaf, Ron Shamir, and Roded Sharan. "A polynomial approximation algorithm for the minimum fill-in problem." SIAM Journal on Computing 30.4 (2000): 1067-1079.

[8] Tarjan, Robert E. "Decomposition by clique separators." Discrete mathematics 55.2 (1985): 221-232.

[9] Tarjan, Robert E., and Mihalis Yannakakis. "Simple linear-time algorithms to test chordality of graphs, test acyclicity of hypergraphs, and selectively reduce acyclic hypergraphs." SIAM Journal on computing 13.3 (1984): 566-579.



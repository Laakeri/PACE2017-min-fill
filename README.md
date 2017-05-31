# Minimum fill-in solver

Implementation of exact minimum fill-in solver for PACE challenge 2017 https://pacechallenge.wordpress.com/

## How to use

Build the code with the makefile provided.

Run with ./main

The solver reads the input from the standard input and outputs the solution to the standard output. The output format is always the PACE 2017 format: each line contains description of one fill edge. Description of an edge is its endpoints separated by a space. The solver supports multiple input formats. All input formats present in the instances directory are supported.

### Flags

Some flags that can be used:

-info Prints the optimal fill size, the resulting treewidth (not necessary optimal treewidth) and the time used to the standard error after outputting the solution.

-k=X Sets upper bound X for the search. If the size of the smallest solution is > X, the solver will output that there is no solution. Otherwise it will output the optimal solution.

-pp The solver will preprocess the graph. The preprocessor will output a graph with the same size minimum fill-in as the graph given as input. Other properties of the graph are not preserved.

-pmcprogress The solver will print some info about the progress of the PMC algorithm to the standard error.

## Algorithms




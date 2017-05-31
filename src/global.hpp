#ifndef MINFILL_GLOBAL_HPP
#define MINFILL_GLOBAL_HPP

#include <vector>
#include <cstdint>
#include "timer.hpp"

const int maxN = 1000000;
const std::vector<std::pair<int, int> > FAILSOLUTION = {{-1, -1}};
const int VERB = 0;

const bool useVC = true;

const int VCbranchDg = 20;

const int PROBLEM_MINFILL = 1;

const int PROBLEM_DEFAULT = PROBLEM_MINFILL;

#endif
#pragma once
#include <vector>
#include "graph.hpp"
using namespace graph;

//Поиск в ширину
std::vector<int> bfs(basic_graph<int>);

//Поиск в длину
std::vector<int> lfs(basic_graph<int>);
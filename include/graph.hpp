#ifndef __GRAPH_HPP__
#define __GRAPH_HPP__

#include <set>
#include <map>
#include <vector>
#include <utility>
#include <string>
#include <chrono>
#include <iostream>
#include <queue>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include "utils.hpp"

const int INF = 1e9;
const int SPACE_OFFSET = 4;
const string SEPARETOR = "------------------------------------------------------";

using namespace std;

class Graph
{
private:
    set<int> nodes;
    map<pair<int, int>, int> edge_weights;

public:
    Graph(string topology);
    void show();
    void add_edge(int v, int u, int w);
    void delete_edge(int v, int u);
    void modify_edge(int v, int u, int new_cost);
    void link_state(int s);
    void distance_vector(int s);
    void bgp();
};

#endif
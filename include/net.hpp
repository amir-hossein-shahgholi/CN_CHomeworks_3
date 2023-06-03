#ifndef __NET_HPP__
#define __NET_HPP__

#include "graph.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class Net
{
private:
    Graph *graph;

public:
    void run();
};

#endif
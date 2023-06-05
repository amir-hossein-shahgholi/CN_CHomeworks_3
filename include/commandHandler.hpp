#ifndef __COMMANDHANDLER_HPP__
#define __COMMANDHANDLER_HPP__

#include "graph.hpp"
#include "utils.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class CommandHandler
{
private:
    Graph *graph;

public:
    void run();
};

#endif
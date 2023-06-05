#include "../include/graph.hpp"
#include "../include/utils.hpp"

#include <iostream>
#include <chrono>
#include <sstream>
#include <algorithm>

using namespace std;

Graph::Graph(string main_topology)
{
    vector<string> phrases = split(main_topology, ' ');
    for (auto phrase : phrases)
    {
        vector<string> edge = split(phrase, '-');
        int v = stoi(edge[0]);
        int u = stoi(edge[1]);
        int weight = stoi(edge[2]);
        nodes.insert(v);
        nodes.insert(u);
        add_edge(u, v, weight);
        add_edge(v, u, weight);
    }
}

void Graph::add_edge(int v, int u, int weight)
{
    edge_weights[make_pair(v, u)] = weight;
    edge_weights[make_pair(u, v)] = weight;
}

void Graph::delete_edge(int v, int u)
{
    edge_weights.erase(make_pair(u, v));
    edge_weights.erase(make_pair(v, u));
}

void Graph::modify_edge(int v, int u, int cost)
{
    edge_weights[make_pair(v, u)] = cost;
    edge_weights[make_pair(u, v)] = cost;
}

void Graph::show()
{
    cout << "   |";
    for (auto node : nodes)
    {
        int col = SPACE_OFFSET - digit_counter(node);
        for (int i = 0; i < col; i++)
            cout << " ";
        cout << node;
    }
    cout << endl;
    cout << SEPARETOR;
    cout << endl;

    for (auto v : nodes)
    {
        cout << v;
        for (int i = 0; i < SPACE_OFFSET - digit_counter(v) - 1; i++)
            cout << " ";
        cout << "|";
        for (auto u : nodes)
        {
            int w, col;
            if (u == v)
            {
                col = 3;
                w = 0;
            }
            else if (edge_weights.find(make_pair(v, u)) == edge_weights.end())
            {
                col = 2;
                w = -1;
            }
            else
            {
                w = edge_weights[make_pair(v, u)];
                col = SPACE_OFFSET - digit_counter(w);
                if (w <= 0)
                    col--;
            }

            for (int i = 0; i < col; i++)
                cout << " ";
            cout << w;
        }
        cout << endl;
    }
}

void Graph::link_state(int s)
{
    int n = nodes.size();
    vector<bool> mark(n + 1, false);
    vector<int> dist(n + 1, INF);
    vector<int> par(n + 1, -1);
    auto start = std::chrono::steady_clock::now();

    for (auto v : nodes)
    {
        if (edge_weights.find(make_pair(s, v)) != edge_weights.end())
        {
            dist[v] = edge_weights[make_pair(s, v)];
            par[v] = s;
        }
        else
            dist[v] = INF;
    }

    mark[s] = true;
    dist[s] = 0;
    int sz = 1;
    while (sz < n)
    {
        int mn = INF, v;
        for (auto node : nodes)
        {
            if (mark[node])
                continue;
            if (dist[node] < mn)
            {
                mn = dist[node];
                v = node;
            }
        }

        cout << "   |Iter " + to_string(sz) << ":" << endl;
        cout << "Dest|";
        for (auto node : nodes)
        {
            int col = 4 - digit_counter(node);
            for (int i = 0; i < col; i++)
                cout << " ";
            cout << node << "|";
        }
        cout << endl;
        cout << "Cost|";
        for (auto node : nodes)
        {
            int col = 4 - digit_counter(dist[node]);
            if (dist[node] == INF)
                col = 2;
            for (int i = 0; i < col; i++)
                cout << " ";
            cout << (dist[node] == INF ? -1 : dist[node]) << "|";
        }
        cout << endl;
        for (int i = 0; i < (nodes.size() + 1) * 5; i++)
            cout << "-";
        cout << endl;

        mark[v] = true;
        sz += 1;
        for (auto node : nodes)
        {
            if (mark[node])
                continue;
            if (edge_weights.find(make_pair(v, node)) == edge_weights.end())
                continue;
            if (dist[v] + edge_weights[make_pair(v, node)] < dist[node])
            {
                dist[node] = dist[v] + edge_weights[make_pair(v, node)];
                par[node] = v;
            }
        }
    }

    string path;
    cout << "\nPath: [s] -> [d]         Min-Cost         Shortest Path" << endl;
    cout << "---------------------------------------------------------" << endl;
    for (auto node : nodes)
    {
        path = "";
        if (node == s)
            continue;
        cout << "    [" << s << "] -> [" << node << "]";
        int col = 15 - digit_counter(node);
        for (int i = 0; i < col; i++)
            cout << " ";
        cout << dist[node];
        col = 15 - digit_counter(dist[node]);
        for (int i = 0; i < col; i++)
            cout << " ";
        int p = node;
        while (par[p] != -1)
        {
            path = " -> " + to_string(p) + path;
            p = par[p];
        }
        cout << to_string(s) << path << endl;
    }

    auto finish = std::chrono::steady_clock::now();
    double elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start).count();
    cout << "Elapsed: " << elapsed_seconds << endl;
}

void Graph::distance_vector(int s)
{
    auto start = std::chrono::steady_clock::now();

    int n = nodes.size();
    vector<bool> mark(n + 1, false);
    vector<int> dist(n + 1, INF);
    vector<int> par(n + 1, -1);

    dist[s] = 0;

    while (1)
    {
        bool updated = false;
        for (map<pair<int, int>, int>::iterator it = edge_weights.begin(); it != edge_weights.end(); it++)
        {
            int v = it->first.first, u = it->first.second, w = it->second;
            if (dist[v] + w < dist[u])
            {
                dist[u] = dist[v] + w;
                par[u] = v;
                updated = true;
            }
        }
        if (!updated)
            break;
    }

    string path;
    int prev_p;
    cout << "\nDest         NextHop         Dist         Shortest Path" << endl;
    cout << "---------------------------------------------------------" << endl;
    for (auto node : nodes)
    {
        path = "";
        if (node == s)
            continue;
        cout << node;
        int col = 15 - digit_counter(node);
        for (int i = 0; i < col; i++)
            cout << " ";
        int p = node;
        while (par[p] != -1)
        {
            path = " -> " + to_string(p) + path;
            prev_p = p;
            p = par[p];
        }

        cout << prev_p;
        col = 15 - digit_counter(prev_p);
        for (int i = 0; i < col; i++)
            cout << " ";

        cout << dist[node];
        col = 12 - digit_counter(dist[node]);
        for (int i = 0; i < col; i++)
            cout << " ";

        cout << to_string(s) << path << endl;
    }

    auto finish = std::chrono::steady_clock::now();
    double elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start).count();
    cout << "Elapsed: " << elapsed_seconds << endl;
}

void Graph::bgp(int s)
{
}

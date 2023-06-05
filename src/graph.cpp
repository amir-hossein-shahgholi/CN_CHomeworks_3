#include "../include/graph.hpp"

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
    if (edge_weights.count(make_pair(u, v)) == 0)
    {
        cout << "Edge does not exist" << endl;
        return;
    }
    edge_weights.erase(make_pair(u, v));
    edge_weights.erase(make_pair(v, u));
    cout << "OK" << endl;
    return;
}

void Graph::modify_edge(int v, int u, int cost)
{
    if (v == u)
    {
        cout << "Invalid nodes." << endl;
        return;
    }
    edge_weights[make_pair(v, u)] = cost;
    edge_weights[make_pair(u, v)] = cost;
    cout << "OK" << endl;
    return;
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
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> distance(n + 1, INF);
    vector<int> par(n + 1, -1);
    auto start = std::chrono::steady_clock::now();

    distance[s] = 0;
    pq.push({0, s});

    int counter = 1;
    while (!pq.empty())
    {
        int dist = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (distance[u] != dist)
            continue;

        cout << "   |Iter " << counter << ":" << endl;
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
            int col = 4 - digit_counter(distance[node]);
            if (distance[node] == INF)
                col = 2;
            for (int i = 0; i < col; i++)
                cout << " ";
            cout << (distance[node] == INF ? -1 : distance[node]) << "|";
        }
        cout << endl;
        for (std::string::size_type i = 0; i < (nodes.size() + 1) * 5; i++)
            cout << "-";
        cout << endl;

        counter++;

        for (auto v : nodes)
        {
            if (edge_weights.find(make_pair(u, v)) != edge_weights.end())
            {
                int alt = distance[u] + edge_weights[make_pair(u, v)];
                if (alt < distance[v])
                {
                    distance[v] = alt;
                    par[v] = u;
                    pq.push({distance[v], v});
                }
            }
        }
    }

    string path;
    cout << "\nPath: [s] -> [d]         Min-Cost         Shortest Path" << endl;
    cout << SEPARETOR << endl;
    for (auto node : nodes)
    {
        path = "";
        if (node == s)
            continue;
        cout << "    [" << s << "] -> [" << node << "]";
        int col = 15 - digit_counter(node);
        for (int i = 0; i < col; i++)
            cout << " ";
        cout << distance[node];
        col = 15 - digit_counter(distance[node]);
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
    vector<bool> seen(n + 1, false);
    vector<int> distance(n + 1, INF);
    vector<int> par(n + 1, -1);

    distance[s] = 0;

    while (true)
    {
        bool is_updated = false;
        for (map<pair<int, int>, int>::iterator it = edge_weights.begin(); it != edge_weights.end(); it++)
        {
            int v = it->first.first, u = it->first.second, w = it->second;
            if (distance[v] + w < distance[u])
            {
                distance[u] = distance[v] + w;
                par[u] = v;
                is_updated = true;
            }
        }
        if (!is_updated)
            break;
    }

    string path;
    int prev_p;
    cout << "\nDest         NextHop         Dist         Shortest Path" << endl;
    cout << SEPARETOR << endl;
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

        cout << distance[node];
        col = 12 - digit_counter(distance[node]);
        for (int i = 0; i < col; i++)
            cout << " ";

        cout << to_string(s) << path << endl;
    }

    auto finish = std::chrono::steady_clock::now();
    double elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(finish - start).count();
    cout << "Elapsed: " << elapsed_seconds << endl;
}

void Graph::bgp()
{
    cout << "Not implemented" << endl;
}

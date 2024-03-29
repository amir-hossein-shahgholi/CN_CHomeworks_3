#include "../include/commandHandler.hpp"

void CommandHandler::run()
{
    string command, args;
    stringstream command_stream;
    vector<string> arg_list;
    while (getline(cin, command))
    {

        command_stream = stringstream(command);
        getline(command_stream, command, ' ');
        getline(command_stream, args);
        if (command == "topology")
            graph = new Graph(args);
        else if (command == "modify")
        {
            arg_list = split(args, '-');
            graph->modify_edge(stoi(arg_list[0]), stoi(arg_list[1]), stoi(arg_list[2]));
        }
        else if (command == "remove")
        {
            arg_list = split(args, '-');
            graph->delete_edge(stoi(arg_list[0]), stoi(arg_list[1]));
        }
        else if (command == "show")
            graph->show();
        else if (command == "lsrp")
            graph->link_state(stoi(args));
        else if (command == "dvrp")
            graph->distance_vector(stoi(args));
        else if (command == "bgp")
            graph->bgp();
        else
            cout << "Error: Unknown Command" << endl;
    }
}
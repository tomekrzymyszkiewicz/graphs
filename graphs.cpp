#include <iostream>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <queue>
#include <chrono>
#include <vector>
#include <conio.h>
#include <random>
#include <string>
#include <windows.h>
#include "structures.h"
using namespace std;
using namespace std::chrono;

vector<vector<string>> tasks;
vector<string> results;
string data_file_name = "";
string results_file_name = "";
vector<node> graph_data = vector<node>();
int data_amount = 0;

adjacency_matrix *prim(adjacency_matrix graph_matrix, int start_vertex)
{
    priority_queue<node, vector<node>, node::compare> pq;
    bool *visited = new bool[graph_matrix.number_of_vertices];
    for (int i = 0; i < graph_matrix.number_of_vertices; i++)
        visited[i] = false;
    adjacency_matrix *MST = new adjacency_matrix(graph_matrix.number_of_vertices);
    int MST_weight = 0;
    int v = start_vertex;
    visited[v] = true;
    pq = priority_queue<node, vector<node>, node::compare>();
    for (int i = 0; i < graph_matrix.number_of_vertices - 1; i++)
    {
        // graph_matrix.print();
        for (int j = 0; j < graph_matrix.number_of_vertices; j++)
        {
            if (graph_matrix.matrix[v][j] != 0 && !visited[j])
            {
                pq.push(node(v, j, graph_matrix.matrix[v][j]));
            }
        }
        visited[v] = true;
        while (visited[pq.top().destination] && !pq.empty())
        {
            pq.pop();
        }
        MST->add_edge_undir(pq.top().source, pq.top().destination, pq.top().weight);
        MST_weight += pq.top().weight;
        v = pq.top().destination;
        if (pq.empty())
            return MST;
        pq.pop();
    }
    return MST;
}

adjacency_list *prim(adjacency_list graph_list, int start_vertex)
{
    priority_queue<node, vector<node>, node::compare> pq;
    bool *visited = new bool[graph_list.number_of_vertices];
    for (int i = 0; i < graph_list.number_of_vertices; i++)
        visited[i] = false;
    adjacency_list *MST = new adjacency_list(graph_list.number_of_vertices);
    int MST_weight = 0;
    int v = start_vertex;
    visited[v] = true;
    pq = priority_queue<node, vector<node>, node::compare>();
    for (int i = 0; i < graph_list.number_of_vertices - 1; i++)
    {
        for (adjacency_list::list_node *current_node = graph_list.array_of_lists[v]; current_node != nullptr; current_node = current_node->next)
        {
            if (!visited[current_node->dst])
                pq.push(node(v, current_node->dst, current_node->weight));
        }
        visited[v] = true;
        while (visited[pq.top().destination] && !pq.empty())
        {
            pq.pop();
        }
        MST->add_edge_undir(pq.top().source, pq.top().destination, pq.top().weight);
        MST_weight += pq.top().weight;
        v = pq.top().destination;
        if (pq.empty())
            return MST;
        pq.pop();
    }
    return MST;
}

int **dijkstra(adjacency_list graph_list, int start_vertex)
{
    int **paths_array = new int *[2];
    paths_array[0] = new int[graph_list.number_of_vertices]; //array of distances
    paths_array[1] = new int[graph_list.number_of_vertices]; //array of predecessors
    bool *visited = new bool[graph_list.number_of_vertices];
    for (int i = 0; i < graph_list.number_of_vertices; i++)
    {
        paths_array[0][i] = INT_MAX;
        paths_array[1][i] = -1;
        visited[i] = false;
    }
    paths_array[0][start_vertex] = 0;
    paths_array[1][start_vertex] = start_vertex;
    int v = start_vertex; //current vertex in algorithm
    for (int i = 0; i < graph_list.number_of_vertices; i++)
    {
        //find unvisited vertex with shortes path
        int min = INT_MAX;
        for (int i = 0; i < graph_list.number_of_vertices; i++)
        {
            if (paths_array[0][i] < min && !visited[i])
            {
                min = paths_array[0][i];
                v = i;
            }
        }
        for (adjacency_list::list_node *current_node = graph_list.array_of_lists[v]; current_node != nullptr; current_node = current_node->next)
        {
            if (current_node && paths_array[0][v] + current_node->weight < paths_array[0][current_node->dst] && !visited[v])
            {
                paths_array[0][current_node->dst] = paths_array[0][v] + current_node->weight;
                paths_array[1][current_node->dst] = v;
            }
        }
        visited[v] = true;
    }
    return paths_array;
}

int **dijkstra(adjacency_matrix graph_matrix, int start_vertex)
{
    int **paths_array = new int *[2];
    paths_array[0] = new int[graph_matrix.number_of_vertices]; //array of distances
    paths_array[1] = new int[graph_matrix.number_of_vertices]; //array of predecessors
    bool *visited = new bool[graph_matrix.number_of_vertices];
    for (int i = 0; i < graph_matrix.number_of_vertices; i++)
    {
        paths_array[0][i] = INT_MAX;
        paths_array[1][i] = -1;
        visited[i] = false;
    }
    paths_array[0][start_vertex] = 0;
    paths_array[1][start_vertex] = start_vertex;
    int v = start_vertex; //current vertex in algorithm
    for (int i = 0; i < graph_matrix.number_of_vertices; i++)
    {
        //find unvisited vertex with shortes path
        int min = INT_MAX;
        for (int i = 0; i < graph_matrix.number_of_vertices; i++)
        {
            if (paths_array[0][i] < min && !visited[i])
            {
                min = paths_array[0][i];
                v = i;
            }
        }
        for (int i = 0; i < graph_matrix.number_of_vertices; i++)
        { //v - source i - destination value - weight
            if (graph_matrix.matrix[v][i] != 0 && paths_array[0][v] + graph_matrix.matrix[v][i] < paths_array[0][i] && !visited[v])
            {
                paths_array[0][i] = paths_array[0][v] + graph_matrix.matrix[v][i];
                paths_array[1][i] = v;
            }
        }
        visited[v] = true;
    }
    return paths_array;
}

void print_dijkstra(int **result_array, int number_of_vertices)
{
    for (int i = 0; i < (((number_of_vertices * 3) - 1) / 2); i++)
        printf("=");
    printf("SHORTEST PATHS");
    for (int i = 0; i < (((number_of_vertices * 3) - 1) / 2); i++)
        printf("=");
    printf("\nINDEX       |");
    for (int i = 0; i < number_of_vertices; i++)
        printf("%3d", i);
    printf("\nDISTANCE    |");
    for (int i = 0; i < number_of_vertices; i++)
    {
        if (result_array[0][i] == INT_MAX)
            printf(" in");
        else
            printf("%3d", result_array[0][i]);
    }
    printf("\nPREDECESSOR |");
    for (int i = 0; i < number_of_vertices; i++)
        printf("%3d", result_array[1][i]);
    printf("\n");
}

struct Result
{
    string structure;
    string operation;
    string graph_type;
    int size_of_structure;
    double time_memory;
    int number_of_repeats;
    string test_type;
    Result(string structure, string operation, string graph_type, int size_of_structure, double time_memory, int number_of_repeats, string test_type)
    {
        this->structure = structure;
        this->operation = operation;
        this->graph_type = graph_type;
        this->size_of_structure = size_of_structure;
        this->time_memory = time_memory;
        this->number_of_repeats = number_of_repeats;
        this->test_type = test_type;
    }
    string toString()
    {
        return (structure + ","+ graph_type+"," + operation + "," + to_string(size_of_structure) + "," + to_string(time_memory) + "," + to_string(number_of_repeats) + "," + test_type);
    }
};

void save_results(string results_file_name)
{
    cout << "Saving results" << endl;
    fstream fout;
    fout.open(results_file_name, ios::out);
    fout << "structure,graph_type,operation,size_of_structure,test_type,number_of_repeats" << endl;
    for (int i = 0; i < results.size(); i++)
    {
        fout << results[i] << endl;
    }
    fout.close();
    cout << "Correctly saved " << results.size() << " results" << endl;
}

bool load_data(string file_name, int amount)
{
    cout << "Loading data from " << file_name << " file" << endl;
    ifstream fin;
    fin.open(file_name, ios::in);
    if (!fin.good())
    {
        cout << "Data file " << file_name << " not exist" << endl;
        fin.close();
        return false;
    }
    string loaded_source, loaded_destination, loaded_weight;
    int data_loaded = 0;
    for (int i = 0; i < amount; i++)
    {
        fin >> loaded_source >> loaded_destination >> loaded_weight;
        if (!fin.eof())
        {
            graph_data.push_back(node(stoi(loaded_source), stoi(loaded_destination), stoi(loaded_weight)));
        }
        else
        {
            cout << "Not enough elements in data file " << file_name << endl;
            return false;
        }
        data_loaded++;
    }
    cout << "Loaded correctly " << data_loaded << " numbers" << endl;
    fin.close();
    return true;
}

void load_config()
{
    cout << "Loading config.ini" << endl;
    ifstream fin;
    fin.open("config.ini", ios::in);
    if (!fin.good())
    {
        cout << "Config.ini not found" << endl;
        fin.close();
        return;
    }
    vector<string> row;
    string line;
    fin >> data_file_name >> data_amount;
    fin >> results_file_name;
    while (!fin.eof())
    {
        string structure, graph_type, operation, min_size, max_size, step, number_of_repeats, test_type;
        fin >> structure >> graph_type >> operation >> min_size >> max_size >> step >> number_of_repeats >> test_type;
        if (structure.size() == 0 || graph_type.size() == 0 || operation.size() == 0 || min_size.size() == 0 || max_size.size() == 0 || step.size() == 0 || number_of_repeats.size() == 0 || test_type.size() == 0)
        {
            cout << "Cannot load this task: " << structure << graph_type << operation << min_size << max_size << step << number_of_repeats << test_type;
            break;
        }
        vector<string> task;
        task.push_back(structure);
        task.push_back(graph_type);
        task.push_back(operation);
        task.push_back(min_size);
        task.push_back(max_size);
        task.push_back(step);
        task.push_back(number_of_repeats);
        task.push_back(test_type);
        tasks.push_back(task);
    }
    fin.close();
    cout << "Config loaded correctly" << endl;
    return;
}

int main()
{
    load_config();
    if (!load_data(data_file_name, data_amount))
    {
        cout << "Cannot load " << data_amount << " numbers from " << data_file_name << " file." << endl;
    }
    if (tasks.size() == 0)
    {
        cout << "No tasks found to be performed." << endl;
    }
    else
    {
        for (int i = 0; i < tasks.size(); i++)
        {
            string structure = tasks[i][0];
            string graph_type = tasks[i][1];
            string operation = tasks[i][2];
            int min_size = stoi(tasks[i][3]);
            int max_size = stoi(tasks[i][4]);
            int step = stoi(tasks[i][5]);
            int number_of_repeats = stoi(tasks[i][6]);
            string test_type = tasks[i][7];
            cout << "Computing " << operation << " in " << structure << " in range from " << min_size << " to " << max_size << " with step " << step << " repeated " << number_of_repeats << " times" << endl;
            if (min_size < 1)
            {
                cout << "Cannot execute task. The array must to have at least 1 element.";
            }
            else if (number_of_repeats < 1)
            {
                cout << "Cannot execute task. The minimum number of repetitions is 1.";
            }
            else
            {
                if (structure == "adjacency_matrix")
                {
                    if (graph_type == "directed")
                    {
                        if (operation == "MST")
                        {
                            if (test_type == "time")
                            {
                                for (int current_size = min_size; current_size <= max_size; current_size += step)
                                {
                                    cout << "Computing MST on " << graph_type << " graph (adjacency matrix) with " << current_size << " elements. Time test" << endl;
                                    high_resolution_clock::time_point t_start = high_resolution_clock::now();
                                    high_resolution_clock::time_point t_end = high_resolution_clock::now();
                                    duration<double> time_span = duration<double>(0);
                                    adjacency_matrix current_graph = adjacency_matrix(current_size);
                                    for (int j = 0; graph_data[j].source < current_size && j < graph_data.size(); j++)
                                    {
                                        current_graph.add_edge_dir(graph_data[j].source, graph_data[j].destination, graph_data[j].weight);
                                    }
                                    t_start = high_resolution_clock::now();
                                    for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                    {
                                        prim(current_graph, 0);
                                    }
                                    t_end = high_resolution_clock::now();
                                    time_span = duration_cast<duration<double>>(t_end - t_start);
                                    Result adjacency_matrix_dir_MST = Result(structure, graph_type, operation, current_size, time_span.count(), number_of_repeats, test_type);
                                    results.push_back(adjacency_matrix_dir_MST.toString());
                                }
                            }
                            else if (test_type == "memory")
                            {
                                for (int current_size = min_size; current_size <= max_size; current_size += step)
                                {
                                }
                            }
                            else if (test_type == "print")
                            {
                                for (int current_size = min_size; current_size <= max_size; current_size += step)
                                {
                                    cout << "Computing MST on " << graph_type << " graph (adjacency matrix) with " << current_size << " elements" << endl;
                                    adjacency_matrix current_graph = adjacency_matrix(current_size);
                                    adjacency_matrix *mst = new adjacency_matrix();
                                    for (int j = 0; graph_data[j].source < current_size && j < graph_data.size(); j++)
                                    {
                                        current_graph.add_edge_dir(graph_data[j].source, graph_data[j].destination, graph_data[j].weight);
                                    }
                                    for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                    {
                                        mst = prim(current_graph, 0);
                                        mst->print();
                                    }
                                }
                            }
                            else
                            {
                                cout << "Cannot recognize " << test_type << " type of test.";
                            }
                        }
                        else if (operation == "shortest_paths")
                        {
                            if (test_type == "time")
                            {
                                for (int current_size = min_size; current_size <= max_size; current_size += step)
                                {
                                    cout << "Computing shortest paths on " << graph_type << " graph (adjacency matrix) with " << current_size << " elements. Time test" << endl;
                                    high_resolution_clock::time_point t_start = high_resolution_clock::now();
                                    high_resolution_clock::time_point t_end = high_resolution_clock::now();
                                    duration<double> time_span = duration<double>(0);
                                    adjacency_matrix current_graph = adjacency_matrix(current_size);
                                    for (int j = 0; graph_data[j].source < current_size && j < graph_data.size(); j++)
                                    {
                                        current_graph.add_edge_dir(graph_data[j].source, graph_data[j].destination, graph_data[j].weight);
                                    }
                                    t_start = high_resolution_clock::now();
                                    for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                    {
                                        dijkstra(current_graph, 0);
                                    }
                                    t_end = high_resolution_clock::now();
                                    time_span = duration_cast<duration<double>>(t_end - t_start);
                                    Result adjacency_matrix_dir_shortest_paths = Result(structure, graph_type, operation, current_size, time_span.count(), number_of_repeats, test_type);
                                    results.push_back(adjacency_matrix_dir_shortest_paths.toString());
                                }
                            }
                            else if (test_type == "memory")
                            {
                                for (int current_size = min_size; current_size <= max_size; current_size += step)
                                {
                                }
                            }
                            else if (test_type == "print")
                            {
                                for (int current_size = min_size; current_size <= max_size; current_size += step)
                                {
                                    cout << "Computing shortest paths on " << graph_type << " graph (adjacency matrix) with " << current_size << " elements" << endl;
                                    adjacency_matrix current_graph = adjacency_matrix(current_size);
                                    for (int j = 0; graph_data[j].source < current_size && j < graph_data.size(); j++)
                                    {
                                        current_graph.add_edge_dir(graph_data[j].source, graph_data[j].destination, graph_data[j].weight);
                                    }
                                    for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                    {
                                        print_dijkstra(dijkstra(current_graph, 0), current_size);
                                    }
                                }
                            }
                            else
                            {
                                cout << "Cannot recognize " << test_type << " type of test.";
                            }
                        }
                        else
                        {
                            cout << "Cannot recognize operation " << operation << " in " << structure << " structure.";
                        }
                    }
                    else if (graph_type == "undirected")
                    {
                        if (operation == "MST")
                        {
                            if (test_type == "time")
                            {
                                for (int current_size = min_size; current_size <= max_size; current_size += step)
                                {
                                    cout << "Computing MST on " << graph_type << " graph (adjacency matrix) with " << current_size << " elements. Time test" << endl;
                                    high_resolution_clock::time_point t_start = high_resolution_clock::now();
                                    high_resolution_clock::time_point t_end = high_resolution_clock::now();
                                    duration<double> time_span = duration<double>(0);
                                    adjacency_matrix current_graph = adjacency_matrix(current_size);
                                    for (int j = 0; graph_data[j].source < current_size && j < graph_data.size(); j++)
                                    {
                                        current_graph.add_edge_undir(graph_data[j].source, graph_data[j].destination, graph_data[j].weight);
                                    }
                                    t_start = high_resolution_clock::now();
                                    for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                    {
                                        prim(current_graph, 0);
                                    }
                                    t_end = high_resolution_clock::now();
                                    time_span = duration_cast<duration<double>>(t_end - t_start);
                                    Result adjacency_matrix_undir_MST = Result(structure, graph_type, operation, current_size, time_span.count(), number_of_repeats, test_type);
                                    results.push_back(adjacency_matrix_undir_MST.toString());
                                }
                            }
                            else if (test_type == "memory")
                            {
                                for (int current_size = min_size; current_size <= max_size; current_size += step)
                                {
                                }
                            }
                            else if (test_type == "print")
                            {
                                for (int current_size = min_size; current_size <= max_size; current_size += step)
                                {
                                    cout << "Computing MST on " << graph_type << " graph (adjacency matrix) with " << current_size << " elements" << endl;
                                    adjacency_matrix current_graph = adjacency_matrix(current_size);
                                    adjacency_matrix *mst = new adjacency_matrix();
                                    for (int j = 0; graph_data[j].source < current_size && j < graph_data.size(); j++)
                                    {
                                        current_graph.add_edge_undir(graph_data[j].source, graph_data[j].destination, graph_data[j].weight);
                                    }
                                    for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                    {
                                        mst = prim(current_graph, 0);
                                        mst->print();
                                    }
                                }
                            }
                            else
                            {
                                cout << "Cannot recognize " << test_type << " type of test.";
                            }
                        }
                        else if (operation == "shortest_paths")
                        {
                            if (test_type == "time")
                            {
                                for (int current_size = min_size; current_size <= max_size; current_size += step)
                                {
                                    cout << "Computing shortest paths on " << graph_type << " graph (adjacency matrix) with " << current_size << " elements. Time test" << endl;
                                    high_resolution_clock::time_point t_start = high_resolution_clock::now();
                                    high_resolution_clock::time_point t_end = high_resolution_clock::now();
                                    duration<double> time_span = duration<double>(0);
                                    adjacency_matrix current_graph = adjacency_matrix(current_size);
                                    for (int j = 0; graph_data[j].source < current_size && j < graph_data.size(); j++)
                                    {
                                        current_graph.add_edge_undir(graph_data[j].source, graph_data[j].destination, graph_data[j].weight);
                                    }
                                    t_start = high_resolution_clock::now();
                                    for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                    {
                                        dijkstra(current_graph, 0);
                                    }
                                    t_end = high_resolution_clock::now();
                                    time_span = duration_cast<duration<double>>(t_end - t_start);
                                    Result adjacency_matrix_undir_shortest_paths = Result(structure, graph_type, operation, current_size, time_span.count(), number_of_repeats, test_type);
                                    results.push_back(adjacency_matrix_undir_shortest_paths.toString());
                                }
                            }
                            else if (test_type == "memory")
                            {
                                for (int current_size = min_size; current_size <= max_size; current_size += step)
                                {
                                }
                            }
                            else if (test_type == "print")
                            {
                                for (int current_size = min_size; current_size <= max_size; current_size += step)
                                {
                                    cout << "Computing shortest paths on " << graph_type << " graph (adjacency matrix) with " << current_size << " elements. Time test" << endl;
                                    adjacency_matrix current_graph = adjacency_matrix(current_size);
                                    for (int j = 0; graph_data[j].source < current_size && j < graph_data.size(); j++)
                                    {
                                        current_graph.add_edge_undir(graph_data[j].source, graph_data[j].destination, graph_data[j].weight);
                                    }
                                    for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                    {
                                        print_dijkstra(dijkstra(current_graph, 0), current_size);
                                    }
                                }
                            }
                            else
                            {
                                cout << "Cannot recognize " << test_type << " type of test.";
                            }
                        }
                        else
                        {
                            cout << "Cannot recognize operation " << operation << " in " << structure << " structure.";
                        }
                    }
                    else
                    {
                        cout << "Cannot recognize " << graph_type << " type of graph.";
                    }
                }
                else if (structure == "adjacency_list")
                {
                    if (graph_type == "directed")
                    {
                        if (operation == "MST")
                        {
                            if (test_type == "time")
                            {
                                for (int current_size = min_size; current_size <= max_size; current_size += step)
                                {
                                    cout << "Computing MST on " << graph_type << " graph (adjacency list) with " << current_size << " elements. Time test" << endl;
                                    high_resolution_clock::time_point t_start = high_resolution_clock::now();
                                    high_resolution_clock::time_point t_end = high_resolution_clock::now();
                                    duration<double> time_span = duration<double>(0);
                                    adjacency_list current_graph = adjacency_list(current_size);
                                    for (int j = 0; graph_data[j].source < current_size && j < graph_data.size(); j++)
                                    {
                                        current_graph.add_edge_dir(graph_data[j].source, graph_data[j].destination, graph_data[j].weight);
                                    }
                                    t_start = high_resolution_clock::now();
                                    for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                    {
                                        prim(current_graph, 0);
                                    }
                                    t_end = high_resolution_clock::now();
                                    time_span = duration_cast<duration<double>>(t_end - t_start);
                                    Result adjacency_list_dir_MST = Result(structure, graph_type, operation, current_size, time_span.count(), number_of_repeats, test_type);
                                    results.push_back(adjacency_list_dir_MST.toString());
                                }
                            }
                            else if (test_type == "memory")
                            {
                                for (int current_size = min_size; current_size <= max_size; current_size += step)
                                {
                                }
                            }
                            else if (test_type == "print")
                            {
                                for (int current_size = min_size; current_size <= max_size; current_size += step)
                                {
                                    cout << "Computing MST on " << graph_type << " graph (adjacency list) with " << current_size << " elements" << endl;
                                    adjacency_list current_graph = adjacency_list(current_size);
                                    adjacency_list *mst = new adjacency_list();
                                    for (int j = 0; graph_data[j].source < current_size && j < graph_data.size(); j++)
                                    {
                                        current_graph.add_edge_dir(graph_data[j].source, graph_data[j].destination, graph_data[j].weight);
                                    }
                                    for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                    {
                                        mst = prim(current_graph, 0);
                                        mst->print();
                                    }
                                }
                            }
                            else
                            {
                                cout << "Cannot recognize " << test_type << " type of test.";
                            }
                        }
                        else if (operation == "shortest_paths")
                        {
                            if (test_type == "time")
                            {
                                for (int current_size = min_size; current_size <= max_size; current_size += step)
                                {
                                    cout << "Computing shortest paths on " << graph_type << " graph (adjacency list) with " << current_size << " elements. Time test" << endl;
                                    high_resolution_clock::time_point t_start = high_resolution_clock::now();
                                    high_resolution_clock::time_point t_end = high_resolution_clock::now();
                                    duration<double> time_span = duration<double>(0);
                                    adjacency_list current_graph = adjacency_list(current_size);
                                    for (int j = 0; graph_data[j].source < current_size && j < graph_data.size(); j++)
                                    {
                                        current_graph.add_edge_dir(graph_data[j].source, graph_data[j].destination, graph_data[j].weight);
                                    }
                                    t_start = high_resolution_clock::now();
                                    for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                    {
                                        dijkstra(current_graph, 0);
                                    }
                                    t_end = high_resolution_clock::now();
                                    time_span = duration_cast<duration<double>>(t_end - t_start);
                                    Result adjacency_list_dir_shortest_paths = Result(structure, graph_type, operation, current_size, time_span.count(), number_of_repeats, test_type);
                                    results.push_back(adjacency_list_dir_shortest_paths.toString());
                                }
                            }
                            else if (test_type == "memory")
                            {
                                for (int current_size = min_size; current_size <= max_size; current_size += step)
                                {
                                }
                            }
                            else if (test_type == "print")
                            {
                                for (int current_size = min_size; current_size <= max_size; current_size += step)
                                {
                                    cout << "Computing shortest paths on " << graph_type << " graph (adjacency list) with " << current_size << " elements" << endl;
                                    adjacency_matrix current_graph = adjacency_matrix(current_size);
                                    for (int j = 0; graph_data[j].source < current_size && j < graph_data.size(); j++)
                                    {
                                        current_graph.add_edge_dir(graph_data[j].source, graph_data[j].destination, graph_data[j].weight);
                                    }
                                    for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                    {
                                        print_dijkstra(dijkstra(current_graph, 0), current_size);
                                    }
                                }
                            }
                            else
                            {
                                cout << "Cannot recognize " << test_type << " type of test.";
                            }
                        }
                        else
                        {
                            cout << "Cannot recognize operation " << operation << " in " << structure << " structure.";
                        }
                    }
                    else if (graph_type == "undirected")
                    {
                        if (operation == "MST")
                        {
                            if (test_type == "time")
                            {
                                for (int current_size = min_size; current_size <= max_size; current_size += step)
                                {
                                    cout << "Computing MST on " << graph_type << " graph (adjacency list) with " << current_size << " elements. Time test" << endl;
                                    high_resolution_clock::time_point t_start = high_resolution_clock::now();
                                    high_resolution_clock::time_point t_end = high_resolution_clock::now();
                                    duration<double> time_span = duration<double>(0);
                                    adjacency_list current_graph = adjacency_list(current_size);
                                    for (int j = 0; graph_data[j].source < current_size && j < graph_data.size(); j++)
                                    {
                                        current_graph.add_edge_undir(graph_data[j].source, graph_data[j].destination, graph_data[j].weight);
                                    }
                                    t_start = high_resolution_clock::now();
                                    for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                    {
                                        prim(current_graph, 0);
                                    }
                                    t_end = high_resolution_clock::now();
                                    time_span = duration_cast<duration<double>>(t_end - t_start);
                                    Result adjacency_list_undir_MST = Result(structure, graph_type, operation, current_size, time_span.count(), number_of_repeats, test_type);
                                    results.push_back(adjacency_list_undir_MST.toString());
                                }
                            }
                            else if (test_type == "memory")
                            {
                                for (int current_size = min_size; current_size <= max_size; current_size += step)
                                {
                                }
                            }
                            else if (test_type == "print")
                            {
                                for (int current_size = min_size; current_size <= max_size; current_size += step)
                                {
                                    cout << "Computing MST on " << graph_type << " graph (adjacency list) with " << current_size << " elements" << endl;
                                    adjacency_list current_graph = adjacency_list(current_size);
                                    adjacency_list *mst = new adjacency_list();
                                    for (int j = 0; graph_data[j].source < current_size && j < graph_data.size(); j++)
                                    {
                                        current_graph.add_edge_undir(graph_data[j].source, graph_data[j].destination, graph_data[j].weight);
                                    }
                                    for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                    {
                                        mst = prim(current_graph, 0);
                                        mst->print();
                                    }
                                }
                            }
                            else
                            {
                                cout << "Cannot recognize " << test_type << " type of test.";
                            }
                        }
                        else if (operation == "shortest_paths")
                        {
                            if (test_type == "time")
                            {
                                for (int current_size = min_size; current_size <= max_size; current_size += step)
                                {
                                    cout << "Computing shortest paths on " << graph_type << " graph (adjacency list) with " << current_size << " elements. Time test" << endl;
                                    high_resolution_clock::time_point t_start = high_resolution_clock::now();
                                    high_resolution_clock::time_point t_end = high_resolution_clock::now();
                                    duration<double> time_span = duration<double>(0);
                                    adjacency_list current_graph = adjacency_list(current_size);
                                    for (int j = 0; graph_data[j].source < current_size && j < graph_data.size(); j++)
                                    {
                                        current_graph.add_edge_undir(graph_data[j].source, graph_data[j].destination, graph_data[j].weight);
                                    }
                                    t_start = high_resolution_clock::now();
                                    for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                    {
                                        dijkstra(current_graph, 0);
                                    }
                                    t_end = high_resolution_clock::now();
                                    time_span = duration_cast<duration<double>>(t_end - t_start);
                                    Result adjacency_list_undir_shortest_paths = Result(structure, graph_type, operation, current_size, time_span.count(), number_of_repeats, test_type);
                                    results.push_back(adjacency_list_undir_shortest_paths.toString());
                                }
                            }
                            else if (test_type == "memory")
                            {
                                for (int current_size = min_size; current_size <= max_size; current_size += step)
                                {
                                }
                            }
                            else if (test_type == "print")
                            {
                                for (int current_size = min_size; current_size <= max_size; current_size += step)
                                {
                                    cout << "Computing shortest paths on " << graph_type << " graph (adjacency list) with " << current_size << " elements. Time test" << endl;
                                    adjacency_list current_graph = adjacency_list(current_size);
                                    for (int j = 0; graph_data[j].source < current_size && j < graph_data.size(); j++)
                                    {
                                        current_graph.add_edge_undir(graph_data[j].source, graph_data[j].destination, graph_data[j].weight);
                                    }
                                    for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                    {
                                        print_dijkstra(dijkstra(current_graph, 0), current_size);
                                    }
                                }
                            }
                            else
                            {
                                cout << "Cannot recognize " << test_type << " type of test.";
                            }
                        }
                        else
                        {
                            cout << "Cannot recognize operation " << operation << " in " << structure << " structure.";
                        }
                    }
                    else
                    {
                        cout << "Cannot recognize " << graph_type << " type of graph.";
                    }
                }
                else
                {
                    cout << "Cannot recognize " << structure << " structure.";
                }
            }
        }
    }

    //example graph https://eduinf.waw.pl/inf/alg/001_search/0138.php
    // adjacency_matrix matrix1 = adjacency_matrix(6);
    // matrix1.add_edge_dir(0,1,3);
    // matrix1.add_edge_dir(0,4,3);
    // matrix1.add_edge_dir(1,2,1);
    // matrix1.add_edge_dir(2,3,3);
    // matrix1.add_edge_dir(2,5,1);
    // matrix1.add_edge_dir(3,1,3);
    // matrix1.add_edge_dir(4,5,2);
    // matrix1.add_edge_dir(5,0,6);
    // matrix1.add_edge_dir(5,3,1);
    // print_dijkstra(dijkstra(matrix1,0),matrix1.number_of_vertices);

    //example graph https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
    // adjacency_matrix matrix2 = adjacency_matrix(9);
    // matrix2.add_edge_undir(0,1,4);
    // matrix2.add_edge_undir(0,7,8);
    // matrix2.add_edge_undir(1,7,11);
    // matrix2.add_edge_undir(1,2,8);
    // matrix2.add_edge_undir(2,8,2);
    // matrix2.add_edge_undir(2,5,4);
    // matrix2.add_edge_undir(2,3,7);
    // matrix2.add_edge_undir(3,5,14);
    // matrix2.add_edge_undir(3,4,9);
    // matrix2.add_edge_undir(4,5,10);
    // matrix2.add_edge_undir(5,6,2);
    // matrix2.add_edge_undir(6,7,1);
    // matrix2.add_edge_undir(6,8,6);
    // matrix2.add_edge_undir(7,8,7);
    // print_dijkstra(dijkstra(matrix2,0),matrix2.number_of_vertices);

    //example graph https://eduinf.waw.pl/inf/alg/001_search/0141.php#A2
    // adjacency_matrix matrix = adjacency_matrix(10);
    // // matrix.add_edge_undir(0,8,1);
    // matrix.add_edge_undir(0,1,5);
    // matrix.add_edge_undir(0,3,9);
    // matrix.add_edge_undir(0,6,3);
    // matrix.add_edge_undir(1,5,6);
    // matrix.add_edge_undir(1,4,8);
    // matrix.add_edge_undir(1,7,7);
    // matrix.add_edge_undir(2,3,9);
    // matrix.add_edge_undir(2,4,4);
    // matrix.add_edge_undir(2,6,5);
    // matrix.add_edge_undir(2,7,3);
    // matrix.add_edge_undir(3,6,8);
    // matrix.add_edge_undir(4,5,2);
    // matrix.add_edge_undir(4,6,1);
    // matrix.add_edge_undir(5,6,6);
    // matrix.add_edge_undir(6,7,9);
    // matrix.print();
    // adjacency_matrix *mst_matrix = prim(matrix,0);
    // mst_matrix->print();
    // print_dijkstra(dijkstra(matrix,0),matrix.number_of_vertices);

    //example graph https://eduinf.waw.pl/inf/alg/001_search/0141.php#A2
    // adjacency_list list = adjacency_list(8);
    // // list.add_edge_undir(0,8,1);
    // list.add_edge_undir(0,1,5);
    // list.add_edge_undir(0,3,9);
    // list.add_edge_undir(0,6,3);
    // list.add_edge_undir(1,5,6);
    // list.add_edge_undir(1,4,8);
    // list.add_edge_undir(1,7,7);
    // list.add_edge_undir(2,3,9);
    // list.add_edge_undir(2,4,4);
    // list.add_edge_undir(2,6,5);
    // list.add_edge_undir(2,7,3);
    // list.add_edge_undir(3,6,8);
    // list.add_edge_undir(4,5,2);
    // list.add_edge_undir(4,6,1);
    // list.add_edge_undir(5,6,6);
    // list.add_edge_undir(6,7,9);
    // // list.print();
    // // adjacency_list *mst = prim(list,0);
    // // mst->print();
    // // int **dijkstra_arr = dijkstra(list, 0);
    // print_dijkstra(dijkstra(list, 0),list.number_of_vertices);

    // // incident_matrix inc = incident_matrix();
    // // inc.add_vertex();
    // // inc.add_vertex();
    // // inc.add_vertex();
    // // inc.add_vertex();
    // // inc.add_vertex();
    // // inc.add_vertex();
    // // inc.add_edge_undir(2,3);
    // // inc.add_edge_undir(3,2);
    // // inc.add_edge_undir(3,2);
    // // inc.add_edge_undir(3,5);
    // // inc.add_edge_undir(4,3);
    // // inc.add_edge_undir(1,3);
    // inc.print();
    save_results(results_file_name);
    cout << "Press any key to continue...";
    getch();
    return 0;
}
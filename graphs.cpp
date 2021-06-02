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
int number_of_current_graph_vertices = 0;
int number_of_current_graph_edges = 0;

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
        //find unvisited vertex with shortest path
        int min = INT_MAX;
        for (int j = 0; j < graph_list.number_of_vertices; j++)
        {
            if (paths_array[0][j] < min && !visited[j])
            {
                min = paths_array[0][j];
                v = j;
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
    for (int i = 0; i < graph_matrix.number_of_vertices-1; i++)
    {
        //find unvisited vertex with shortest path
        int min = INT_MAX;
        for (int j = 0; j < graph_matrix.number_of_vertices; j++)
        {
            if (paths_array[0][j] < min && !visited[j])
            {
                min = paths_array[0][j];
                v = j;
            }
        }
        for (int j = 0; j < graph_matrix.number_of_vertices; j++)
        { //v - source i - destination value - weight
            if (graph_matrix.matrix[v][j] != 0 && paths_array[0][v] + graph_matrix.matrix[v][j] < paths_array[0][j] && !visited[v])
            {
                paths_array[0][j] = paths_array[0][v] + graph_matrix.matrix[v][j];
                paths_array[1][j] = v;
            }
        }
        visited[v] = true;
    }
    return paths_array;
}


struct Result
{
    string structure;
    string operation;
    string graph_type;
    int number_of_vertices;
    int number_of_edges;
    double time_memory;
    int number_of_repeats;
    string test_type;
    Result(string structure, string graph_type, string operation, int number_of_vertices, int number_of_edges, double time_memory, int number_of_repeats, string test_type)
    {
        this->structure = structure;
        this->operation = operation;
        this->graph_type = graph_type;
        this->number_of_vertices = number_of_vertices;
        this->number_of_edges = number_of_edges;
        this->time_memory = time_memory;
        this->number_of_repeats = number_of_repeats;
        this->test_type = test_type;
    }
    string toString()
    {
        return (structure + "," + graph_type + "," + operation + "," + to_string(number_of_vertices) + "," + to_string(number_of_edges) + "," + to_string(time_memory) + "," + to_string(number_of_repeats) + "," + test_type);
    }
};

void save_results(string results_file_name)
{
    std::cout << "Saving results" << endl;
    fstream fout;
    fout.open(results_file_name, ios::out);
    fout << "structure,graph_type,operation,number_of_vertices,number_of_edges,time_memory,number_of_repeats,test_type" << endl;
    for (int i = 0; i < results.size(); i++)
    {
        fout << results[i] << endl;
    }
    fout.close();
    std::cout << "Correctly saved " << results.size() << " results" << endl;
}

bool load_data(string file_name)
{
    std::cout << "Loading data from " << file_name << " file" << endl;
    ifstream fin;
    fin.open(file_name, ios::in);
    if (!fin.good())
    {
        std::cout << "Data file " << file_name << " not exist" << endl;
        fin.close();
        return false;
    }
    string loaded_source, loaded_destination, loaded_weight;
    int edges_loaded = 0;
    number_of_current_graph_vertices = 0;
    number_of_current_graph_edges = 0;
    while (!fin.eof())
    {
        fin >> loaded_source >> loaded_destination >> loaded_weight;
        if (!fin.eof())
        {
            graph_data.push_back(node(stoi(loaded_source), stoi(loaded_destination), stoi(loaded_weight)));
            number_of_current_graph_edges += 1;
            if (number_of_current_graph_vertices < stoi(loaded_destination) + 1)
                number_of_current_graph_vertices = stoi(loaded_destination) + 1;
            edges_loaded++;
        }
    }
    std::cout << "Loaded correctly " << edges_loaded << " edges and " << number_of_current_graph_vertices << " vertices" << endl;
    fin.close();
    return true;
}

void load_config()
{
    std::cout << "Loading config.ini" << endl;
    ifstream fin;
    fin.open("config.ini", ios::in);
    if (!fin.good())
    {
        std::cout << "Config.ini not found" << endl;
        fin.close();
        return;
    }
    vector<string> row;
    string line;
    fin >> results_file_name;
    while (!fin.eof())
    {
        string file_name, structure, graph_type, operation, number_of_repeats, test_type;
        fin >> file_name >> structure >> graph_type >> operation >> number_of_repeats >> test_type;
        if (file_name.size() == 0 || structure.size() == 0 || graph_type.size() == 0 || operation.size() == 0 || number_of_repeats.size() == 0 || test_type.size() == 0)
        {
            std::cout << "Cannot load this task: " << structure << graph_type << operation << number_of_repeats << test_type;
            break;
        }
        vector<string> task;
        task.push_back(file_name);
        task.push_back(structure);
        task.push_back(graph_type);
        task.push_back(operation);
        task.push_back(number_of_repeats);
        task.push_back(test_type);
        tasks.push_back(task);
    }
    fin.close();
    std::cout << "Config loaded correctly" << endl;
    return;
}

int main()
{
    load_config();
    if (tasks.size() == 0)
    {
        std::cout << "No tasks found to be performed." << endl;
    }
    else
    {
        for (int i = 0; i < tasks.size(); i++)
        {
            std::cout << endl;
            string graph_file_name = tasks[i][0];
            string structure = tasks[i][1];
            string graph_type = tasks[i][2];
            string operation = tasks[i][3];
            int number_of_repeats = stoi(tasks[i][4]);
            string test_type = tasks[i][5];
            if (!load_data(graph_file_name))
            {
                std::cout << "Cannot load graph from " << data_file_name << " file." << endl;
            }
            std::cout << "Computing " << operation << " in " << structure << " graph with " << number_of_current_graph_edges << " edges and " << number_of_current_graph_vertices << " vertices repeated " << number_of_repeats << " times; " << test_type << " test" << endl;
            if (number_of_current_graph_edges < 1)
            {
                std::cout << "Cannot execute task. The array must to have at least 1 element.";
            }
            else if (number_of_repeats < 1)
            {
                std::cout << "Cannot execute task. The minimum number of repetitions is 1.";
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
                                high_resolution_clock::time_point t_start = high_resolution_clock::now();
                                high_resolution_clock::time_point t_end = high_resolution_clock::now();
                                duration<double> time_span = duration<double>(0);
                                adjacency_matrix current_graph = adjacency_matrix(number_of_current_graph_vertices);
                                for (int j = 0; graph_data[j].source < number_of_current_graph_edges && j < graph_data.size(); j++)
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
                                Result adjacency_matrix_dir_MST = Result(structure, graph_type, operation, number_of_current_graph_vertices, number_of_current_graph_edges, time_span.count(), number_of_repeats, test_type);
                                results.push_back(adjacency_matrix_dir_MST.toString());
                            }
                            else if (test_type == "memory")
                            {
                                int mem_size = 0;
                                for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                {
                                    mem_size += sizeof(int) * number_of_current_graph_vertices * number_of_current_graph_vertices; //original graph
                                    mem_size += sizeof(bool) * number_of_current_graph_vertices;                                   //visited array
                                    mem_size += sizeof(int) * number_of_current_graph_vertices * number_of_current_graph_vertices; //mst graph
                                    mem_size += sizeof(int);                                                                       //mst weight
                                    mem_size += sizeof(int);                                                                       //current vertex
                                    mem_size += sizeof(node) * number_of_current_graph_edges;                                      //max priority queue size
                                }
                                Result mem_test = Result(structure, graph_type, operation, number_of_current_graph_vertices, number_of_current_graph_edges, mem_size, number_of_repeats, test_type);
                                results.push_back(mem_test.toString());
                            }
                            else if (test_type == "print")
                            {
                                adjacency_matrix current_graph = adjacency_matrix(number_of_current_graph_vertices);
                                adjacency_matrix *mst = new adjacency_matrix();
                                for (int j = 0; graph_data[j].source < number_of_current_graph_edges && j < graph_data.size(); j++)
                                {
                                    current_graph.add_edge_dir(graph_data[j].source, graph_data[j].destination, graph_data[j].weight);
                                }
                                for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                {
                                    mst = prim(current_graph, 0);
                                    mst->print();
                                }
                            }
                            else
                            {
                                std::cout << "Cannot recognize " << test_type << " type of test.";
                            }
                        }
                        else if (operation == "shortest_paths")
                        {
                            if (test_type == "time")
                            {
                                high_resolution_clock::time_point t_start = high_resolution_clock::now();
                                high_resolution_clock::time_point t_end = high_resolution_clock::now();
                                duration<double> time_span = duration<double>(0);
                                adjacency_matrix current_graph = adjacency_matrix(number_of_current_graph_vertices);
                                for (int j = 0; graph_data[j].source < number_of_current_graph_edges && j < graph_data.size(); j++)
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
                                Result adjacency_matrix_dir_shortest_paths = Result(structure, graph_type, operation, number_of_current_graph_vertices, number_of_current_graph_edges, time_span.count(), number_of_repeats, test_type);
                                results.push_back(adjacency_matrix_dir_shortest_paths.toString());
                            }
                            else if (test_type == "memory")
                            {
                                int mem_size = 0;
                                for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                {
                                    mem_size += sizeof(int) * number_of_current_graph_vertices * number_of_current_graph_vertices; //original graph
                                    mem_size += sizeof(bool) * number_of_current_graph_vertices;                                   //visited array
                                    mem_size += sizeof(int) * 2 * number_of_current_graph_vertices;                                //result array
                                    mem_size += sizeof(int);                                                                       //current vertex
                                }
                                Result mem_test = Result(structure, graph_type, operation, number_of_current_graph_vertices, number_of_current_graph_edges, mem_size, number_of_repeats, test_type);
                                results.push_back(mem_test.toString());
                            }
                            else if (test_type == "print")
                            {
                                adjacency_matrix current_graph = adjacency_matrix(number_of_current_graph_vertices);
                                for (int j = 0; graph_data[j].source < number_of_current_graph_edges && j < graph_data.size(); j++)
                                {
                                    current_graph.add_edge_dir(graph_data[j].source, graph_data[j].destination, graph_data[j].weight);
                                }
                                for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                {
                                    print_dijkstra(dijkstra(current_graph, 0), number_of_current_graph_vertices);
                                }
                            }
                            else
                            {
                                std::cout << "Cannot recognize " << test_type << " type of test.";
                            }
                        }
                        else
                        {
                            std::cout << "Cannot recognize operation " << operation << " in " << structure << " structure.";
                        }
                    }
                    else if (graph_type == "undirected")
                    {
                        if (operation == "MST")
                        {
                            if (test_type == "time")
                            {
                                high_resolution_clock::time_point t_start = high_resolution_clock::now();
                                high_resolution_clock::time_point t_end = high_resolution_clock::now();
                                duration<double> time_span = duration<double>(0);
                                adjacency_matrix current_graph = adjacency_matrix(number_of_current_graph_vertices);
                                for (int j = 0; graph_data[j].source < number_of_current_graph_edges && j < graph_data.size(); j++)
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
                                Result adjacency_matrix_undir_MST = Result(structure, graph_type, operation, number_of_current_graph_vertices, number_of_current_graph_edges, time_span.count(), number_of_repeats, test_type);
                                results.push_back(adjacency_matrix_undir_MST.toString());
                            }
                            else if (test_type == "memory")
                            {
                                int mem_size = 0;
                                for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                {
                                    mem_size += sizeof(int) * number_of_current_graph_vertices * number_of_current_graph_vertices; //original graph
                                    mem_size += sizeof(bool) * number_of_current_graph_vertices;                                   //visited array
                                    mem_size += sizeof(int) * number_of_current_graph_vertices * number_of_current_graph_vertices; //mst graph
                                    mem_size += sizeof(int);                                                                       //mst weight
                                    mem_size += sizeof(int);                                                                       //current vertex
                                    mem_size += sizeof(node) * number_of_current_graph_edges;                                      //max priority queue size
                                }
                                Result mem_test = Result(structure, graph_type, operation, number_of_current_graph_vertices, number_of_current_graph_edges, mem_size, number_of_repeats, test_type);
                                results.push_back(mem_test.toString());
                            }
                            else if (test_type == "print")
                            {
                                adjacency_matrix current_graph = adjacency_matrix(number_of_current_graph_vertices);
                                adjacency_matrix *mst = new adjacency_matrix();
                                for (int j = 0; graph_data[j].source < number_of_current_graph_edges && j < graph_data.size(); j++)
                                {
                                    current_graph.add_edge_undir(graph_data[j].source, graph_data[j].destination, graph_data[j].weight);
                                }
                                for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                {
                                    mst = prim(current_graph, 0);
                                    mst->print();
                                }
                            }
                            else
                            {
                                std::cout << "Cannot recognize " << test_type << " type of test.";
                            }
                        }
                        else if (operation == "shortest_paths")
                        {
                            if (test_type == "time")
                            {
                                high_resolution_clock::time_point t_start = high_resolution_clock::now();
                                high_resolution_clock::time_point t_end = high_resolution_clock::now();
                                duration<double> time_span = duration<double>(0);
                                adjacency_matrix current_graph = adjacency_matrix(number_of_current_graph_vertices);
                                for (int j = 0; graph_data[j].source < number_of_current_graph_edges && j < graph_data.size(); j++)
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
                                Result adjacency_matrix_undir_shortest_paths = Result(structure, graph_type, operation, number_of_current_graph_vertices, number_of_current_graph_edges, time_span.count(), number_of_repeats, test_type);
                                results.push_back(adjacency_matrix_undir_shortest_paths.toString());
                            }
                            else if (test_type == "memory")
                            {
                                int mem_size = 0;
                                for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                {
                                    mem_size += sizeof(int) * number_of_current_graph_vertices * number_of_current_graph_vertices; //original graph
                                    mem_size += sizeof(bool) * number_of_current_graph_vertices;                                   //visited array
                                    mem_size += sizeof(int) * 2 * number_of_current_graph_vertices;                                //result array
                                    mem_size += sizeof(int);                                                                       //current vertex
                                }
                                Result mem_test = Result(structure, graph_type, operation, number_of_current_graph_vertices, number_of_current_graph_edges, mem_size, number_of_repeats, test_type);
                                results.push_back(mem_test.toString());
                            }
                            else if (test_type == "print")
                            {
                                adjacency_matrix current_graph = adjacency_matrix(number_of_current_graph_vertices);
                                for (int j = 0; graph_data[j].source < number_of_current_graph_edges && j < graph_data.size(); j++)
                                {
                                    current_graph.add_edge_undir(graph_data[j].source, graph_data[j].destination, graph_data[j].weight);
                                }
                                for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                {
                                    print_dijkstra(dijkstra(current_graph, 0), number_of_current_graph_vertices);
                                }
                            }
                            else
                            {
                                std::cout << "Cannot recognize " << test_type << " type of test.";
                            }
                        }
                        else
                        {
                            std::cout << "Cannot recognize operation " << operation << " in " << structure << " structure.";
                        }
                    }
                    else
                    {
                        std::cout << "Cannot recognize " << graph_type << " type of graph.";
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
                                high_resolution_clock::time_point t_start = high_resolution_clock::now();
                                high_resolution_clock::time_point t_end = high_resolution_clock::now();
                                duration<double> time_span = duration<double>(0);
                                adjacency_list current_graph = adjacency_list(number_of_current_graph_vertices);
                                for (int j = 0; graph_data[j].source < number_of_current_graph_edges && j < graph_data.size(); j++)
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
                                Result adjacency_list_dir_MST = Result(structure, graph_type, operation, number_of_current_graph_vertices, number_of_current_graph_edges, time_span.count(), number_of_repeats, test_type);
                                results.push_back(adjacency_list_dir_MST.toString());
                            }
                            else if (test_type == "memory")
                            {
                                int mem_size = 0;
                                for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                {
                                    mem_size += sizeof(adjacency_list::list_node) * number_of_current_graph_edges;          //original graph
                                    mem_size += 2 * sizeof(int) * number_of_current_graph_vertices;                         //array of pointers
                                    mem_size += sizeof(adjacency_list::list_node) * (number_of_current_graph_vertices - 1); //mst graph
                                    mem_size += sizeof(bool) * number_of_current_graph_vertices;                            //visited array
                                    mem_size += sizeof(int);                                                                //mst weight
                                    mem_size += sizeof(int);                                                                //current vertex
                                    mem_size += sizeof(node) * number_of_current_graph_edges;                               //max priority queue size
                                }
                                Result mem_test = Result(structure, graph_type, operation, number_of_current_graph_vertices, number_of_current_graph_edges, mem_size, number_of_repeats, test_type);
                                results.push_back(mem_test.toString());
                            }
                            else if (test_type == "print")
                            {
                                adjacency_list current_graph = adjacency_list(number_of_current_graph_vertices);
                                adjacency_list *mst = new adjacency_list();
                                for (int j = 0; graph_data[j].source < number_of_current_graph_edges && j < graph_data.size(); j++)
                                {
                                    current_graph.add_edge_dir(graph_data[j].source, graph_data[j].destination, graph_data[j].weight);
                                }
                                for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                {
                                    mst = prim(current_graph, 0);
                                    mst->print();
                                }
                            }
                            else
                            {
                                std::cout << "Cannot recognize " << test_type << " type of test.";
                            }
                        }
                        else if (operation == "shortest_paths")
                        {
                            if (test_type == "time")
                            {
                                high_resolution_clock::time_point t_start = high_resolution_clock::now();
                                high_resolution_clock::time_point t_end = high_resolution_clock::now();
                                duration<double> time_span = duration<double>(0);
                                adjacency_list current_graph = adjacency_list(number_of_current_graph_vertices);
                                for (int j = 0; graph_data[j].source < number_of_current_graph_edges && j < graph_data.size(); j++)
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
                                Result adjacency_list_dir_shortest_paths = Result(structure, graph_type, operation, number_of_current_graph_vertices, number_of_current_graph_edges, time_span.count(), number_of_repeats, test_type);
                                results.push_back(adjacency_list_dir_shortest_paths.toString());
                            }
                            else if (test_type == "memory")
                            {
                                int mem_size = 0;
                                for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                {
                                    mem_size += sizeof(adjacency_list::list_node) * number_of_current_graph_edges; //original graph
                                    mem_size += 2 * sizeof(int) * number_of_current_graph_vertices;                //array of pointers

                                    mem_size += sizeof(bool) * number_of_current_graph_vertices;    //visited array
                                    mem_size += sizeof(int) * 2 * number_of_current_graph_vertices; //result array
                                    mem_size += sizeof(int);                                        //current vertex
                                }
                                Result mem_test = Result(structure, graph_type, operation, number_of_current_graph_vertices, number_of_current_graph_edges, mem_size, number_of_repeats, test_type);
                                results.push_back(mem_test.toString());
                            }
                            else if (test_type == "print")
                            {
                                adjacency_matrix current_graph = adjacency_matrix(number_of_current_graph_vertices);
                                for (int j = 0; graph_data[j].source < number_of_current_graph_edges && j < graph_data.size(); j++)
                                {
                                    current_graph.add_edge_dir(graph_data[j].source, graph_data[j].destination, graph_data[j].weight);
                                }
                                for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                {
                                    print_dijkstra(dijkstra(current_graph, 0), number_of_current_graph_vertices);
                                }
                            }
                            else
                            {
                                std::cout << "Cannot recognize " << test_type << " type of test.";
                            }
                        }
                        else
                        {
                            std::cout << "Cannot recognize operation " << operation << " in " << structure << " structure.";
                        }
                    }
                    else if (graph_type == "undirected")
                    {
                        if (operation == "MST")
                        {
                            if (test_type == "time")
                            {
                                high_resolution_clock::time_point t_start = high_resolution_clock::now();
                                high_resolution_clock::time_point t_end = high_resolution_clock::now();
                                duration<double> time_span = duration<double>(0);
                                adjacency_list current_graph = adjacency_list(number_of_current_graph_vertices);
                                for (int j = 0; graph_data[j].source < number_of_current_graph_edges && j < graph_data.size(); j++)
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
                                Result adjacency_list_undir_MST = Result(structure, graph_type, operation, number_of_current_graph_vertices, number_of_current_graph_edges, time_span.count(), number_of_repeats, test_type);
                                results.push_back(adjacency_list_undir_MST.toString());
                            }
                            else if (test_type == "memory")
                            {
                                int mem_size = 0;
                                for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                {
                                    mem_size += sizeof(adjacency_list::list_node) * number_of_current_graph_edges;          //original graph
                                    mem_size += 2 * sizeof(int) * number_of_current_graph_vertices;                         //array of pointers
                                    mem_size += sizeof(adjacency_list::list_node) * (number_of_current_graph_vertices - 1); //mst graph
                                    mem_size += sizeof(bool) * number_of_current_graph_vertices;                            //visited array
                                    mem_size += sizeof(int);                                                                //mst weight
                                    mem_size += sizeof(int);                                                                //current vertex
                                    mem_size += sizeof(node) * number_of_current_graph_edges;                               //max priority queue size
                                }
                                Result mem_test = Result(structure, graph_type, operation, number_of_current_graph_vertices, number_of_current_graph_edges, mem_size, number_of_repeats, test_type);
                                results.push_back(mem_test.toString());
                            }
                            else if (test_type == "print")
                            {
                                adjacency_list current_graph = adjacency_list(number_of_current_graph_vertices);
                                adjacency_list *mst = new adjacency_list();
                                for (int j = 0; graph_data[j].source < number_of_current_graph_edges && j < graph_data.size(); j++)
                                {
                                    current_graph.add_edge_undir(graph_data[j].source, graph_data[j].destination, graph_data[j].weight);
                                }
                                for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                {
                                    mst = prim(current_graph, 0);
                                    mst->print();
                                }
                            }
                            else
                            {
                                std::cout << "Cannot recognize " << test_type << " type of test.";
                            }
                        }
                        else if (operation == "shortest_paths")
                        {
                            if (test_type == "time")
                            {
                                high_resolution_clock::time_point t_start = high_resolution_clock::now();
                                high_resolution_clock::time_point t_end = high_resolution_clock::now();
                                duration<double> time_span = duration<double>(0);
                                adjacency_list current_graph = adjacency_list(number_of_current_graph_vertices);
                                for (int j = 0; graph_data[j].source < number_of_current_graph_edges && j < graph_data.size(); j++)
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
                                Result adjacency_list_undir_shortest_paths = Result(structure, graph_type, operation, number_of_current_graph_vertices, number_of_current_graph_edges, time_span.count(), number_of_repeats, test_type);
                                results.push_back(adjacency_list_undir_shortest_paths.toString());
                            }
                            else if (test_type == "memory")
                            {
                                int mem_size = 0;
                                for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                {
                                    mem_size += sizeof(adjacency_list::list_node) * number_of_current_graph_edges; //original graph
                                    mem_size += 2 * sizeof(int) * number_of_current_graph_vertices;                //array of pointers
                                    mem_size += sizeof(bool) * number_of_current_graph_vertices;                   //visited array
                                    mem_size += sizeof(int) * 2 * number_of_current_graph_vertices;                //result array
                                    mem_size += sizeof(int);                                                       //current vertex
                                }
                                Result mem_test = Result(structure, graph_type, operation, number_of_current_graph_vertices, number_of_current_graph_edges, mem_size, number_of_repeats, test_type);
                                results.push_back(mem_test.toString());
                            }
                            else if (test_type == "print")
                            {
                                adjacency_list current_graph = adjacency_list(number_of_current_graph_vertices);
                                for (int j = 0; graph_data[j].source < number_of_current_graph_edges && j < graph_data.size(); j++)
                                {
                                    current_graph.add_edge_undir(graph_data[j].source, graph_data[j].destination, graph_data[j].weight);
                                }
                                for (int repeat = 0; repeat < number_of_repeats; repeat++)
                                {
                                    print_dijkstra(dijkstra(current_graph, 0), number_of_current_graph_vertices);
                                }
                            }
                            else
                            {
                                std::cout << "Cannot recognize " << test_type << " type of test.";
                            }
                        }
                        else
                        {
                            std::cout << "Cannot recognize operation " << operation << " in " << structure << " structure.";
                        }
                    }
                    else
                    {
                        std::cout << "Cannot recognize " << graph_type << " type of graph.";
                    }
                }
                else
                {
                    std::cout << "Cannot recognize " << structure << " structure.";
                }
            }
        }
    }
    std::cout << endl;
    save_results(results_file_name);
    std::cout << "Press any key to continue...";
    getch();
    return 0;
}
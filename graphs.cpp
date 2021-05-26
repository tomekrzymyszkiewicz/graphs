#include<iostream>
#include<string.h>
#include<fstream>
#include<stdio.h>
#include<queue>
#include<vector>
#include<windows.h>
#include"structures.h"
using namespace std;
vector<vector<string>> tasks;
vector<string> results;
string data_file_name = "";
string results_file_name = "";
vector<node> graph_data = vector<node>();

adjacency_matrix* prim(adjacency_matrix graph_matrix, int start_vertex){
    priority_queue<node, vector<node>, node::compare> pq;
    bool *visited = new bool[graph_matrix.number_of_vertices];
    for(int i = 0; i < graph_matrix.number_of_vertices; i++)
        visited[i] = false;
    adjacency_matrix *MST = new adjacency_matrix(graph_matrix.number_of_vertices);
    int MST_weight = 0;
    int v = start_vertex;
    visited[v] = true;
    pq = priority_queue<node, vector<node>, node::compare>();
    for(int i = 0; i < graph_matrix.number_of_vertices-1; i++){
        for(int j = 0; j < graph_matrix.number_of_vertices; j++){
            if(graph_matrix.matrix[v][j] != 0 && !visited[j]){
                pq.push(node(v,j,graph_matrix.matrix[v][j]));
            }
        }
        visited[v] = true;
        while(visited[pq.top().destination]){
            pq.pop();
        }
        MST->add_edge_undir(pq.top().source,pq.top().destination,pq.top().weight);
        MST_weight += pq.top().weight;
        v = pq.top().destination;
        pq.pop();
    }
    return MST;
}

adjacency_list* prim(adjacency_list graph_list, int start_vertex){
    priority_queue<node, vector<node>, node::compare> pq;
    bool *visited = new bool[graph_list.number_of_vertices];
    for(int i = 0; i < graph_list.number_of_vertices; i++)
        visited[i] = false;
    adjacency_list *MST = new adjacency_list(graph_list.number_of_vertices);
    int MST_weight = 0;
    int v = start_vertex;
    visited[v] = true;
    pq = priority_queue<node, vector<node>, node::compare>();
    for(int i = 0; i < graph_list.number_of_vertices-1; i++){
        for(adjacency_list::list_node *current_node = graph_list.array_of_lists[v]; current_node != nullptr; current_node = current_node->next){
            if(!visited[current_node->dst])
                pq.push(node(v,current_node->dst,current_node->weight));
        }
        visited[v] = true;
        while(visited[pq.top().destination]){
            pq.pop();
        }
        MST->add_edge_undir(pq.top().source,pq.top().destination,pq.top().weight);
        MST_weight += pq.top().weight;
        v = pq.top().destination;
        pq.pop();

    }
    return MST;
}

int** dijkstra(adjacency_list graph_list, int start_vertex){
    int **paths_array = new int*[2];
    paths_array[0] = new int[graph_list.number_of_vertices]; //array of distances
    paths_array[1] = new int[graph_list.number_of_vertices]; //array of predecessors
    bool *visited = new bool[graph_list.number_of_vertices];
    for(int i = 0; i < graph_list.number_of_vertices; i++){
        paths_array[0][i] = INT_MAX;
        paths_array[1][i] = -1;
        visited[i] = false;
    }
    paths_array[0][start_vertex] = 0;
    paths_array[1][start_vertex] = start_vertex;
    int v = start_vertex; //current vertex in algorithm
    for(int i = 0; i < graph_list.number_of_vertices; i++){
        //find unvisited vertex with shortes path
        int min = INT_MAX;
        for(int i = 0; i < graph_list.number_of_vertices; i++){
            if(paths_array[0][i] < min && !visited[i]){
                min = paths_array[0][i];
                v = i;
            }
        }
        for(adjacency_list::list_node *current_node = graph_list.array_of_lists[v]; current_node != nullptr; current_node = current_node->next){
            if(current_node && paths_array[0][v]+current_node->weight < paths_array[0][current_node->dst] && !visited[v]){
                paths_array[0][current_node->dst] = paths_array[0][v] + current_node->weight;
                paths_array[1][current_node->dst] = v;
            }
        }
        visited[v] = true;
        }
    return paths_array;
}

int** dijkstra(adjacency_matrix graph_matrix, int start_vertex){
    int **paths_array = new int*[2];
    paths_array[0] = new int[graph_matrix.number_of_vertices]; //array of distances
    paths_array[1] = new int[graph_matrix.number_of_vertices]; //array of predecessors
    bool *visited = new bool[graph_matrix.number_of_vertices];
    for(int i = 0; i < graph_matrix.number_of_vertices; i++){
        paths_array[0][i] = INT_MAX;
        paths_array[1][i] = -1;
        visited[i] = false;
    }
    paths_array[0][start_vertex] = 0;
    paths_array[1][start_vertex] = start_vertex;
    int v = start_vertex; //current vertex in algorithm
    for(int i = 0; i < graph_matrix.number_of_vertices; i++){
        //find unvisited vertex with shortes path
        int min = INT_MAX;
        for(int i = 0; i < graph_matrix.number_of_vertices; i++){
            if(paths_array[0][i] < min && !visited[i]){
                min = paths_array[0][i];
                v = i;
            }
        }
        for(int i = 0; i < graph_matrix.number_of_vertices; i++){ //v - source i - destination value - weight
            if(graph_matrix.matrix[v][i] != 0 && paths_array[0][v]+graph_matrix.matrix[v][i] < paths_array[0][i] && !visited[v]){
                paths_array[0][i] = paths_array[0][v] + graph_matrix.matrix[v][i];
                paths_array[1][i] = v;
            }
        }
        visited[v] = true;
        }
    return paths_array;
}

void print_dijkstra(int **result_array, int number_of_vertices){
    for(int i = 0; i < (((number_of_vertices*3)-1)/2); i ++)
        printf("=");
    printf("SHORTEST PATHS");
    for(int i = 0; i < (((number_of_vertices*3)-1)/2); i ++)
        printf("=");
    printf("\nINDEX       |");
    for(int i = 0; i < number_of_vertices; i ++)
        printf("%3d",i);
    printf("\nDISTANCE    |");
    for(int i = 0; i < number_of_vertices; i ++){
        if(result_array[0][i] == INT_MAX)
           printf(" in");
        else
            printf("%3d",result_array[0][i]);
    }
    printf("\nPREDECESSOR |");
    for(int i = 0; i < number_of_vertices; i ++)
        printf("%3d",result_array[1][i]);
    printf("\n");
}

vector<node> load_data(){

}

void load_config(){
    cout<<"Loading config.ini"<<endl;
    ifstream fin;
    fin.open("config.ini",ios::in);
    if(!fin.good()){
        cout<<"Config.ini not found"<<endl;
        fin.close();
        return;
    }
    vector<string> row;
    string line;
    fin >> data_file_name;
    fin >> results_file_name;
    while(!fin.eof()){
        string structure, operation, min_size, max_size, step, number_of_repeats;
        fin>>structure>>operation>>min_size>>max_size>>step>>number_of_repeats;
        if(structure.size() == 0 || operation.size() == 0 || min_size.size() == 0 || max_size.size() == 0 || step.size() == 0 || number_of_repeats.size() == 0){
            break;
        }
        vector<string> task;
        task.push_back(structure);
        task.push_back(operation);
        task.push_back(min_size);
        task.push_back(max_size);
        task.push_back(step);
        task.push_back(number_of_repeats);
        tasks.push_back(task);
    }
    fin.close();
    cout<<"Config loaded correctly"<<endl;
    return;
}

int main(){
    //example graph https://eduinf.waw.pl/inf/alg/001_search/0138.php
    adjacency_matrix matrix1 = adjacency_matrix(6);
    matrix1.add_edge_dir(0,1,3);
    matrix1.add_edge_dir(0,4,3);
    matrix1.add_edge_dir(1,2,1);
    matrix1.add_edge_dir(2,3,3);
    matrix1.add_edge_dir(2,5,1);
    matrix1.add_edge_dir(3,1,3);
    matrix1.add_edge_dir(4,5,2);
    matrix1.add_edge_dir(5,0,6);
    matrix1.add_edge_dir(5,3,1);
    // print_dijkstra(dijkstra(matrix1,0),matrix1.number_of_vertices);

    //example graph https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
    adjacency_matrix matrix2 = adjacency_matrix(9);
    matrix2.add_edge_undir(0,1,4);
    matrix2.add_edge_undir(0,7,8);
    matrix2.add_edge_undir(1,7,11);
    matrix2.add_edge_undir(1,2,8);
    matrix2.add_edge_undir(2,8,2);
    matrix2.add_edge_undir(2,5,4);
    matrix2.add_edge_undir(2,3,7);
    matrix2.add_edge_undir(3,5,14);
    matrix2.add_edge_undir(3,4,9);
    matrix2.add_edge_undir(4,5,10);
    matrix2.add_edge_undir(5,6,2);
    matrix2.add_edge_undir(6,7,1);
    matrix2.add_edge_undir(6,8,6);
    matrix2.add_edge_undir(7,8,7);
    // print_dijkstra(dijkstra(matrix2,0),matrix2.number_of_vertices);

    //example graph https://eduinf.waw.pl/inf/alg/001_search/0141.php#A2
    adjacency_matrix matrix = adjacency_matrix(10);
    // matrix.add_edge_undir(0,8,1);
    matrix.add_edge_undir(0,1,5);
    matrix.add_edge_undir(0,3,9);
    matrix.add_edge_undir(0,6,3);
    matrix.add_edge_undir(1,5,6);
    matrix.add_edge_undir(1,4,8);
    matrix.add_edge_undir(1,7,7);
    matrix.add_edge_undir(2,3,9);
    matrix.add_edge_undir(2,4,4);
    matrix.add_edge_undir(2,6,5);
    matrix.add_edge_undir(2,7,3);
    matrix.add_edge_undir(3,6,8);
    matrix.add_edge_undir(4,5,2);
    matrix.add_edge_undir(4,6,1);
    matrix.add_edge_undir(5,6,6);
    matrix.add_edge_undir(6,7,9);
    // matrix.print();
    // adjacency_matrix *mst_matrix = prim(matrix,0);
    // mst_matrix->print();
    print_dijkstra(dijkstra(matrix,0),matrix.number_of_vertices);


    //example graph https://eduinf.waw.pl/inf/alg/001_search/0141.php#A2
    adjacency_list list = adjacency_list(8);
    // list.add_edge_undir(0,8,1);
    list.add_edge_undir(0,1,5);
    list.add_edge_undir(0,3,9);
    list.add_edge_undir(0,6,3);
    list.add_edge_undir(1,5,6);
    list.add_edge_undir(1,4,8);
    list.add_edge_undir(1,7,7);
    list.add_edge_undir(2,3,9);
    list.add_edge_undir(2,4,4);
    list.add_edge_undir(2,6,5);
    list.add_edge_undir(2,7,3);
    list.add_edge_undir(3,6,8);
    list.add_edge_undir(4,5,2);
    list.add_edge_undir(4,6,1);
    list.add_edge_undir(5,6,6);
    list.add_edge_undir(6,7,9);
    // list.print();
    // adjacency_list *mst = prim(list,0);
    // mst->print();
    // int **dijkstra_arr = dijkstra(list, 0);
    print_dijkstra(dijkstra(list, 0),list.number_of_vertices);
   
    // incident_matrix inc = incident_matrix();
    // inc.add_vertex();
    // inc.add_vertex();
    // inc.add_vertex();
    // inc.add_vertex();
    // inc.add_vertex();
    // inc.add_vertex();
    // inc.add_edge_undir(2,3);
    // inc.add_edge_undir(3,2);
    // inc.add_edge_undir(3,2);
    // inc.add_edge_undir(3,5);
    // inc.add_edge_undir(4,3);
    // inc.add_edge_undir(1,3);
    // inc.print();

    return 0;
}
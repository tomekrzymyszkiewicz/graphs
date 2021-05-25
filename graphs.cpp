#include<iostream>
#include<string.h>
#include<stdio.h>
#include<queue>
#include<vector>
#include"structures.h"
using namespace std;


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
    // priority_queue<node, vector<node>, node::compare> pq = priority_queue<node, vector<node>, node::compare>();
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
            if(current_node && paths_array[0][v]+current_node->weight < paths_array[0][current_node->dst]){
                paths_array[0][current_node->dst] = paths_array[0][v] + current_node->weight;
                paths_array[1][current_node->dst] = v;
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
    for(int i = 0; i < number_of_vertices; i ++)
        printf("%3d",result_array[0][i]);
    printf("\nPREDECESSOR |");
    for(int i = 0; i < number_of_vertices; i ++)
        printf("%3d",result_array[1][i]);
}


int main(){
    //example graph https://eduinf.waw.pl/inf/alg/001_search/0141.php#A2
    // adjacency_matrix matrix = adjacency_matrix(9);
    // printf("======\n");
    // matrix.add_edge_undir(0,8,1);
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
    list.print();
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
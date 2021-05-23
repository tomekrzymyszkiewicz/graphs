#include<iostream>
#include<string.h>
#include<stdio.h>
#include<queue>
#include<vector>
using namespace std;

struct adjacency_matrix{
    int **matrix;
    int number_of_vertices;
    adjacency_matrix(){
        matrix = new int*[0];
        matrix[0] = new int[0];
        number_of_vertices = 0;
    };
    adjacency_matrix(int number_of_vertices){
        matrix = new int*[number_of_vertices];
        for(int i = 0; i < number_of_vertices; i++){
            matrix[i] = new int[number_of_vertices];
            memset(matrix[i],0,number_of_vertices*sizeof(int));
        }
        this->number_of_vertices = number_of_vertices;
    };
    void print(){
        printf("   |");
        for(int i = 0; i < this->number_of_vertices; i++){
            printf("%3d ", i);
        }
        printf("\n----");
        for(int i = 0; i < this->number_of_vertices; i++){
            printf("----", i);
        }
        printf("\n");
        for(int i = 0; i < this->number_of_vertices; i++){
            printf("%3d|",i);
            for(int j = 0; j < this->number_of_vertices; j++){
                printf("%3d ",this->matrix[i][j]); //matrix[row][col]
            }
            printf("\n");
        }
    }
    void add_vertex(){
        this->number_of_vertices = this->number_of_vertices+1;
        int **temp_matrix = new int*[this->number_of_vertices];
        memcpy(temp_matrix,this->matrix,(this->number_of_vertices-1)*sizeof(int));
        for(int i = 0; i < this->number_of_vertices-1; i++){
            temp_matrix[i] = new int[number_of_vertices];
            memcpy(temp_matrix[i],this->matrix[i],(this->number_of_vertices-1)*sizeof(int));
            temp_matrix[i][number_of_vertices-1] = 0;
        }
        temp_matrix[number_of_vertices-1] = new int[number_of_vertices];
        memset(temp_matrix[number_of_vertices-1],0,number_of_vertices*sizeof(int));
        delete[] this->matrix;
        this->matrix = temp_matrix;
    };
    bool add_edge_dir(int src_vertex, int dst_vertex, int weight = 1){
        if(src_vertex < this->number_of_vertices && dst_vertex < this->number_of_vertices && src_vertex >= 0 && dst_vertex >= 0){
            this->matrix[src_vertex][dst_vertex] = weight;
            return true;
        }else{
            return false;
        }
    };
    bool add_edge_undir(int src_vertex, int dst_vertex, int weight = 1){
        if(src_vertex < this->number_of_vertices && dst_vertex < this->number_of_vertices && src_vertex >= 0 && dst_vertex >= 0){
            this->matrix[src_vertex][dst_vertex] = weight;
            this->matrix[dst_vertex][src_vertex] = weight;
            return true;
        }else{
            return false;
        }
    };
};

struct adjacency_list{
    struct node{
        int dst;
        int weight;
        node *next;
        node(){
            dst = 0;
            weight = 0;
            next = nullptr;
        };
        node(int dst, int weight, node *next = nullptr){
            this->dst = dst;
            this->weight = weight;
            this->next = next;
        }
    };
    struct node_compare{
        bool operator()(const node& a, node& b){
            return a.weight>b.weight;
        }
    };
    node **array_of_lists;
    int number_of_vertices;
    adjacency_list(){
        number_of_vertices = 0;
        array_of_lists = new node*[0];
    };
    adjacency_list(int number_of_vertices){
        this->number_of_vertices = number_of_vertices;
        array_of_lists = new node*[number_of_vertices];
        for(int i = 0; i < number_of_vertices; i++){
            array_of_lists[i] = nullptr;
        }
    };
    void print(){
        for(int i = 0; i < this->number_of_vertices; i++){
            printf("V %3d ->",i);
            node *current_node = array_of_lists[i];
            while(current_node){
                printf(" {V: %3d|W: %3d},",current_node->dst,current_node->weight);
                current_node = current_node->next;
            }
            printf("\n");
        }  
    };
    void add_vertex(){
        this->number_of_vertices++;
        node **temp_array_of_lists = new node*[this->number_of_vertices];
        for(int i = 0; i < this->number_of_vertices; i++){
            temp_array_of_lists[i] = this->array_of_lists[i];
        }
        temp_array_of_lists[this->number_of_vertices-1] = nullptr;
        delete[] this->array_of_lists;
        this->array_of_lists = temp_array_of_lists;
    };
    bool add_edge_dir(int src_vertex, int dst_vertex, int weight = 1){
        if(src_vertex < this->number_of_vertices && dst_vertex < this->number_of_vertices && src_vertex >= 0 && dst_vertex >= 0){
            node*& source_node = this->array_of_lists[src_vertex]; //reference to pointer
            if(source_node == nullptr){
                source_node = new node(dst_vertex,weight);
            }else{
                node* current_node = this->array_of_lists[src_vertex];
                while(current_node->next != nullptr){
                    current_node = current_node->next;
                }
                current_node->next = new node(dst_vertex,weight);
            }
            return true;
        }else{
            return false;
        }
    };
    bool add_edge_undir(int src_vertex, int dst_vertex, int weight = 1){
        if(src_vertex < this->number_of_vertices && dst_vertex < this->number_of_vertices && src_vertex >= 0 && dst_vertex >= 0){
            node*& source_node = this->array_of_lists[src_vertex]; //reference to pointer
            if(source_node == nullptr){
                source_node = new node(dst_vertex,weight);
            }else{
                node* current_node = this->array_of_lists[src_vertex];
                while(current_node->next != nullptr){
                    current_node = current_node->next;
                }
                current_node->next = new node(dst_vertex,weight);
            }
            node*& second_direction_source_node = this->array_of_lists[dst_vertex]; //reference to pointer
            if(second_direction_source_node == nullptr){
                second_direction_source_node = new node(src_vertex,weight);
            }else{
                node* current_node = this->array_of_lists[dst_vertex];
                while(current_node->next != nullptr){
                    current_node = current_node->next;
                }
                current_node->next = new node(src_vertex,weight);
            }
            return true;
        }else{
            return false;
        }
    };
};

struct incident_matrix{
    int **matrix;
    int number_of_vertices;
    int number_of_edges;
    incident_matrix(){
        matrix = new int*[0];
        matrix[0] = new int[0];
        number_of_vertices = 0;
        number_of_edges = 0;
    };
    incident_matrix(int number_of_vertices, int number_of_edges){
        matrix = new int*[number_of_edges];
        for(int i = 0; i < number_of_edges; i++){
            matrix[i] = new int[number_of_vertices];
            memset(matrix[i],0,number_of_vertices*sizeof(int)); //filled with 0
        }
        this->number_of_vertices = number_of_vertices;
        this->number_of_edges = number_of_edges;
    };
    void print(){
        printf("   |");
        for(int i = 0; i < this->number_of_edges; i++){
            int source = 0;
            int destination = 0;
            bool directed = false;
            for(int j = 0; j < this->number_of_vertices; j++){
                if(this->matrix[i][j] == 1){
                    source = j;
                    break;
                }
            }
            for(int j = 0; j < this->number_of_vertices; j++){
                if(this->matrix[i][j] == -1){
                    destination = j;
                    directed = true;
                    break;
                }
            }
            if(!directed){
                for(int j = 0; j < this->number_of_vertices; j++){
                if(this->matrix[i][j] == 1 && j != source){
                    destination = j;
                    break;
                }
            }
            }
            printf("%2d-%2d ", source, destination);
        }
        printf("\n----");
        for(int i = 0; i < this->number_of_edges; i++){
            printf("------", i);
        }
        printf("\n");
        for(int i = 0; i < this->number_of_vertices; i++){
            printf("%3d|",i);
            for(int j = 0; j < this->number_of_edges; j++){
                printf(" %4d ",this->matrix[j][i]);
            }
            printf("\n");
        }
    };
    void add_vertex(){
        this->number_of_vertices = this->number_of_vertices+1;
        int **temp_matrix = new int*[this->number_of_edges];
        for(int i = 0; i < this->number_of_edges; i++){
            temp_matrix[i] = new int[this->number_of_vertices];
            memcpy(temp_matrix[i],this->matrix[i],(this->number_of_vertices-1)*sizeof(int));
            temp_matrix[i][this->number_of_vertices-1] = 0;
        }
        if(this->number_of_vertices>1)
            delete[] this->matrix;
        this->matrix = temp_matrix;
    };
    bool add_edge_dir(int src_vertex, int dst_vertex){
        if(src_vertex < this->number_of_vertices && dst_vertex < this->number_of_vertices && src_vertex >= 0 && dst_vertex >= 0){
            for(int i = 0; i < this->number_of_edges-1; i++){
                int source = 0;
                for(int j = 0; j < this->number_of_vertices; j++){
                    if(this->matrix[i][j] == 1)
                       source = j;
                }
                int destination = source;
                for(int j = 0; j < this->number_of_vertices; j++){
                    if(this->matrix[i][j] == -1)
                       destination = j;
                }
                if(source == src_vertex && destination == dst_vertex){
                    return true; //edge already exist
                }
            }
            this->number_of_edges = this->number_of_edges+1;
            int **temp_matrix = new int*[this->number_of_edges];
            for(int i = 0; i < this->number_of_edges-1; i++){
                temp_matrix[i] = new int[this->number_of_vertices];
                memcpy(temp_matrix[i],this->matrix[i],this->number_of_vertices*sizeof(int));
            }
            temp_matrix[this->number_of_edges-1] = new int[this->number_of_vertices];
            memset(temp_matrix[this->number_of_edges-1],0,this->number_of_vertices*sizeof(int));
            temp_matrix[this->number_of_edges-1][src_vertex] = 1;
            temp_matrix[this->number_of_edges-1][dst_vertex] = -1;
            delete[] this->matrix;
            this->matrix = temp_matrix;
            return true;
        }else{
            return false;
        }
    };
    bool add_edge_undir(int src_vertex, int dst_vertex){
        if(src_vertex < this->number_of_vertices && dst_vertex < this->number_of_vertices && src_vertex >= 0 && dst_vertex >= 0){
            for(int i = 0; i < this->number_of_edges; i++){
                int source = 0;
                int destination = 0;
                for(int j = 0; j < this->number_of_vertices; j++){
                    if(this->matrix[i][j] == 1){
                        source = j;
                        break;
                    }
                }
                for(int j = 0; j < this->number_of_vertices; j++){
                    if(this->matrix[i][j] == 1 && j != source){
                        destination = j;
                        break;
                    }
                }
                if(source == src_vertex && destination == dst_vertex || source == dst_vertex && destination == src_vertex){
                    return true; //edge already exist
                }
            }
            this->number_of_edges = this->number_of_edges+1;
            int **temp_matrix = new int*[this->number_of_edges];
            for(int i = 0; i < this->number_of_edges-1; i++){
                temp_matrix[i] = new int[this->number_of_vertices];
                memcpy(temp_matrix[i],this->matrix[i],this->number_of_vertices*sizeof(int));
            }
            temp_matrix[this->number_of_edges-1] = new int[this->number_of_vertices];
            memset(temp_matrix[this->number_of_edges-1],0,this->number_of_vertices*sizeof(int));
            temp_matrix[this->number_of_edges-1][src_vertex] = 1;
            temp_matrix[this->number_of_edges-1][dst_vertex] = 1;
            delete[] this->matrix;
            this->matrix = temp_matrix;
            return true;
        }else{
            return false;
        }
    };
};

adjacency_matrix* prim(adjacency_matrix graph_matrix, int start_vertex){
    
    struct node{
        int source;
        int destination;
        int weight;
        node(int source, int destination, int weight){
            this->source = source;
            this->destination = destination;
            this->weight = weight;
        };
        struct compare{
            bool operator()(const node& a, node& b){
                return a.weight>b.weight;
            }
        };
    };
    priority_queue<node, vector<node>, node::compare> pq;
    bool *visited = new bool[graph_matrix.number_of_vertices];
    for(int i = 0; i < graph_matrix.number_of_vertices; i++)
        visited[i] = false;
    adjacency_matrix *MST = new adjacency_matrix(graph_matrix.number_of_vertices);
    int MST_weight = 0;
    int v = start_vertex;
    // int *node = new int[3]; // 0 - src 1 - dsc 2 - weight
    visited[v] = true;
    for(int i = 0; i < graph_matrix.number_of_vertices-1; i++){
        pq = priority_queue<node, vector<node>, node::compare>();
        int current_vertex = graph_matrix.matrix[v][0];
        for(int j = 0; j < graph_matrix.number_of_vertices; j++){
            if(graph_matrix.matrix[v][j] != 0 && !visited[j]){
                // node[0] = v;
                // node[1] = j;
                // node[2] = graph_matrix.matrix[v][j];
                pq.push(node(v,j,graph_matrix.matrix[v][j]));
            }
        }
        visited[v] = true;
        MST->add_edge_undir(v,pq.top().destination,pq.top().weight);
        MST_weight += pq.top().weight;
        v = pq.top().destination;
        // MST->add_edge_undir(v,pq.top()[1],pq.top()[2]);
        // MST_weight += pq.top()[2];
        // v = pq.top()[1];
    }
    return MST;
}

adjacency_list* prim(adjacency_list graph_list, int start_vertex){
    priority_queue<adjacency_list::node, vector<adjacency_list::node>, adjacency_list::node_compare> pq;
    bool *visited = new bool[graph_list.number_of_vertices];
    for(int i = 0; i < graph_list.number_of_vertices; i++)
        visited[i] = false;
    adjacency_list *MST = new adjacency_list(graph_list.number_of_vertices);
    int MST_weight = 0;
    int v = start_vertex;
    visited[v] = true;
    for(int i = 0; i < graph_list.number_of_vertices-1; i++){
        pq = priority_queue<adjacency_list::node, vector<adjacency_list::node>, adjacency_list::node_compare>();
        for(adjacency_list::node *current_node = graph_list.array_of_lists[v]; current_node != nullptr; current_node = current_node->next){
            if(!visited[current_node->dst])
                pq.push(*current_node);
        }
        visited[v] = true;
        MST->add_edge_undir(v,pq.top().dst,pq.top().weight);
        MST_weight += pq.top().weight;
        v = pq.top().dst;
    }
    return MST;
}

int main(){
    //example graph https://eduinf.waw.pl/inf/alg/001_search/0141.php#A2
    adjacency_matrix matrix = adjacency_matrix(8);
    printf("======\n");
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
    matrix.print();
    adjacency_matrix *mst_matrix = prim(matrix,0);
    mst_matrix->print();


    //example graph https://eduinf.waw.pl/inf/alg/001_search/0141.php#A2
    // adjacency_list list = adjacency_list(8);
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
    // list.print();
    // adjacency_list *mst = prim(list,0);
    // mst->print();


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
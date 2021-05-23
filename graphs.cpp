#include<iostream>
#include<string.h>
#include<stdio.h>
#include<list>
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
                printf("%3d ",this->matrix[i][j]);
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
    bool add_edge(int src_vertex, int dst_vertex, int weight = 1){
        if(src_vertex < this->number_of_vertices && dst_vertex < this->number_of_vertices && src_vertex >= 0 && dst_vertex >= 0){
            this->matrix[src_vertex][dst_vertex] = weight;
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
        // memcpy(temp_array_of_lists,this->array_of_lists,(this->number_of_vertices-1)*sizeof(this->array_of_lists[0]));
        delete[] this->array_of_lists;
        this->array_of_lists = temp_array_of_lists;
    };
    bool add_edge(int src_vertex, int dst_vertex, int weight = 1){
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
};

struct incident_matrix{
    int ***matrix;
    int number_of_vertices;
    int number_of_edges;
    incident_matrix(){
        matrix = new int**[0];
        matrix[0] = new int*[3];
        matrix[0][0] = new int[0];
        matrix[0][1] = new int[0];
        matrix[0][2] = new int[0];
        number_of_vertices = 0;
        number_of_edges = 0;
    };
    incident_matrix(int number_of_vertices, int number_of_edges){
        matrix = new int**[number_of_edges];
        for(int i = 0; i < number_of_edges; i++){
            matrix[i] = new int*[3];
            matrix[i][0] = new int[number_of_vertices]; //0 - pointer do array of vertex
            memset(matrix[i][0],0,number_of_vertices*sizeof(int)); //filled with 0
            matrix[i][1] = new int[0]; // 1 - source vertex
            memset(matrix[i][1],0,sizeof(int));
            matrix[i][2] = new int[0]; // 2 - destination vertex
            memset(matrix[i][2],0,sizeof(int));
        }
        this->number_of_vertices = number_of_vertices;
        this->number_of_edges = number_of_edges;
    };
    void print(){
        printf("   |");
        for(int i = 0; i < this->number_of_edges; i++){
            printf("%2d-%2d ", this->matrix[i][1][0],this->matrix[i][2][0]);
        }
        printf("\n----");
        for(int i = 0; i < this->number_of_edges; i++){
            printf("------", i);
        }
        printf("\n");
        for(int i = 0; i < this->number_of_vertices; i++){
            printf("%3d|",i);
            for(int j = 0; j < this->number_of_edges; j++){
                printf(" %4d ",this->matrix[j][0][i]);
            }
            printf("\n");
        }
    };
    void add_vertex(){
        this->number_of_vertices = this->number_of_vertices+1;
        int ***temp_matrix = new int**[this->number_of_edges];
        for(int i = 0; i < this->number_of_edges; i++){
            temp_matrix[i] = new int*[3];
            temp_matrix[i][0] = new int[this->number_of_vertices];
            memcpy(temp_matrix[i][0],this->matrix[i][0],(this->number_of_vertices-1)*sizeof(int));
            temp_matrix[i][0][this->number_of_vertices-1] = 0;
            temp_matrix[i][1] = this->matrix[i][1];
            temp_matrix[i][2] = this->matrix[i][2];
        }
        delete[] this->matrix;
        this->matrix = temp_matrix;
    };
    bool add_edge(int src_vertex, int dst_vertex){
        if(src_vertex < this->number_of_vertices && dst_vertex < this->number_of_vertices && src_vertex >= 0 && dst_vertex >= 0){
            for(int i = 0; i < this->number_of_edges-1; i++){
                if(this->matrix[i][1][0] == src_vertex && this->matrix[i][2][0] == dst_vertex){
                    this->matrix[i][0][src_vertex] = 1;
                    this->matrix[i][0][dst_vertex] = -1;
                    return true;
                }
            }
            this->number_of_edges = this->number_of_edges+1;
            int ***temp_matrix = new int**[this->number_of_edges];
            for(int i = 0; i < this->number_of_edges-1; i++){
                temp_matrix[i] = new int*[3];
                temp_matrix[i][0] = new int[this->number_of_vertices];
                temp_matrix[i][1] = new int[1];
                temp_matrix[i][2] = new int[1];
                memcpy(temp_matrix[i][0],this->matrix[i][0],this->number_of_vertices*sizeof(int));
                memcpy(temp_matrix[i][1],this->matrix[i][1],sizeof(int));
                memcpy(temp_matrix[i][2],this->matrix[i][2],sizeof(int));
            }
            temp_matrix[this->number_of_edges-1] = new int*[3];
            temp_matrix[this->number_of_edges-1][0] = new int[this->number_of_vertices];
            temp_matrix[this->number_of_edges-1][1] = new int[1];
            temp_matrix[this->number_of_edges-1][1][0] = src_vertex;
            temp_matrix[this->number_of_edges-1][2] = new int[2];
            temp_matrix[this->number_of_edges-1][2][0] = dst_vertex;
            memset(temp_matrix[this->number_of_edges-1][0],0,this->number_of_vertices*sizeof(int));
            temp_matrix[this->number_of_edges-1][0][src_vertex] = 1;
            temp_matrix[this->number_of_edges-1][0][dst_vertex] = -1;
            delete[] this->matrix;
            this->matrix = temp_matrix;
            return true;
        }else{
            return false;
        }
    };
};

int main(){
    // adjacency_matrix matrix = adjacency_matrix();
    // printf("======\n");
    // matrix.add_vertex();
    // matrix.add_vertex();
    // matrix.add_vertex();
    // matrix.add_vertex();
    // matrix.add_vertex();
    // printf("======\n");
    // matrix.add_edge(1,2,10);
    // matrix.add_edge(2,3,5);
    // matrix.add_edge(3,4,3);
    // matrix.print();

    // adjacency_list list = adjacency_list();
    // list.print();
    // list.add_vertex();
    // list.print();
    // list.add_vertex();
    // list.print();
    // list.add_vertex();
    // list.print();
    // list.add_vertex();
    // list.print();
    // list.add_vertex();
    // list.print();
    // list.add_vertex();
    // list.print();
    // list.add_vertex();
    // list.print();
    // list.add_vertex();
    // list.print();
    // list.add_vertex();
    // list.print();
    // list.add_vertex();
    // list.print();
    // list.add_vertex();
    // list.print();
    // list.add_vertex();
    // list.print();
    // list.add_edge(0,3,1);
    // list.print();
    // list.add_edge(3,4,5);
    // list.print();
    // list.add_edge(7,3,1);
    // list.print();
    // list.add_edge(4,3,2);
    // list.print();
    // list.add_edge(4,5,2);
    // list.print();
    // list.add_edge(4,7,100);
    // list.print();


    incident_matrix inc = incident_matrix();
    inc.add_vertex();
    inc.add_vertex();
    inc.add_vertex();
    inc.add_vertex();
    inc.add_vertex();
    inc.add_vertex();
    inc.add_edge(2,3);
    inc.add_edge(3,2);
    inc.add_edge(3,2);
    inc.add_edge(3,5);
    inc.add_edge(4,3);
    inc.add_edge(1,3);
    inc.print();

    return 0;
}
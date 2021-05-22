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
        for(int i = 0; i < this->number_of_vertices; i++){
            for(int j = 0; j < this->number_of_vertices; j++){
                printf("%2d",this->matrix[i][j]);
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
    bool add_edge(int src_vertex, int dst_vertex){
        if(src_vertex < this->number_of_vertices && dst_vertex < this->number_of_vertices && src_vertex >= 0 && dst_vertex >= 0){
            this->matrix[src_vertex][dst_vertex] = 1;
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
            printf("vertex %3d ->",i);
            node *current_node = array_of_lists[i];
            while(current_node){
                printf(" {vertex: %3d|weight: %3d},",current_node->dst,current_node->weight);
                current_node = current_node->next;
            }
            printf("\n");
        }  
    };
    void add_vertex(){
        this->number_of_vertices++;
        node **temp_array_of_lists = new node*[this->number_of_vertices];
        memcpy(temp_array_of_lists,this->array_of_lists,(this->number_of_vertices-1)*sizeof(this->array_of_lists[0]));
        delete[] this->array_of_lists;
        this->array_of_lists = temp_array_of_lists;
    };
    bool add_edge(int src_vertex, int dst_vertex, int weight){
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

int main(){
    // adjacency_matrix matrix = adjacency_matrix(10);
    // matrix.print();
    // printf("======\n");
    // matrix.add_vertex();
    // matrix.print();
    // printf("======\n");
    // matrix.add_edge(1,2);
    // matrix.add_edge(2,3);
    // matrix.add_edge(3,4);
    // matrix.print();
    adjacency_list list = adjacency_list(10);
    list.add_edge(0,3,1);
    list.add_edge(3,4,5);
    list.add_edge(7,3,1);
    list.add_edge(4,3,2);
    list.add_edge(4,5,2);
    list.print();

    return 0;
}
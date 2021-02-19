#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h> 
#define SIZE 100

enum color{red, blue, green, yellow, black, white};

struct node
{
    int id;
    int x;
    int y;
    struct node *Su;
    int length;         // size of the neighboor array
    enum color colour;
};

typedef struct node node;

node *nodes;
int ids = 0;

node *search_node_by_coord(int x, int y){
    int i; 
    for(i = 0; i < ids; i++){
        if(nodes[i].x == x && nodes[i].y == y){
            return nodes+i;
        }
    }
    return NULL;
}
node *search_node_by_id(int id){
    int i;
    for(i = 0; i < ids; i++){
        if(nodes[i].id == id){
            return nodes + i;
        }
    }
    return NULL;
}
//making new nodes 
void new_node(){

    node *new =(node *)malloc(sizeof(node));
    new->id = ids;
    do{
        new->x = (rand()%6+1);  // i is between 0 and 6
        new->y = (rand()%6+1); // y is between 0 and 6
    }
    while(search_node_by_coord(new->x, new->y) != NULL);        // checking if this node already exists
    nodes = (node *)realloc(nodes, (ids + 1) * sizeof(node));
    nodes[ids].x = new->x;         // assigning the values
    nodes[ids].y = new->y;
    nodes[ids].id = new->id;
    nodes[ids].length = 0;    
    ids++;
}
//sending receiving and storing the ids of nodes close to each point
void inform(node point){
    int x_plus = point.x + 1;
    int y_plus = point.y + 1;
    int x_minus = point.x - 1;
    int y_minus = point.y - 1;
    //checking all possible positions around a node in euclidean space 
    node *curr = search_node_by_coord(x_plus, y_plus);
    if(curr != NULL){
        curr->Su = (node *)realloc(curr->Su, (curr->length + 1) * sizeof(node));
        curr->Su[curr->length] = point;
        curr->length++;
        return;
    }
    curr = search_node_by_coord(x_plus, point.y);
    if(curr != NULL){
        curr->Su = (node *)realloc(curr->Su, (curr->length + 1) * sizeof(node));
        curr->Su[curr->length] = point;
        curr->length++;
        return;
    }
    curr = search_node_by_coord(x_plus, y_minus);
    if(curr != NULL){
        curr->Su = (node *)realloc(curr->Su, (curr->length + 1) * sizeof(node));
        curr->Su[curr->length] = point;
        curr->length++;
        return;
    }
    curr = search_node_by_coord(point.x, y_plus);
    if(curr != NULL){
        curr->Su = (node *)realloc(curr->Su, (curr->length + 1) * sizeof(node));
        curr->Su[curr->length] = point;
        curr->length++;
        return;
    }
    curr = search_node_by_coord(point.x, y_minus);
    if(curr != NULL){
        curr->Su = (node *)realloc(curr->Su, (curr->length + 1) * sizeof(node));
        curr->Su[curr->length] = point;
        curr->length++;
        return;
    }
    curr = search_node_by_coord(x_minus, y_plus);
    if(curr != NULL){
        curr->Su = (node *)realloc(curr->Su, (curr->length + 1) * sizeof(node));
        curr->Su[curr->length] = point;
        curr->length++;
        return;
    }
    curr = search_node_by_coord(x_minus, point.y);
    if(curr != NULL){
        curr->Su = (node *)realloc(curr->Su, (curr->length + 1) * sizeof(node));
        curr->Su[curr->length] = point;
        curr->length++;
        return;
    }
    curr = search_node_by_coord(x_minus, y_minus);
    if(curr != NULL){
        curr->Su = (node *)realloc(curr->Su, (curr->length + 1) * sizeof(node));
        curr->Su[curr->length] = point;
        curr->length++;
        return;
    }
}

int compare(const void *a,const void *b){
    int *x = (int *) a;
    int *y = (int *) b;
    return *x - *y;
}
//sorting the Set
void traverse_nodes(){
    int i;
    for(i = 0; i < ids; i++){
        qsort(&nodes[i], sizeof(node), sizeof(int), compare);  
    }
}

// create a 2x2 table that has which node connects with which
void create_graph_table(int graph[ids][ids]){
    int i, j, k;
    //initiallizing the values
    for(i = 0; i < ids; i++){
        for(j = 0; j < ids; j++){
            graph[i][j] = -1;
        }
    }
    for(i = 0; i < ids; i++){
        for(j = 0; j < ids; j++){
            if(graph[i][j] != -1){
                break;
            }
            for(k = 0 ; k < nodes[i].length; k++){
                if(nodes[j].id == nodes[i].Su[k].id){
                    graph[i][j] = 1;
                    break;
                }
            }
            if(graph[i][j] == -1){
                graph[i][j] = 0;
            }
        }
    }
}
//check if it's safe to assign a colour to a specific node
bool isSafe(int v, int graph[ids][ids], int c){

    for (int i = 0; i < ids; i++){
        if (graph[v][i] && c == nodes[i].colour){
            return false;
        }
    }
    return true; 
}

bool graph_coloring(int graph[ids][ids], int v){ 
    //base case: If all vertices are assigned a color then return true 
    if (v == ids) 
        return true; 
    
    // Consider this vertex v and try different colors 
    int c;
    for (c = 0; c < 5; c++) { 
        // Check if assignment of color c to v is fine
        if (isSafe(v, graph, c)) { 
            nodes[v].colour = c; 
            //recur to assign colors to rest of the vertices 
            if (graph_coloring(graph, v + 1) == true) 
                return true; 
  
            //If assigning color c doesn't lead to a solution then remove it 
            nodes[v].colour = 0; 
        } 
    } 
    //If no color can be assigned to this vertex then return false 
    return false; 
} 

//creating the graph and
//colouring the nodes 
void finalizing(){

    int graph[ids][ids];
    
    create_graph_table(graph);
    /*
    int i, j;
    for(i = 0; i < ids; i++){
        for(j = 0; j < ids; j++){
            printf(" %d", graph[i][j]);;
        }
        printf("\n");
    }*/
    graph_coloring(graph, 0);
}

/*int main(int argc, char *argv[]){

    srand(time(NULL));
    new_node();
    new_node();
    new_node();
    new_node();
    new_node();
    new_node();
    new_node();
    new_node();
    new_node();
    new_node();
    int i;
    for(i = 0; i < ids ; i++){
        inform(nodes[i]);
    }
    
    finalizing();
    
    return 0;
}*/
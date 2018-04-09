/********************************************************************
 "I pledge my honor that I have abided by the Stevens Honor System."
  Matthew Monaco and Nicholas Primamore
 
  primmst.cpp
 *******************************************************************/


#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "minprio.h"
#include "graph.h"
#include <stdio.h>
#include "primmst.h"

/* Assume g is non-null pointer to non-empty graph.
 * Assume g is a symmetric graph, so it can be
 * considered as an undirected graph.
 * (In particular, the weight u->v must equal weight v->u
 * for every u,v.)
 * Assume g is a connected graph.
 *
 * Return a minimum spanning tree, as a new graph.
 * (So it will have the same number of vertices,
 * but only some of the edges.  Weights same as in g.)
 *
 * Implement this using Prim's algorithm.  The implementation only
 * needs to access the input graph via the graph.h API, so
 * it shouldn't matter which representation the input uses.
 *
 * Note that prim's uses a priority queue. Our priority queue is general.
 * That is, the client(I.E. your prim's implementation) must provide 2 things:
 *  1) The comparison function
 *  2) The content to be put into the queue.
 *
 *  The content should be some sort of struct, that contains the vertex, and its current known distance
 *  Remember that all distances but 1 of them start at INFINITY, and the other at 0.
 *  Since content is a void pointer, you will need to cast it to your struct type
 *  before accessing the structs fields, cast by doing : ((struct_name*)your_handle*->content)->struct_field
 *
 * For function pointers, simply create a function that matches the template defined in minprio.h,
 * and when calling the Queue's constructor, pass in the address of that function,
 * (using the & operator and the function name)
 */

struct content{
    int node;
    float currentDistance;
};

int compare(void* lhs, void* rhs){
    //handle what type of pointer (cast to my struct)
    float left = ((content*)lhs)->currentDistance;
    float right = ((content*)rhs)->currentDistance;
    
    if(left < right){
        return -1;
    }
    else if(left > right){
        return 1;
    }
    else return 0;
}

Graph* minSpanTree(Graph* h){
    Graph* MST = new Graph(h->numVerts(), 0);
    int numVertices = MST->numVerts();
    content* chosenNode = new content;
    MinPrio* PQ = new MinPrio(&compare, numVertices);
    int* link = new int[numVertices];
    int* inQ = new int[numVertices];
    MinPrio::handle** handles = new MinPrio::handle*[numVertices];
    
    for(int index = 0; index < numVertices; index++){
        link[index] = 0;
        handles[index] = NULL;
        inQ[index] = 1;
    }
    
    chosenNode->node = 0;
    chosenNode->currentDistance = 0;
    handles[0] = PQ->enqueue(chosenNode);
    
    for(int index = 1; index < numVertices; index++){
        content* currentNode = new content;
        currentNode->node = index;
        currentNode->currentDistance = INFINITY;
        handles[index] = PQ->enqueue(currentNode);
    }
    
    while(PQ->nonempty()){
        int v = ((content*)(PQ->dequeueMin()))->node;
        printf("%d\n",v);
        inQ[v] = 0;
        handles[v] = NULL;
        int* suc = h->successors(v);
        
        for(int index = 0; suc[index] != -1; index++){
            if(inQ[suc[index]] == 1 && h->edge(suc[index], v) <
                ((content*)handles[suc[index]]->content)->currentDistance){
                //seg faults in the if statement
                link[suc[index]] = v;
                ((content*)handles[suc[index]]->content)->currentDistance = h->edge(suc[index], v);
                PQ->decreasedKey(handles[suc[index]]);
            }
        }
    }
   
    for(int index = 0; index < numVertices; index++){
        MST->addEdge(index, link[index], h->edge(index, link[index]));
        MST->addEdge(link[index], index, h->edge(link[index], index));
    }
    
    delete chosenNode;
    delete PQ;
    delete link;
    delete inQ;
    for(int index = 0; index < numVertices; index++){
        if(handles[index] != NULL){
            delete handles[index];
        }
    }
    delete handles;
    
    return MST;
}

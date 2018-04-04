#ifndef PRIMMST_H
#define PRIMMST_H

#include "graph.h"
#include "minprio.h"

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
 *  before accessing the structs fields, casting by doing : ((struct_name*)your_handle*->content)->struct_field
 * 
 * For function pointers, simply create a function that matches the template defined in minprio.h, 
 * and when calling the Queue's constructor, pass in the address of that function,
 * (using the & operator and the function name)
 */

Graph* minSpanTree(Graph* g);

#endif

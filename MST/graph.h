#ifndef	GRAPH_H
#define	GRAPH_H	

#include <math.h>

/* Simple interface for weighted directed graphs.

   The vertices are numbered 0 .. n-1 for some fixed n.
   Edges have nonnegative weights.

   The two standard graph representations are supported.
   Function cloneGraph() can be used to convert between 
   representations.

*/

#define MATRIX 0
#define LIST 1


using namespace std;

class Graph{

 public:

  struct Node; //a node in our linked list, for our adjacency list. To be implemented in the cpp file

  
  
/* Make an empty graph with n vertices (empty means all edges either set to INFINITY or NULL,
   depending on representation), using either adjacency matrix 
   or adjacency lists depending on whether rep==MATRIX or rep==LIST.
   if n <1, return NULL
   if rep is not MATRIX or LIST, return NULL
*/
Graph(int n, int rep);


 /* free the graph object and ALL its resources. 
*/
~Graph();


/* make a DEEP copy of g, but using the representation
   specified by rep (which is assumed to be MATRIX or LIST) 
   Everything should be copied over, not just references
   I.E. a change in the first graph should not change the second graph
   If rep is not MATRIX or LIST, return NULL
*/
Graph* cloneGraph(int rep);




/* return number of vertices.
 Note that this is needed since the property numVerts is private*/
int numVerts();


/* add edge from source to target with weight w, and return
   true, if source and target are valid vertex numbers and
   there was no edge from source to target initially.
   Otherwise if source or target are invalid, or the edge already exists,
    make no change and return false. If the edge already exists, DO NOT UPDATE IT! Just return false
    if w is INFINITY or negative, do nothing, and return false.
*/
bool addEdge( int source, int target, float w);


/* delete edge from source to target, and return
   true, if there was an edge from source to target, and both source and target are valid vertex numbers.
   Otherwise, make no change and return false. 
*/
bool delEdge( int source, int target);


/* return weight of the edge from source to target,
   if there is one; otherwise return INFINITY. (part of the math.h library)
   Return -1.0 if source or target are not valid vertex numbers.
*/
float edge( int source, int target);


/* return a freshly new'd array with the successor
   vertices of source, if any, followed by an entry with -1
   to indicate end of sequence.
   Make sure your returned array has enough spots for the number of successors, and the -1.
   If there are no successors, the array should only contain a -1
   If source is not a valid vertex number, return NULL

   Ownersip: the caller is responsible for deleting the array in their file.
   You should not make any delete call in your file graph relating to successors, but rather in your test file
*/
int* successors( int source);

/*  return a freshly new'd array with the predecessor
   vertices of source, if any, followed by an entry with -1
   to indicate end of sequence.
   Make sure your returned array has enough spots for the number of successors, and the -1.
   If there are no successors, the array should only contain a -1
   If source is not a valid vertex number, return NULL

   Ownersip: the caller is responsible for deleting the array in their file.
   You should not make any delete call in your file relating to successors
*/
int* predecessors( int target);

 
 private:
  int numVertices; //numer of vertices the graph has
  int type; //the representation of the graph, list or matrix
  Node** list; //a pointer to the array used for the adjacency list, if used
  float* matrix; //a pointer to the matrix, if used

  //note that at any point in time, only list or matrix will be used; the other will be null 
};
  

#endif

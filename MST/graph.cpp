
/*
 *
 * Graphs.cpp
 * Creates a graph, implimented both as an adjacency list and matrix
 * Performs some bounds checks, but not all
 * 
 *  C 2018 Justin Barish
 */

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

struct Graph::Node {
  int val;
  float weight;
  Node* next;
};

/* make an empty graph with n vertices,
using either adjacency matrix or adjacency lists
depending on whether rep==MATRIX or rep==LIST
Precondition: n>=1 and rep is MATRIX or LIST.
*/
Graph::Graph (int n, int rep){
  if ( n < 1){
    numVertices = 1;
    n=1;
  }else{
    numVertices = n;
  }
  if(rep != MATRIX && rep!=LIST){
    type = MATRIX;
  }else{
    type = rep;
  }
 
  if(rep == MATRIX){
    list = NULL;
    matrix = new float[n*n];
    int index;
    for(index=0; index<(n*n); index++){
      matrix[index] = INFINITY;
    }
  }else {
    list = new Node*[n];
    int index;
    for (index=0; index<numVertices; index++){
      
      list[index] = NULL;
    }
    matrix = NULL;
  }
}

/* make a copy of G, but using the representation
   specified by rep (which is assumed to be MATRIX or LIST) 
*/
Graph* Graph::cloneGraph(int rep){
  if(rep != MATRIX && rep!=LIST){
    return NULL;
  }
  Graph* g = new Graph(numVertices,rep);
 
  int n = g->numVerts();
    
  /*go through old graph and add nodes to the new one */
  int src; int tgt;
  for (src=0; src < n; src++){
    for(tgt = 0; tgt<n; tgt++){
      g->addEdge(src, tgt ,edge(src, tgt)); 
    }
  }
 return g;
}

/* free the graph object and all its resources.
Postcondition: G is no longer a valid pointer. 
Alert: the caller should set their variable to null.
*/
Graph::~Graph(){
  if (type ==MATRIX){
    int i;

    //clear out all data
    for(i = 0; i< numVertices * numVertices; i++){
      matrix[i] = INFINITY;
    }
    
    numVertices =type = 0;
    //free(G->v);
    delete[] matrix;
  }else {
    //free all nodes
    Node* curr;
    Node* next;
    int index;
    int n = numVertices;
    for (index=0; index<n; index++){
      curr = ( list[index]);
      while(curr!= NULL){
	next = curr->next;
	curr->next = NULL; curr->weight = 0; curr->val = 0;
	delete curr;
	curr = next;
      }
    }
    numVertices = type = 0;
    delete[] list;
  }
}

/* number of vertices */
int Graph::numVerts(){
  return numVertices;
}

/* add edge from source to target with weight w, and return
OK, if source and target are valid vertex numbers and
there was no edge from source to target initially.
Otherwise, make no change and return ERROR. 
*/
bool Graph::addEdge(int source, int target, float w){
  if(source >=numVertices || source <0 ||target >=numVertices || target <0 || w < 0 || w== INFINITY){
    return false;
  }
  if(type==MATRIX){
    if(matrix[numVertices*source +target] == INFINITY){
      matrix[numVertices*source +target] = w;
      return true;
    }else {
      return false;
    }
  }else {
    Node* curr = (list[source]);
    Node* prev = NULL;
    while(curr!=NULL){
      if(curr->val == target){
	return false;
      }
      prev = curr;
      curr = curr->next;
    }
    Node* n = new Node();
    n->next = NULL;
    n->val = target;
    n->weight = w;
    if(prev ==NULL){
      (list[source])= n;
    }else{
      prev->next = n;
    }
    return true;
  }
}

/* delete edge from source to target, and return
OK, if there was an edge from source.
Otherwise, make no change and return ERROR. 
*/
bool Graph::delEdge( int source, int target){
  if(source >=numVertices || source <0 ||target >=numVertices || target <0){
    return false;
  }
 if(type==MATRIX){
    if(matrix[numVertices*source +target] != INFINITY){
      matrix[numVertices*source +target] = INFINITY;
      return true;
    }else {
      return false;
    }
  }else {
    Node* curr = ( list[source]);
    Node* prev = NULL;
   
    while(curr!=NULL){
      if(curr->val == target){
	if (prev ==NULL){
	  (list[source])= curr->next;
	}else {
	  prev->next = curr->next;
	}
	curr->val = 0; curr->weight = 0; curr->next = NULL;
	delete curr;
	return true;
      }
      prev = curr;
      curr = curr->next;
    }
    return false;
  }
}

/* return weight of the edge from source to target,
if there is one; otherwise return INFINITY.
Return -1 if source or target are out of range.
*/
float Graph::edge(int source, int target){
  if(source >=numVertices || source <0 ||target >=numVertices || target <0){
    return -1;
  }
   if(type==MATRIX){
     return matrix[numVertices*source +target];
   }else /*type is LIST*/ {
    Node* curr = ( list[source]);
    Node* prev = NULL;
   
    while(curr!=NULL){
      if(curr->val == target){
	return curr->weight;
      }
      prev = curr;
      curr = curr->next;
    }
    return INFINITY;
  }
}

/* a freshly allocated array with the successor
vertices of source, if any, followed by an entry with -1
to indicate end of sequence.
*/
int* Graph::successors( int source){
  if(source >=numVertices || source <0){
    return NULL;
  }
   if(type==MATRIX){

     //figure out how big array will need to be
     int i;
     int counter=0;
     for(i=0; i<numVertices; i++){
       if(matrix[numVertices*source+i]!= INFINITY){
	 counter++;
       }
     }
     int* s = new int[counter+1];//space for -1
      for(i=0; i<numVertices; i++){
	if(matrix[numVertices*source+i]!= INFINITY){
	  *s = i;
	  s++;
	}
      }
      *s = -1; //-1 terminating array
      return s-counter; //put our pointer back to the begining
   }else /*type is LIST*/ {
     int i;
     int counter=0;
     Node* curr = ( list[source]);
     while(curr!=NULL){
       
       counter++;
       curr = curr->next;
     }
     int* s = new int[counter+1];//space for -1
     curr = ( list[source]);
     while(curr!=NULL){
       *s = curr->val;
       curr = curr->next;
       s++;
     }
     *s = -1;
      
     return s-counter; 
   }
}

/* a freshly allocated array with the predecessor
vertices of target, if any, followed by an entry with -1
to indicate end of sequence.
*/
int* Graph::predecessors( int target){
  if(target >=numVertices || target <0){
    return NULL;
  }
  if(type==MATRIX){
    int i;
    int counter=0;
    for(i=0; i<numVertices; i++){
      if(matrix[numVertices*i+target]!= INFINITY){
	counter ++;
      }
    }
    int* s = new int[counter+1];//space for -1
    for(i=0; i<numVertices; i++){
      if(matrix[numVertices*i+target]!= INFINITY){
	*s = i;
	s++;
      }
    }
    *s = -1; //-1 terminating array
    return s-counter;
  }else{
    int i;
    int counter=0;
    for(i = 0; i<numVertices; i++){
      Node* curr = ( list[i]);
      while(curr!=NULL){
	if(curr->val == target){
	  counter ++;
	}
	curr = curr->next;
      }
    }
    int* s = new int[counter+1];//space for -1
    for(i = 0; i<numVertices; i++){
      Node* curr = (list[i]);
      while(curr!=NULL){
	if(curr->val == target){
	  *s = i;
	  s++;
	}
	curr = curr->next;
      }
    }
    *s = -1;
    return s-counter; 
  }
}

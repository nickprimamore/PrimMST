#ifndef GRAPHIO_H
#define GRAPHIO_H
 
#include "graph.h"

/****************************************************************
Graphs with names for vertices.
Provides function to read a graph from a file, and a 
function to write a graph to standard out.  (You can 'pipe' that
to a file when needed.)

File format is assumed to be:
- first line is a nonnegative integer N in decimal
- following N lines each have one vertex name (a sequence of 
  non-blank chars, at most MAX_NAMELEN of them).
- remainder of the file can have blank lines; non-blank lines 
have either two or three non-blank elements, in the form:
S T W
where S and T are vertex names, for an edge S->T, and W is a decimal weight.
The W is optional; if omitted, the weight is interpreted as DEFAULT_WEIGHT.
******************************************************************/

/* change history:
 * DN 4/18/2016 writeGraph write weights 
 * DN 2/16/2017 add disposeGraphInfo, improve comments */


#define DEFAULT_WEIGHT 1.0 
#define MAX_NAMELEN 32 /* max length of a vertex name */

/* A graph together with names for its vertices. 
   The vertnames field should point to an array of string pointers.
   The length of that array should be numVerts(graph).
*/
struct graphinfo {
    Graph* graph;
    char **vertnames; 
};
typedef struct graphinfo* GraphInfo;


/* Index of a given vertex name, or -1 if not found. */
/* NOTE: To get the name for a given vertex number v, just use g->vertnames[v] */
int vertexNum(GraphInfo gi, char* name);


/* Read a graph from a text file, assuming format described above. 
 * Assumes filepath is a null-terminated string that is valid file path.
 * Assumes the file has the format specified above.
 *
 * Ownership: the caller is responsible for disposing of the graph, for
 * freeing the vertnames, and for freeing the graphinfo object itself.
 * See disposeGraphInfo.
 */
GraphInfo readGraph(char* filepath, int repType);


/* Same as readGraph, but make a symmetric graph by also adding T->S
 * in addition to S->T, for a line with S T, unless there was already
 * an edge T->S from a preceding line of the file.
 */
GraphInfo readGraphMakeSymm(char* filepath, int repType);


/* Prints the graph to stdout (the terminal), in the file format:
 * Assumes gi points to a valid object.  
 * Output should be IDENTICAL to the input file, except that it
 * prints weights even if they are the default weight, and blank lines in the 
 * input file don't need to be in the output text
 */
void writeGraph(GraphInfo gi);


/* Free the graph, the vertex names, the array of those,
 * and the GraphioInfo itself. 
 */
void disposeGraphInfo(GraphInfo gi);


#endif 

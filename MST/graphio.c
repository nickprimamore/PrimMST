#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "graphio.h"

/* IMPLEMENTATION OF graphio.h*/


/* max data to read from a line of input, allowing for two vertex names and weight */
#define LINE_LEN (2 * (MAX_NAMELEN+1) + 20)


/* see graphio.h */
int vertexNum(GraphInfo gi, char* name) {
    int i = 0;
    int n = gi->graph->numVerts();
    while ( i < n && strcmp(gi->vertnames[i], name) )
        i++;
    if (i < n)
        return i;
    else
        return -1;
}



/* Read a graph from a text file, using the requested representation.
 Assumes filepath is a null-terminated string that is valid file path.
 Assumes the file exists and has the format specified in graphio.h.
 If makeSymmetric is MAKE_SYMM, add both (S,T) and (T,S) for each edge S T in the file
 (except when (T,S) present from an earlier line of the file).
 */
GraphInfo readGraphX(char* filepath, int repType, int makeSymmetric) {
    
    /* open file, initialize input buffer, get number of vertices */
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        fprintf(stderr, "graphio:readGraph - fatal error: could not open file %s\n", filepath);
        exit(1);
    }
    size_t lineSize = LINE_LEN;
    char* line = (char *) malloc(lineSize);
    getline(&line, &lineSize, file); /* get one line of input */
    int numVerts;
    sscanf(line, "%i", &numVerts); /* parse the line */
    
    /* allocate and initialize GraphInfo */
    GraphInfo gi = (GraphInfo) malloc(sizeof(struct graphinfo));
    gi->graph = new Graph(numVerts, repType);
    char **vertnames = (char**) malloc(numVerts * sizeof(char *));
    gi->vertnames = vertnames;
    
    /* get vertex names */
    char source[MAX_NAMELEN + 1];
    int i = 0;
    while( i < numVerts && getline(&line, &lineSize, file) > 0 ) {
        sscanf(line, "%s", source);
        vertnames[i] = (char *) malloc(strlen(source) + 1);
        strcpy(vertnames[i], source);
        i++;
    }
    if (i != numVerts) {
        fprintf(stderr, "graphio:readGraph - file format error getting vertex names in %s\n", filepath);
        exit(1);
    }
    
    /* get the edges */
    char target[MAX_NAMELEN + 1];
    float weight;
    
    while ( getline(&line, &lineSize, file) > 0 ) {
        int result = sscanf(line, "%s %s %f\n", source, target, &weight);
        
        if (result >= 2 ) { // read at least two items
            int src = vertexNum(gi, source);
            int trg = vertexNum(gi, target);
            if (result == 2) // weight not included
                weight = DEFAULT_WEIGHT;
            (gi->graph)->addEdge(src, trg, weight);
            if (makeSymmetric)
	      (gi->graph)->addEdge( trg, src, weight);
        } else
            if (result == 1)
                fprintf(stderr, "graphio:readGraph - file format error (ignored) getting edges in %s\n", filepath);
    }
    
    /* clean up and return */
    free(line);
    fclose(file);
    
    return gi;
}


/* see graphio.h */
GraphInfo readGraph(char* filepath, int repType) {
    return readGraphX(filepath, repType, 0);
}


/* see graphio.h */
GraphInfo readGraphMakeSymm(char* filepath, int repType) {
    return readGraphX(filepath, repType, 1);
}


/* see graphio.h */
void writeGraph(GraphInfo gi) {
  int numV = gi->graph->numVerts();
  printf("%i \n", numV);
  for (int v = 0; v < numV; v++)
    printf("%s \n", gi->vertnames[v]);
  for (int v = 0; v < numV; v++) {
    int* succs = gi->graph->successors( v);
    int i = 0;
    if (succs[0] != -1) 
      printf("\n");
    while (succs[i] != -1) {
      printf("%s %s %f \n", gi->vertnames[v], gi->vertnames[succs[i]], gi->graph->edge(v, succs[i]));
      i++;
    }
  }
  printf("\n");
  
    
}


/* see graphio.h */
void disposeGraphInfo(GraphInfo gi) {
    for (int i = 0; i < gi->graph->numVerts(); i++)
        free(gi->vertnames[i]);
    free(gi->vertnames);    
    delete (gi->graph); //note this calls the graph destructor
    free(gi);
}




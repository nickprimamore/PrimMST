#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "minprio.h"

/* min-priority queues
 * Items in a given queue are struct pointers that should be
 * comparable by the comparator provided to makeQueue.
 *
 * Implement using an array heap as in Levitin 6.4 (but MIN).
 *
 * This API provides "addressable" queue, meaning that the client
 * is given a handle for each enqueued item.  This enables an
 * efficient interface for the decrease-key operation.  (The handle
 * enables the decreaseKey to find, in constant time, an item's
 * index in the array.)
 *
 */


/* NOTE:
 * If you don't understand priority queues, it is imperative that you read Ch 6.4 in the textbook.
 * Otherwise, you WILL be lost.
 * You MUST implement this using an Array, as described in the textbook. Do NOT use a tree.
 * With this manner, node i's kids will be in array positions 2i and 2i+1.
 * Also, node i's parent is at position i/2 (using integer division)
 * Also, do NOT use the HeapBottomUp method in the textbook to reorder your queue.
 * That is for creating a heap, not reordering an existing one.
 * It probably could work, but it has a horrible time complexity, and in this class,we aim for good algorithms,
 * compared to the correct solution (Which isn't that difficult, just requires some thought).
 * Perhaps some of the logic of HeapBottomUp carries over...
 */


/* handles for efficient access to enqueued items
 * A handle is a container that has the content (of any type), and the position in the array.
 * Why the position?  Prim's is where we update a key in our Priority Queue.
 * If we have a 'handle' to our entry in the Priority Queue, it is easy to find where it is to update.
 * ALERT: Clients must not read or write the pos field; it's for use only
 * by code in minprio.cpp.
 * Note that any use of this outside of this class requires doing MinPrio::handle* ......;
 */
struct handle {
    int pos;       /* not for client use (current position in queue array) */
    void* content; /* the client's data, void* since can be of any type */
};

/* make an empty queue
 * Items will be compared using compFunc, and this compFunc is saved into our private variable comp
 * Makes an array of size maxsize+1 (since spot 0 is empty) of handle pointers
 * All array spots need to be initialized with NULL
 *
 * It's the client's responsibility to ensure that
 * there are never more than maxsize elements in queue.
 * Assume that compFunc is a valid compFunc. Do not check this fact, and do not write tests that
 * use an invalid comparison function.
 * If maxsize < 1, make maxsize = 1.
 */
MinPrio::MinPrio(Comparator compFunc, int maxsize){
    if(maxsize < 1){
        MAXSIZE = 1;
    }
    else{
        MAXSIZE = maxsize;
    }
    
    comp = compFunc;
    currentSize = 0;
    
    handles = new MinPrio::handle*[MAXSIZE + 1];
    for(int index = 1; index < MAXSIZE; index++){
        handles[index] = NULL;
    }
}


/* dispose of memory owned by the queue
 * Namely: the queue object, the array, and the Handles.
 * The Handle contents are the responsibility of the client (since the client has its own array of handles).
 */
MinPrio::~MinPrio(){
    for(int index = 1; index < MAXSIZE; index++){
        if(handles[index] != NULL){
            delete handles[index];
        }
    }
    delete handles;
}


/* enqueue
 * If queue is full, return NULL
 * Value of item doesn't matter. Even if NULL, enqueue it anyway
 * Returns a handle* containing the item, for use with decreaseKey.
 */
MinPrio::handle* MinPrio::enqueue(void *item){
    if(currentSize == MAXSIZE){
        return NULL;
    }else if(currentSize = 0){
        handles[1] = new handle;
        handles[1]->pos = 1;
        handles[1]->content = item;
        currentSize++;
        return handles[1];
    }else{
        currentSize++;
        handles[currentSize] = new handle;
        handles[currentSize]->pos = currentSize;
        handles[currentSize]->content = item;
        int t = currentSize;
        while(t/2 > 0){
            int compare = comp(handles[t/2]->content, handles[t]->content);
            if(compare > 0){
                //Parent greater than child
                handle *tempNode = handles[t];
                handles[t] = handles[t/2];
                handles[t]->pos = t;
                handles[t/2] = tempNode;
                handles[t/2]->pos = t/2;
                t = t/2;
                delete tempNode;
            }else{
                return handles[t];
            }
        }
    }
}


/* true if queue has elements, else false (assuming qp non-null) */
bool MinPrio::nonempty(){
    if(currentSize > 0){
        return true;
    }else{
        return false;
    }
}

/* dequeue and return a minimum element according to the comparator.
 * This returns the CONTENT of the handle, NOT the Handle!
 *
 * Ensure that for any elements that might be re-ordered, you change their position values
 *
 * If queue is empty, return NULL.
 * Frees the handle, so client should no longer use handle.
 */
void* MinPrio::dequeueMin(){
    if(currentSize < 1){
        return NULL;
    }
    int minPos = 1;
    MinPrio::handle *minNode = handles[minPos];
    handles[minPos]->content = handles[currentSize]->content;
    handles[currentSize] = NULL;
    currentSize--;
    int t = minPos;
    bool cont = true;
    while(handles[2*t] != NULL){
        if(handles[2*t+1] == NULL){
            int compLeft = comp(handles[t]->content, handles[2*t]->content);
            if(compLeft > 0){
                handle *tempNode = handles[t];
                handles[t] = handles[2*t];
                handles[t]->pos = t;
                handles[2*t] = tempNode;
                handles[2*t]->pos = 2*t;
                t = 2*t;
                delete tempNode;   
            }
        }
        int compLeft = comp(handles[t]->content, handles[2*t]->content);
        int compRight = comp(handles[t]->content, handles[2*t+1]->content);
        if(compLeft > 0 || compRight > 0){
            if(comp(handles[2*t]->content, handles[2*t+1]->content) < 0){
                //left Child less than right child
                handle *tempNode = handles[t];
                handles[t] = handles[2*t];
                handles[t]->pos = t;
                handles[2*t] = tempNode;
                handles[2*t]->pos = 2*t;
                t = 2*t;
                delete tempNode;
            }else{
                //Right less than left
                handle *tempNode = handles[t];
                handles[t] = handles[2*t+1];
                handles[t]->pos = t;
                handles[2*t+1] = tempNode;
                handles[2*t+1]->pos = 2*t+1;
                t = 2*t+1;
                delete tempNode;
            }
        }
    }
    return minNode->content;
}


/* decrease the item's key
 * MUST be called whenever comparison value may have changed in the client.
 * Must only decrease comparison value (i.e. raise priority).
 *
 * Uses the handle to find the element in the queue in O(1) time.
 * Since the handle has a position field, that position should be its position in the array
 * Ensure that for any elements that might be re-ordered, you change their position values
 *
 * Assumes hand is in the queue, and non-null.
 * So, do NOT test cases where hand is null or not in the queue. Even if your code handles it, don't.
 */
void MinPrio::decreasedKey(handle* hand){
    handles[hand->pos]->content = hand->content;
    int t = hand->pos;
    while(t/2 > 0){
        int compare = comp(handles[t/2]->content, handles[t]->content);
        if(compare > 0){
            //Parent greater than child
            handle *tempNode = handles[t];
            handles[t] = handles[t/2];
            handles[t]->pos = t;
            handles[t/2] = tempNode;
            handles[t/2]->pos = t/2;
            
            t = t/2;
            delete tempNode;
        }else{
            return;
        }
    }
    return;
}

#ifndef minprio_H
#define minprio_H

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




class MinPrio{

 
 public:

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
  

  //typedef struct handle* Handle;


  /* type and contract for comparison function
   * Assumes lhs and rhs non-null.
   * This is a function pointer, where we name our function Comparator, 
   * that takes in 2 void pointers, and returns an integer.
   * This is NOT a function, but a pointer to one. You do NOT implement this in minprio.h
   * Since this queue is meant to work with ANY data type, it needs to be generic,
   * so the queue doesn't know how to compare the elements.
   * So, the client defines a comparsion function (this would be in primmst.cpp)
   * and passes it into the constructor of minprio, and then we save a 
   * POINTER to that function (see private fields).
   * Again, DO NOT IMPLEMENT THIS FUNCTION IN minprio.cpp!!!
   *
   * When this function is implemented by the client, it will return values as following:
   *  Return value < 0 if the comparison value of lhs < comparison value of rhs
   *  Return value > 0 if the comparison value of lhs > comparison value of rhs
   *  Return value = 0 if the comparison value of lhs = comparison value of rhs
   */
  typedef int (*Comparator)(void* lhs, void* rhs);


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
  MinPrio(Comparator compFunc, int maxsize);


  /* dispose of memory owned by the queue
   * Namely: the queue object, the array, and the Handles.
   * The Handle contents are the responsibility of the client (since the client has its own array of handles).
   */
  ~MinPrio();


  /* enqueue 
   * If queue is full, return NULL
   * Value of item doesn't matter. Even if NULL, enqueue it anyway
   * Returns a handle* containing the item, for use with decreaseKey.
   */
  handle* enqueue( void* item);  


  /* true if queue has elements, else false (assuming qp non-null) */
  bool nonempty();


  /* dequeue and return a minimum element according to the comparator.
   * This returns the CONTENT of the handle, NOT the Handle!
   *
   * Ensure that for any elements that might be re-ordered, you change their position values
   *
   * If queue is empty, return NULL.
   * Frees the handle, so client should no longer use handle.
   */
  void* dequeueMin(); 


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
  void decreasedKey(handle* hand);



  
 
 private:
  Comparator comp; //Our stored comparison function
  int MAXSIZE; //the max size our queue can get to
  int currentSize; // how many elements are currently in the queue, adjusted by enqueue and dequeue
  handle** handles; // our array of elements. We use handle pointers to represent our elements.
};

#endif

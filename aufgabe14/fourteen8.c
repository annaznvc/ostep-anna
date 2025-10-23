



/*

Try out some of the other interfaces to memory allocation. 
For example, create a simple vector-like data structure and related routines that use realloc() to manage the vector. 
Use an array to store the vectors elements; 
when a user adds an entry to the vector, use realloc() to allocate more space for it. 
How well does such a vector perform? How does it compare to a linked list? 
Use valgrind to help you find bug

*/



#include <stdio.h>
#include <stdlib.h>
#include "fourteen8.h"
 
int main()
{
    vector v = {.data = (int *)malloc(sizeof(int)), .size = 1, .capacity = 1};
    v.data[0] = 4;
 
    vector *vp = &v;
    vector_add(vp, 5);
    vector_add(vp, 6);
    vector_add(vp, 7);
    vector_delete(vp);
 
    vector_print(vp);
    vector_free(vp);
 
    return 0;
}
#include <stdlib.h>
#include <stdio.h>
 
int main()
{
    int *data = (int *)malloc(100 * sizeof(int));

    int *p = &data[3];
    free(p);
  
    return 0;
}
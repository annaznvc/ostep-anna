>1. First build main-race.c. Examine the code so you can see the
(hopefully obvious) data race in the code. Now run helgrind (by
typing valgrind --tool=helgrind main-race) to see how it
reports the race. Does it point to the right lines of code? What other
information does it give to you

```
valgrind --tool=helgrind ./main-race
```

```
#include <stdio.h>

#include "common_threads.h"

int balance = 0;

void* worker(void* arg) {
    balance++; // unprotected access 
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p;
    Pthread_create(&p, NULL, worker, NULL);
    balance++; // unprotected access
    Pthread_join(p, NULL);
    return 0;
}

```

- Der Thread 0 ist main
- Thread 1 ist der Worker thread durch Pthread_create(...)
- beide greifen gleichzeitig auf die globale Variable balance zu
- helgrind meldet korrekt

- Konflikt 1: main liest balance während worker balance schreibt

- Konflikt 2: main schreibt balance während worker balance schreibt


>2. What happens when you remove one of the offending lines of code?
Now add a lock around one of the updates to the shared variable,
and then around both. What does helgrind report in each of these
cases?


- Wenn man problematischen code rausnimmt, dann meldet helgrind nichts
- 

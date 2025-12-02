1. First build main-race.c. Examine the code so you can see the
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
```
==2375== Possible data race during read of size 4 at 0x10C014 by thread #1
```


- Konflikt 2: main schreibt balance während worker balance schreibt
```
Possible data race during write of size 4 by thread #1

```

2. What happens when you remove one of the offending lines of code? Now add a lock around one of the updates to the shared variable, and then around both. What does helgrind report in each of these cases?

- Wenn man die offending lines wegmacht, gibt das tool keine errors
- Wenn man beide locks hinzufügt, gibts keine race conditions

3. Now let’s look at main-deadlock.c. Examine the code. This code has a problem known as deadlock (which we discuss in much more depth in a forthcoming chapter). Can you see what problem it might have?

- p1 bekommt für die worker funktion als Argument 0, p2 bekommt 1
- p1 bekommt den if Teil, p2 bekommt den else Teil
- p1 kann mittendrin stoppen und zu p2 switshcne und dan wiede rzurück
- ERGEBIS: Beide warten blockieren sich gegenseitig und warten.


4. Now run helgrind on this code. What does helgrind report?

```
==826== Thread #3: lock order "0x10C040 before 0x10C080" violated
``` 


5. Now run helgrind on main-deadlock-global.c. Examine the code; does it have the same problem that main-deadlock.c has? Should helgrind be reporting the same error? What does this tell you about tools like helgrind?

- 2 Threads, die dieselben Locks in untersch, Reihenfolge sperren sollen
- damit kein Deadlock, globaler Mutex genutzt, um kritischen bereich zu schützen
- g thread erlaubt immer nur einem thread, überhaupt in diesen bereich reinzukommen
- jeder thread lockt g immer als erstes, das passiert vor der i fverzweigung

- helgrind macht trotzdem auf problem aufmerksam, da Helgrund due 
-helgridn meldet nur potentielle probleme, nicht garantierte probleme

6. Let’s next look at main-signal.c. This code uses a variable (done)
to signal that the child is done and that the parent can now continue.
Why is this code inefficient? (what does the parent end up spending its time doing, particularly if the child thread takes a long time
to complete?)

- der main (parent thread) prüft mit der while schleife permanent die variable -> verschwnedete CPU Zeit nur zum prüfen
- wenn der child (worker thread) zu lange braucht, gehen amssiv ressourcen verschwendet
- Hinweis: if ist auch keine Lösung, weil die Abfrage nur ein mal erfolgt, und der code danach weitergeht und die Variable done nicht mehr überprüft


7. Now run helgrind on this program. What does it report? Is the code correct?

- Helgrind meldet probleme, weild er eine thread in done schreibt und der andere done liest 
- ungeschützter zugriff -> Datenrennen
- keiner der beuden threads hält nen lock, weswegen der zugriff unsicher ist



8. Now look at a slightly modified version of the code, which is found in main-signal-cv.c. This version uses a condition variable to do the signaling (and associated lock). Why is this code preferred to the previous version? Is it correctness, or performance, or both?

- Der main thread wartet auf den worker, dass es signalisiert, dass es fertig ist 

```
 signal_wait(&s); //im main threAD
 ```

 ```
 	Pthread_cond_wait(&s->cond, &s->lock); //Auschnitt aus signal wait fkt
```

- keine data races, da der zugriff auf s->done immer gelockt ist


9. Once again run helgrind on main-signal-cv. Does it report any errors?
- nein
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "common_threads.h"

// If done correctly, each child should print their "before" message
// before either prints their "after" message. Test by adding sleep(1)
// calls in various locations.

// You likely need two semaphores to do this correctly, and some
// other integers to track things.


typedef struct __barrier_t {
    int count; //zählt, wie viele Threads schon angekommen sind
    int num_threads; //Gesamtzahl der Threads, die im Barrier ankommen müssen
    sem_t mutex;
    sem_t turnstile; // Das eig. Tor, wird nur vom letzten Thread geöffnet, blockert und lässt erst dann alle weiter
} barrier_t;


// the single barrier we are using for this program
barrier_t b;

void barrier_init(barrier_t *b, int num_threads) {
    // initialization code goes here
    sem_init(&b->mutex, 0, 0);
    sem_init(&b->turnstile, 0, 0);
    b->count = 0;
    b->num_threads = num_threads;
}

void barrier(barrier_t *b) {
    sem_wait(&b->mutex);
    b->count++;

    if (b->count == b->num_threads) {
        for (int i = 0; i < b->num_threads; i++) {
            sem_post(&b->turnstile);
        }
    }

    sem_post(&b->mutex);        // ✅ Mutex FREIGEBEN
    sem_wait(&b->turnstile);    // ✅ erst DANACH blockieren
}


//Thread A kommt, nimmt kurz Mutex, erhöht count von 0 auf 1, gibt Mutex sofort wieder frei, geht zum turnstile und wartet dort
//Thread B kommt, nimmt Mutex, erhöht count von ⁻1, gibt Mutex frei, geht zum Turnstile, Turnstile ist zu, wartet dort
// letzter Thread C kommt, nimmt Mutex, erhöht count von 2 auf 3, erkennt, dass er der letzte ist, öffnet das turnstile für alle, gibt mutex frei, geht slebst durch turnstile

    sem_wait(&b->turnstile); //blockier alle

    sem_post(&b->mutex);
}

//
// XXX: don't change below here (just run it!)
//
typedef struct __tinfo_t {
    int thread_id;
} tinfo_t;

void *child(void *arg) {
    tinfo_t *t = (tinfo_t *) arg;
    printf("child %d: before\n", t->thread_id);
    barrier(&b);
    printf("child %d: after\n", t->thread_id);
    return NULL;
}


// run with a single argument indicating the number of 
// threads you wish to create (1 or more)
int main(int argc, char *argv[]) {
    assert(argc == 2);
    int num_threads = atoi(argv[1]);
    assert(num_threads > 0);

    pthread_t p[num_threads];
    tinfo_t t[num_threads];

    printf("parent: begin\n");
    barrier_init(&b, num_threads);
    
    int i;
    for (i = 0; i < num_threads; i++) {
	t[i].thread_id = i;
	Pthread_create(&p[i], NULL, child, &t[i]);
    }

    for (i = 0; i < num_threads; i++) 
	Pthread_join(p[i], NULL);

    printf("parent: end\n");
    return 0;
}


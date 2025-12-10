1. Examine flag.s. This code “implements” locking with a single memory
flag. Can you understand the assembly?
- Lock if it’s unlocked, or spin-wait if it’s not. After critical section, check if we have to do this again, if not then stop.

2. When you run with the defaults, does flag.s work? Use the -M and -R
flags to trace variables and registers (and turn on -c to see their values).
Can you predict what value will end up in flag?
```
python3 x86.py -p flag.s -M flag,count -R ax,bx -C -c
```
```
flag count      ax    bx   >= >  <= <  != ==        Thread 0                Thread 1
    0     0       0     0   0  0  0  0  0  0 
    0     0       0     0   0  0  0  0  0  0  1000 mov  flag, %ax
    0     0       0     0   1  0  1  0  0  1  1001 test $0, %ax
    0     0       0     0   1  0  1  0  0  1  1002 jne  .acquire
    1     0       0     0   1  0  1  0  0  1  1003 mov  $1, flag
    1     0       0     0   1  0  1  0  0  1  1004 mov  count, %ax
    1     0       1     0   1  0  1  0  0  1  1005 add  $1, %ax
    1     1       1     0   1  0  1  0  0  1  1006 mov  %ax, count
    0     1       1     0   1  0  1  0  0  1  1007 mov  $0, flag
    0     1       1    -1   1  0  1  0  0  1  1008 sub  $1, %bx
    0     1       1    -1   0  0  1  1  1  0  1009 test $0, %bx
    0     1       1    -1   0  0  1  1  1  0  1010 jgt .top
    0     1       1    -1   0  0  1  1  1  0  1011 halt
    0     1       0     0   0  0  0  0  0  0  ----- Halt;Switch -----  ----- Halt;Switch ----- 
    0     1       0     0   0  0  0  0  0  0                           1000 mov  flag, %ax
    0     1       0     0   1  0  1  0  0  1                           1001 test $0, %ax
    0     1       0     0   1  0  1  0  0  1                           1002 jne  .acquire
    1     1       0     0   1  0  1  0  0  1                           1003 mov  $1, flag
    1     1       1     0   1  0  1  0  0  1                           1004 mov  count, %ax
    1     1       2     0   1  0  1  0  0  1                           1005 add  $1, %ax
    1     2       2     0   1  0  1  0  0  1                           1006 mov  %ax, count
    0     2       2     0   1  0  1  0  0  1                           1007 mov  $0, flag
    0     2       2    -1   1  0  1  0  0  1                           1008 sub  $1, %bx
    0     2       2    -1   0  0  1  1  1  0                           1009 test $0, %bx
    0     2       2    -1   0  0  1  1  1  0                           1010 jgt .top
    0     2       2    -1   0  0  1  1  1  0                           1011 halt


```

- When we run it with defaults, flag.s will work fine. Since the default number of threads is set to 2, the program will run for 2 times and update the value ax to 2.
- final value in flag is 0, bc we always reset it 



3. Change the value of the register %bx with the -a flag (e.g., -a bx=2,bx=2
if you are running just two threads). What does the code do? How does it
change your answer for the question above?

´´´
$ ./x86.py -p flag.s -M flag,count -R ax,bx -c -a bx=2,bx=2
´´´

- flag is still zero
- each thread will loop for 2 times since bx is 2
- ax is 2+2 = 4

4. Set bx to a high value for each thread, and then use the -i flag to generate
different interrupt frequencies; what values lead to a bad outcomes? Which
lead to good outcomes?

- good outcomes:
- keine zwei threads sind gleichzeitig in der critical section
- expected outcome ist bx1 + bx2 z.B 50+50 = 100
- weil es 11 instruktionen sind, muss interrupt ein vielfaches von 11 sein
- wenns kein vielfaches von 11 ist, verschiebt sich alles und interurpts treten an vershc. stellen mit der zeit auf


5. Now let’s look at the program test-and-set.s. First, try to understand
the code, which uses the xchg instruction to build a simple locking primitive. How is the lock acquire written? How about lock release?

```
xchg %ax, mutex 
```
- tauscht den wert von ax mit mutex
```
mov  $0, mutex
```
- tut einfach 0 in mutex

6. Now run the code, changing the value of the interrupt interval (-i) again,
and making sure to loop for a number of times. Does the code always work
as expected? Does it sometimes lead to an inefficient use of the CPU? How
could you quantify that?

- sehr korrket und deterministisch
- cpu zeit verschwnedung weils spinlock ist
- quantifizierbar anhand anzahl ausgeführter befehle, kontextwehsel pro erfolgreicher inkrement operation


7. Use the -P flag to generate specific tests of the locking code. For example,
run a schedule that grabs the lock in the first thread, but then tries to acquire
it in the second. Does the right thing happen? What else should you test?

```
python3 ./x86.py -p test-and-set.s -M count,mutex -R ax -a bx=3,bx=3 -P 0011111 -c
```

- thread 0 nimmt zuerst den lock, danach kann thread 1 arbeiten
- thread 1 spinnt einfach solange thread 0 den lock hat
- im grundegenommen hat die riehenfolge keine ausiwrkung aufs ergebnis, irgendwer wird halt unlucky und spinnt halt etwas

8. Now let’s look at the code in peterson.s, which implements Peterson’s
algorithm (mentioned in a sidebar in the text). Study the code and see if
you can make sense of it.
 
- wir laden die basisadresse von flag in fx

```
spin1
mov 0(%fx,%cx,4), %ax   # flag[1-self]
test $1, %ax            
jne .fini               # if flag[1-self] != 1, skip past loop to .fini


```

- wir laden den flag[other] in ax
- wenn flag[otther]!= 1, also der andere will nicht rein, sind wir frei, spring zu fini

```
.spin2                  # just labeled for fun, not needed
mov turn, %ax
test %cx, %ax           # compare 'turn' and '1 - self'
je .spin1               # if turn==1-self, go back and start spin again

```

- lade turn
- prüfe ob turn == (1-self)
- wenn ja, gehe zurück zu spi, wenn nein dürfen wir weiter und fallen aus spin ehraus



- Man muss warten, solange beides gilt:

- other wants in → flag[other] == 1

- other has priority → turn == other

basically dasselbe wie: 

```

while (flag[other] == 1 && turn == other) {
    // busy wait
}
```

9. Now run the code with different values of -i. What kinds of different behavior do you see? Make sure to set the thread IDs appropriately (using -a
bx=0,bx=1 for example) as sthe code assumes it.

- count bleitb immer korrekt
- nur wenn man i klein wählt, passieren oft interrupts

10. Can you control the scheduling (with the -P flag) to “prove” that the code
works? What are the different cases you should show hold? Think about
mutual exclusion and deadlock avoidance.

- mutual excvlusion:
```
$ ./x86.py -p peterson.s -M count,flag,turn -R ax,cx -a bx=0,bx=1 -c -P 0,1,0,1,0,1
```
um zu testen, ob beide gleichze9tg rein wollen, muss ich abwechselnd scheduling geben, sodass beide ihr flag setzen und turn other setzen. dafür brauch ich schedules, die beiden threads abwechselnd eine isnturktion geben. so kommen beide in den wettkampfzustand. nur einer wird die cs betreten -> mutual exclusion

- keine deadlocks:
```
$ ./x86.py -p peterson.s -M count,flag,turn -R ax,cx -a bx=0,bx=1 -c -P 0,1,1,1,1,1,0 
```
um deadlocks auszuschleissen, simuliert man den worst case
scheduler gibt fast nur einem thread rechenzeit, während der andere nie sauber au der spin schleife rauskommt.
erst 0, damit thread 0 überhaupt glag = 1 setzem kann
dann viele 1 damit thread 1 dominiert
am ende wieder 0, damit man sieht = trotz unfairness kommt 0 irgendwann weiter
wenn beide nie aus der spin schleife kämen -> deadlock, tun sie aber nicht

11. Now study the code for the ticket lock in ticket.s. Does it match the code
in the chapter? Then run with the following flags: -a bx=1000,bx=1000
(causing each thread to loop through the critical section 1000 times). Watch
what happens; do the threads spend much time spin-waiting for the lock?

```
fetchadd %ax, ticket
```

lies den aktuellen wert von ticket
schriebe diesen alten wert in ax
erhöhe ticket um ax
ticket = ticket + ax

```
./x86.py -p ticket.s -M count,ticket,turn -R ax,bx,cx -a bx=1000,bx=1000 -c
```

- jeder thread führt cirtical ection 10000 mal aus
- beide threads greifen also 1000 mal auf lock zu
- jeder thread läuft nur ein einziges mal durch die cirtical section prio zeitscheibe, danahc lock sofort frei, dann wartet er, bis urn wieder zu seinem ticket wird
- wenn thread 0 die cs macht, st thread 1 immer in .tryagain weil 
turn == N
my_ticket == N+1 → nicht dran
- jedes mal müssen beide einmal warten


12. How does the code behave as you add more threads?
- dasselbe. mehr threads machen wenig nützliches per time slice

13. Now examine yield.s, in which a yield instruction enables one thread
to yield control of the CPU (realistically, this would be an OS primitive, but
for the simplicity, we assume an instruction does the task). Find a scenario
where test-and-set.s wastes cycles spinning, but yield.s does not.
How many instructions are saved? In what scenarios do these savings arise?

```
xchg %ax, mutex
```
schreibt den wert von ax in mutex
 gib den alten wert von mutex in ax zurück




14. Finally, examine test-and-test-and-set.s. What does this lock do?
What kind of savings does it introduce as compared to test-and-set.s?

t tests to see if mutex == 0 before attempting the xchg instruction. This behaves more like a compare-and-swap. The win here is that we're only writing to mutex when there's a fair chance we'll get the lock. That means less bus traffic and better cache coherance.

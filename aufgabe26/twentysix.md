| Flag | Zweck |
|------|--------|
| -p   | Programm laden |
| -t   | Anzahl Threads |
| -a   | Register initialisieren |
| -R   | Register verfolgen |
| -M   | Memory verfolgen |
| -C   | Condition-Codes anzeigen |
| -c   | Werte berechnen |
| -s   | Zufalls-Seed setzen |
| -i   | Interrupt-Frequenz |
| -r   | Zufällige Interrupts |
| -L   | Ladeadresse des Codes |
| -m   | Größe des Adressraums |
| -S   | Extra-Statistiken |
| -h   | Hilfe anzeigen |




>1. Let’s examine a simple program, “loop.s”. First, just read and understand it. Then, run it with these arguments (./x86.py -t 1
-p loop.s -i 100 -R dx) This specifies a single thread, an interrupt every 100 instructions, and tracing of register %dx. What
will %dx be during the run? Use the -c flag to check your answers;
the answers, on the left, show the value of the register (or memory
value) after the instruction on the right has run


```
.main
.top
sub  $1,%dx      ; Jede Runde um 1 verringert DX = DX - 1
test $0,%dx      ; setzt Flags entsprechend DX, prüft ob DX null, positiv, negativ ist
jgte .top        ; springe zu .top, wenn DX >= 0
halt
```

gleichzusetzen mit:
```while(dx >= 0){
    dx = dx-1;
}
halt();

```
## Lösung:
```
dx          Thread 0
    0
    -1   1000 sub  $1,%dx
    -1   1001 test $0,%dx
    -1   1002 jgte .top
    -1   1003 halt
```

>2. Same code, different flags: (./x86.py -p loop.s -t 2 -i 100
-a dx=3,dx=3 -R dx) This specifies two threads, and initializes
each %dx to 3. What values will %dx see? Run with -c to check.
Does the presence of multiple threads affect your calculations? Is
there a race in this code?

```

 dx          Thread 0                Thread 1
    3
    2   1000 sub  $1,%dx
    2   1001 test $0,%dx
    2   1002 jgte .top
    1   1000 sub  $1,%dx
    1   1001 test $0,%dx
    1   1002 jgte .top
    0   1000 sub  $1,%dx
    0   1001 test $0,%dx
    0   1002 jgte .top
    -1   1000 sub  $1,%dx
    -1   1001 test $0,%dx
    -1   1002 jgte .top
    -1   1003 halt
    3   ----- Halt;Switch -----  ----- Halt;Switch -----
    2                            1000 sub  $1,%dx
    2                            1001 test $0,%dx
    2                            1002 jgte .top
    1                            1000 sub  $1,%dx
    1                            1001 test $0,%dx
    1                            1002 jgte .top
    0                            1000 sub  $1,%dx
    0                            1001 test $0,%dx
    0                            1002 jgte .top
    -1                            1000 sub  $1,%dx
    -1                            1001 test $0,%dx
    -1                            1002 jgte .top
    -1                            1003 halt

```


>3. Run this: ./x86.py -p loop.s -t 2 -i 3 -r -R dx -a
dx=3,dx=3 This makes the interrupt interval small/random; use
different seeds (-s) to see different interleavings. Does the interrupt frequency change anything?

- Nach jeder ausgeführten Instruktion entscheidet -r zufällig, ob ein Interrupt passiert oder nicht. 
- Interleaving wird komplett zufällig.
- Der Zustand des Registerwerts dx ist nicht deterministisch.



>4. Now, a different program, looping-race-nolock.s, which accesses a shared variable located at address 2000; we’ll call this variable value. Run it with a single thread to confirm your understanding: ./x86.py -p looping-race-nolock.s -t 1 -M
2000 What is value (i.e., at memory address 2000) throughout the
run? Use -c to check.

```

/x86.py -p looping-race-nolock.s -t 1 -M

 2000          Thread 0
    0
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   1003 sub  $1, %bx
    1   1004 test $0, %bx
    1   1005 jgt .top
    1   1006 halt
```
Der Wert an der Adresse 2000 wird erst 1, wenn wirs auch wirklich wieder zurückschreiben.

>5. Run with multiple iterations/threads: ./x86.py -p
looping-race-nolock.s -t 2 -a bx=3 -M 2000 Why does
each thread loop three times? What is final value of value?

```
./x86.py -p looping-race-nolock.s -t 2 -a bx=3 -M 2000

2000          Thread 0                Thread 1
    0
    0   1000 mov 2000, %ax
    0   1001 add $1, %ax
    1   1002 mov %ax, 2000
    1   1003 sub  $1, %bx
    1   1004 test $0, %bx
    1   1005 jgt .top
    1   1000 mov 2000, %ax
    1   1001 add $1, %ax
    2   1002 mov %ax, 2000
    2   1003 sub  $1, %bx
    2   1004 test $0, %bx
    2   1005 jgt .top
    2   1000 mov 2000, %ax
    2   1001 add $1, %ax
    3  1002 mov %ax, 2000
    3  1003 sub  $1, %bx
    3   1004 test $0, %bx
    3   1005 jgt .top
    3   1006 halt
    3   ----- Halt;Switch -----  ----- Halt;Switch -----
    3                            1000 mov 2000, %ax
    3                            1001 add $1, %ax
    4                            1002 mov %ax, 2000
    4                            1003 sub  $1, %bx
    4                            1004 test $0, %bx
    4                            1005 jgt .top
    4                            1000 mov 2000, %ax
    4                            1001 add $1, %ax
    5                            1002 mov %ax, 2000
    5                            1003 sub  $1, %bx
    5                            1004 test $0, %bx
    5                            1005 jgt .top
    5                            1000 mov 2000, %ax
    5                            1001 add $1, %ax
    6                            1002 mov %ax, 2000
    6                            1003 sub  $1, %bx
    6                            1004 test $0, %bx
    6                            1005 jgt .top
    6                            1006 halt

```

- Wir haben in dem Fall keine race conditions
- Threads laufen chronologisch
- keine Probleme beim inkrementieren

> 6. Run with random interrupt intervals: ./x86.py -p
looping-race-nolock.s -t 2 -M 2000 -i 4 -r -s 0 with
different seeds (-s 1, -s 2, etc.) Can you tell by looking at the
thread interleaving what the final value of value will be? Does the
timing of the interrupt matter? Where can it safely occur? Where
not? In other words, where is the critical section exactly?

- Das Timing ist sehr entscheidend
- Wenn der neue Wert es schafft, in 2000 geschrieben zu werden, wird der nächste Thread mit diesem Wert auch weiterarbeiten
- Wenn nicht, arbeitet man mit einem veralteten Wert -> ungewollt
- kritische Sektion ist Bereich von read -> add -> write

> 7. Now examine fixed interrupt intervals: ./x86.py -p
looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 1 What
will the final value of the shared variable value be? What about
when you change -i 2, -i 3, etc.? For which interrupt intervals
does the program give the “correct” answer?

./x86.py -p looping-race-nolock.s -a bx=1 -t 2 -M 2000 -i 1

- Wenn Interrupt nach dem write an Adresse stattfindet, ist's richtig
- correct answers ab 3, weil da alle 3 assembly instruktionen von read -> add -> write abgedeckt sind

> 8. Run the same for more loops (e.g., set -a bx=100). What interrupt intervals (-i) lead to a correct outcome? Which intervals are
surprising?

./x86.py -p looping-race-nolock.s -a bx=100 -t 2 -M 2000 -i 1,2,3... -c

| Interrupt-Intervall (-i) | Endwert | Korrekt? |
|--------------------------|---------|----------|
| 1                        | 100     | ❌        |
| 2                        | 100     | ❌        |
| 3                        | 200     | ✅        |
| 4                        | 150     | ❌        |
| 5                        | 160     | ❌        |
| 6                        | 200     | ✅        |
| 7                        | 157     | ❌        |
| 8                        | 150     | ❌        |
| 9                        | 200     | ✅        |
| 10                       | 160     | ❌        |
| 11                       | 164     | ❌        |
| 12                       | 200     | ✅        |
| 13                       | 160     | ❌        |
| 14                       | 158     | ❌        |
| 15                       | 200     | ✅        |
| 16                       | 164     | ❌        |
| 17                       | 166     | ❌        |
| 18                       | 200     | ✅        |
| 19                       | 163     | ❌        |
| 20                       | 160     | ❌        |



- Die kritische Sektion umfasst genau 3 INstruktionen, deswegen funktionieren nur interrupt interfalle, die ein Vielfaches von 3 sind.
- sonst passieren interrupts mitten in einem read, add oder write, was nen race verursacht


> 9. One last program: wait-for-me.s. Run: ./x86.py -p
wait-for-me.s -a ax=1,ax=0 -R ax -M 2000 This sets the
%ax register to 1 for thread 0, and 0 for thread 1, and watches %ax
and memory location 2000. How should the code behave? How is
the value at location 2000 being used by the threads? What will its
final value be?

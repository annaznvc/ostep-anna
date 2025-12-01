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

dx          Thread 0
    0
    -1   1000 sub  $1,%dx
    -1   1001 test $0,%dx
    -1   1002 jgte .top
    -1   1003 halt


>2. Same code, different flags: (./x86.py -p loop.s -t 2 -i 100
-a dx=3,dx=3 -R dx) This specifies two threads, and initializes
each %dx to 3. What values will %dx see? Run with -c to check.
Does the presence of multiple threads affect your calculations? Is
there a race in this code?



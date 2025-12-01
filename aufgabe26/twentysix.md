>1. Let’s examine a simple program, “loop.s”. First, just read and understand it. Then, run it with these arguments (./x86.py -t 1
-p loop.s -i 100 -R dx) This specifies a single thread, an interrupt every 100 instructions, and tracing of register %dx. What
will %dx be during the run? Use the -c flag to check your answers;
the answers, on the left, show the value of the register (or memory
value) after the instruction on the right has run


```
.main
.top
sub  $1,%dx      ; DX = DX - 1
test $0,%dx      ; setzt Flags entsprechend DX
jgte .top        ; springe zu .top, wenn DX >= 0
halt
```

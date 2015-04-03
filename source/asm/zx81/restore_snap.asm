

                org 16444+25

                pop hl
                pop de
                pop bc
                pop af
                jr  n1
                halt
n1              ld sp, pila
                out ($fd),a
                ei
                jp  0
pila            defw 0,0,0,0

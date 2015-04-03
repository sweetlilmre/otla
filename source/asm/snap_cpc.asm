
                org  $ff00
         

snap            ld  de,$ff58
                ld  hl,mem
                ld  bc,$a8

                ldir

                ld  sp,hl

                pop hl
                pop de
                pop bc
                pop af

                exx
                ex  af,af'

                pop iy
                pop ix
                pop hl
                pop de

                
                im  0
                
                ld  a,1
                ld  i,a
                ld  a,1                
                ld  r,a

        ld a, $c0     ; pagina 0*
        ld b, $7f     ; OUT &7Fxx, ; %11bbbmmm   ;   actualizar ram mode 
        out (c), a  

                pop bc               
                pop af
                
                ld  sp, $5c01 

                di 
                jp salta // reti

mem
                

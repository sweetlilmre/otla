
                org  $8000
         
       

        ld a, $c4  ; pagina 0*
        ld b, $7f     ;OUT &7Fxx, ; %11bbbmmm   ;   actualizar ram mode 
        out (c), a        
                 
                ld  hl,mem
                ld  de,$4000
                ld  bc,$4000

                ldir

        ld a, $c0  ; pagina 0*
        ld b, $7f     ;OUT &7Fxx, ; %11bbbmmm   ;   actualizar ram mode 
        out (c), a  

                ret

mem		nop

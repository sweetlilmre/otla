
                org  $5b00
         

snap            ld  de,$ff58
                ld  hl,$5b40
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
                
                
                pop  af
                ld  i,a
                rra                
                ld  r,a
              
                ld  a,$7
                out ($fe),a

                ld bc,$1ffd
                ld a,0
                out (c),a 


                ld bc, $7ffd
                ld a,0
                out (c),a

                pop bc               
                pop af
                
                im  0
                ld  sp, $5555 
                
                di 
                jp $4444; // reti

mem
                

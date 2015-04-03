
                org  $5b00
         

                ld   a,7
                ld   bc , $7ffd
                out  (c),a             
                 
                ld  de,$c000
                ld  hl,mem
                ld  bc,$4000

                ldir

                ld a,0

                ld bc $7ffd
                out (c),a

                ret

mem		nop

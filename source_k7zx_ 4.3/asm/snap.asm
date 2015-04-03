
                org  $4900 + 29
         

snap            ld  de,$ff00
                ld  hl,18400
                ld  bc,$100
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

                pop af
                im  0
                jr  z,bb
                im  1
                jr  nc,bb
                im  2

bb              out ($fe),a
                rla

                pop bc
                ld  a,b
                ld  i,a
                ld  a,c                
                ld  r,a

                pop bc
                jr  c , en_in

                pop af
                ld  sp, ($4900 + 24)
                jr  fin

en_in           pop af
                ld  sp,($4900 + 24)
                ei
fin             jr  snap-3 ;jp retorno

retorno  nop ;ret


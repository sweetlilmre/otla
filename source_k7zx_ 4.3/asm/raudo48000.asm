

        org $fdbf - 21


CTES
        defb MAX-CTES
        defb K0-CTES
        defb K1-CTES
        defb  pos_EI -CTES
        defb  SYNC -CTES

inicio  ld hl, 23797
        ld de, ini2
        ld b,2
        ldir
MAX
        ld hl,$3fff
        jp pilot1

ini2
         org $fdbf   ;;;;;;;;;;;;;;;;;;;;


         inc  h
         jr   nc , NO_FULL

         xor  b
         ld   (de),a
         inc  de
         ld   a,4
         ex   af,af'    ; 4+ 7 +  28 = 39

         in   l, (c)
         jp   (hl)

NO_FULL
         or   b
         add  a,a
         jr   c,  fin
         add  a,a
         ex   af,af'  ; 4 + 12 + 23  = 39
         in   l, (c)
         jp   (hl)

trozo
         ld   a,R
         ld   l,a
         ld   b,(hl)
         xor  a
         ld   R,a
         dec  h
         ex   af,af'    ;  39
         in   l, (c)
         jp   (hl)

fin
pos_EI
        ei
        jp   7997  ;ret   z ;rst 8 ;defb 26

pilot1  push  hl
        ld    hl,trozo
        ld    de, $febf
        ld    bc, 12          
        ldir
                             
        ld    de,$feff + 12  
        ld    c, 13
        lddr

        ld   hl, $1068
        ld   bc, $edfe

        jr    pilot2




      org  $fdff  ;;;;;;;;;;;;;;;;;;;;


         inc  h
         jr   nc , NO_FULL

         xor  b
         ld   (de),a
         inc  de
         ld   a,4
         ex   af,af'    ; 4+ 7 +  28 = 39

         in   l, (c)
         jp   (hl)

k0
k1

        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80

        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80

        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80

        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80

        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80

        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80

        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
   

        defb $ff,$ff,$ff,$ff


        



         org $fe00 + 80  ;;;;;;;;;;;;;;;;;;;;



pilot2  
        di
        xor   a
        ex    af,af'
        pop   de

pii 
        out  (c),b
        xor  a

pi_LOW  inc a
        in   f,(c)
        jp   po, pi_LOW

        out  (c),l

pi_HIGH sub c
        in f,(c)
        jp pe, pi_HIGH

SYNC        cp  74   ;29
        rl h
        cp 54        ;19
        jr nc, pii

        inc h
        jr nz , pii

        cp  34  ;8

        ld a,b
        ld h,$e9          ; 68
        ld b,0x10        ;  7
 
        jr nc,inv
                         ;  7

        ld ($fdbf),a     ; 13
        ld ($fdc0),hl    ; 16
        ld ($ff00),hl    ; 16
       ; ld b,0x10        ;  7
        ld  h,$fe        ;  7
        jr  entra0       ; 12  = 71
                         ; +20
             ; 68+7+71+20=166


inv                      ; 12
        ld ($fdff),a
        ld ($fe00),hl
        ld ($fec0),hl
      ;  ld  b,0x10
        ld  h,$fe
        jr  entra0         
                         ; +20

           ; 68+12+71+20=171




       org $febf+4
entra0



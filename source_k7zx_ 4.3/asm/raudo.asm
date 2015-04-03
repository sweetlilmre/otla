

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
         jr  C,FULL       

NO_FULL  or   b
         add  a,a
         jr   c,  fin
         add  a,a
         ex   af,af'  
k0       out  ($fe),a      ;4 + 7 + 34 = 45
         in   l, (c)
         jp   (hl)
FULL
         xor  b
         ld   (de),a
         inc  de
         ld   a,4
         ex   af,af'    
         scf             ; 4+ 12 + 32 = 48

         in   l, (c)
         jp   (hl)


trozo
         ld   a,R
         ld   l,a
         ld   b,(hl)
         ld   a,4  
         ld   R,a
         adc  a,a
         ex   af,af'    
         dec  h           ; 48
         in   l, (c)
         jp   (hl)


pilot1  di
        xor a
        push  hl
        ld    hl,trozo
        ld    de, $febf
        ld    bc, 12+2;          
        ldir
                             
        ld    de,$feff + 12+2  
        ld    c, 13+2
        lddr
        
        ld   hl, $e968

        jr    pilot2


        org  $fdff  ;;;;;;;;;;;;;;;;;;;;


         inc  h
         jr  C,FULL 


;NO_FULL
         or   b
         add  a,a
         jr   c,  fin
         add  a,a
         ex   af,af'  
         out  ($fe),a 

         in   l, (c)
         jp   (hl)

fin
pos_EI
        ei
        jp   7997  ;ret   z ;rst 8 ;defb 26

        defb  0
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

       
        defb  0x80,0x80,0x80
pilot2
;       defb  0x80,0x80,0x80
        ld  e,$ed
        defb 0x80 

;       defb 0x80,0x80,0x80
        ex   af,af'
        ld  d,l   ;pop  de
        defb  0x80

;        defb 0x80,0x80,0x80
        ld  b,$10
        defb 0x80 

;        defb  0x80,0x80,0x80

        ld  c,$fe

         org $fe00 + $80   ;;;;;;;;;;;;;;;;;;;;

  


       

pii 
        out  (c),e
        xor  a

pi_LOW  inc a
        in   f,(c)
        jp   po, pi_LOW

        out  (c),l

pi_HIGH sub c
        in f,(c)
        jp pe, pi_HIGH

SYNC        cp  74   
        rl d
        cp 54        
        jr nc, pii

        inc d
        jr nz , pii

        cp  34  ;8

        ld a,e
        pop de 
 
        jr nc,inv
                         ;  7

        ld ($fdbf),a     ; 13
        ld ($fdc0),hl    ; 16
        ld ($ff00),hl    ; 16
entra1        ld  h,$fe        ;  7
        jr  entra0       ; 12  = 71
                         ; +20
             ; 75+7+71+20=166


inv     in  f,(c)
        jp  po , inv                 ; 12
        ld ($fdff),a
        ld ($fe00),hl
        ld ($fec0),hl
        jr  entra1         
                         ; +20

           ; 75+12+71+20=171




       org $febf+4
entra0



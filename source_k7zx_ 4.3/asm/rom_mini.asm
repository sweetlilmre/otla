
        org  65320 ;

CTES
        defb MAX-CTES
        defb K0-CTES
        defb K1-CTES
        defb pos_EI -CTES
        defb SYNC -CTES
 

        ld hl, 23797
        ld de, inicio
        ld b,1
        ldir
k1
MAX     ld hl,$3fff
        jp pilot

inicio
pilot   di
        ld  bc, $0efe
        ld  de, $10ea

pii     xor a
        out (c),e

pi_LOW  inc  a
        in  f,(c)
        jp  po, pi_LOW
        out (c),b
pi_HIGH sub c
        in  f,(c)
        jp  pe, pi_HIGH

SYNC    cp 74   
        rl  d
        cp  54     
        jr  nc, pii 
        inc d
        jr  nz , pii  

         cp  34 
         ld  a,e       

        jr  c, noinv

Ps      in  f,(c)
        jp  po, ps

         xor 8
noinv   
        ld  (pe1 ),a 
         xor 8 ;     
        ld  (po1 ),a     
           
K0      ld  de,$e0f2
        ld  b,$3f

        JR  entra ;: SIGUE



FULL   ld  (hl),d
       inc hl
       ld  d,C        ;  12 + 17 = 29                                          
entra
SIGUE  xor a  
       ld  R,a        ; 13

HIGH   in  f,(c)
pe1    jp  pe , HIGH 

       Ld a,R    
       add a,e
       rl  d          ;

LOW    in  f,(c)
po1    jp  po , LOW   ; 22
       
       jr  Nc, FULL

       ld  a,R       
       cp  b
       jr  c, SIGUE   ; 7+ 25 = 32
              
fin    
pos_EI 
       ei
       jp rand

rand nop

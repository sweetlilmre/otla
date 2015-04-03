
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

MAX     ld hl,$3fff
        jp pilot

inicio
pilot   di
        ld  de, $10e8
        ld  bc, $affe

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
           
        ld  d,e    

        JR  entra 



FULL   ld  (hl),d
       inc hl
       ld  d,C      ;  12 + 17 = 29                    

SIGUE       
poo    in  f,(c)
       jp  po , poo   ; 22

      
k1     cp  $1f
       jr  nc, fin

       rra
       or  e
       ld  b,a
entra
       xor a  
       ld  R,a       ; 37

poe    in  f,(c)
       jp  pe , poe 

       Ld a,R
       
k0     cp  $8
       ccf
       rl  d          ; 27

       jr  Nc, full
      
      
       out (c),b     ; 
       jr  c, SIGUE   ; 7+ 24 = 31

              
fin    
pos_EI 
       ei
       jp rand

rand nop

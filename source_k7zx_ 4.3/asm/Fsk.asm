 
         org 65320     


CTES
        defb MAX-CTES
        defb K0-CTES
        defb K1-CTES 
        defb pos_EI -CTES 
        defb SYNC -CTES

inicio  ld hl, 23797
        ld de, tabla
        ld b,1
        ldir
MAX
        ld hl,$3fff
        jp pilot

tabla  
k0
pilot   dec hl
        xor a
        ex  af,af'
        di
        ld  bc, $eefe
        ld  de, $10ea
        
pii     xor a
        out (c),e
pi_LOW  inc a
        in  f,(c)
        jp  po, pi_LOW

        out (c),b
pi_HIGH sub c
        in  f,(c)
        jp  pe, pi_HIGH

SYNC    cp  74
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
noinv    ld  (pe1),a 
         xor 8 ;       
         ld  (po1),a  

         ld  de,$80c8
         xor a
         jr  entra
                
full    
         ex  af,af'
         xor d
         ex  af,af'
         ld  (hl),d     
         inc hl
         ld  d,c       ;10 + 29 = 39
sigue 
HIGH     dec a
         in  f,(c)
pe1      jp  pe, HIGH 
 
         out (c),b 
k1       cp  $f0
         ld  a,b ; c
entra 
         rl  d        ; 12 +7+4+8=  31
                   
LOW      dec  a
         in  f,(c)        
po1      jp  po, LOW               

         jp  nc , FULL 

         out (c),e
         cp  e
         jr  nc , sigue  ;  10 + 28 = 38

         ex  af,af'
         or  a  
pos_EI 
fin      
acaba    ei
         jp  randomizeusr ;ret   z
                         ;rst 8
                         ;defb 26

     

         
randomizeusr nop

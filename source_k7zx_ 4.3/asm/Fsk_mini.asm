 
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
        di
        ld  bc, $cefe
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

k1       ld  de,$806d

         jr  entra
                
full     ld   (hl),d  
         inc  hl
         ld   d,c        ;8+12 + 17  = 37     
  
sigue    
HIGH     in  f,(c)
pe1      jp  pe, HIGH 
 
         ld  a,r  
         ld  b,a 
         add a,e
entra 
         
         ld  a,c                
         ld  r,a      ; 30     
         
LOW      in f,(c)        
po1      jp po, LOW  
        
         rl  d 

         jr nc , FULL 
   
         ld  a,$d0
         cp  b      
         jr  nc, sigue   ; 8+ 7 + 23 = 38

fin       xor  a  
pos_EI 
acaba    ei
         jp  randomizeusr ;ret   z
                         ;rst 8
                         ;defb 26

     

         
randomizeusr nop

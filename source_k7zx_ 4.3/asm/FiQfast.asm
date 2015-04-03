             
        org  65320 
  
CTES
        defb MAX-CTES
        defb K0-CTES
        defb K1-CTES
        defb pos_EI -CTES
        defb SYNC -CTES


 
inicio  ld  hl, 23797
        ld  de, pilot
        ld  b,1
        ldir
max 
        ld  hl,$3fff
        jp  pilot

pilot   
        di
        ld  bc, $cefe
        ld  de, $10ea

pii      xor a 
         out (c),e

pi_LOW  inc a
        in  f,(c)
        jp  po, pi_LOW

         out (c),b

pi_HIGH  sub c
         in  f,(c)
         jp  pe, pi_HIGH

SYNC     cp  75  
         rl  d
         cp  54  
         jr  nc, pii

         inc d
         jr  nz , pii   
      
         cp  34  
 

         ld  a,e
         jr  c, noinv

Ps       in  f,(c)
         jp  po, ps

         xor  8
noinv    ld  (pe1),a 
         xor  8 ;       
         ld  (po1),a  
        
k1       ld  b,$28   
         
         jr  entra  


full     ld  (hl),e
         inc hl 
         ld  e,c          ;21 + 10+ 17=48                             
sigue 
HIGH     in  f , (c)
pe1      jp  pe , HIGH  
         
         ld  a,r
entra
         cp  b  
         ccf   
         rl  e

         sub d 
         cp  d
 
k0       ld  a,8 
         ld  R,a             ; 49

LOW      
         in  f,(c)
po1      jp  po, LOW

         ld  a,R
         rl  e  
         ld  d,a           ; 21

         jp  NC , FULL

         cp  b
         ld  b,b 
         jp  c, sigue     ; 21 +10+ 18 =49

FIN    
         
pos_EI 
         ei
         jp randomizeusr ;ret   z
                         ;rst 8
                         ;defb 26

randomizeusr nop

       

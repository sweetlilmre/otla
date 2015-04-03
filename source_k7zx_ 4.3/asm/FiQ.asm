             
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

pilot   di
        ld  a,$cb
        ex  af,af' 
        ld  bc, $cefe
        ld  de, $10ea

pii      xor a 
         out (c),e

pi_LOW   inc a
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
      
k0      ld  b,$88   
        jr  entra  


full     ex af,af'      ;       
         xor 7   
         ld  (hl),e
         inc hl 
         ld  e,c        ;13 +12 +11 +17 +4 =55

sigue    ex af,af'
HIGH     in  f , (c)
pe1      jp  pe , HIGH           
         
         
         ld  a,R
entra 
k1       add a, $48  

         rl  e
         sub d
         sub b
         cp  d
         rl  e     
 
         ld  a,c 
         ld  R,a             ; 57

LOW      in  f,(c)
po1      jp  po, LOW

         ld  a,R
         ld  d,a          ; 13

         jr  NC , FULL

         ex  af,af'
         out (c),a
         cp  d         
         jp  Nc, sigue     ; 13 + 7 + 30+ 4  = 54

FIN    
       
pos_EI 
         ei
         jp randomizeusr ;ret   z
                         ;rst 8
                         ;defb 26

randomizeusr nop

       

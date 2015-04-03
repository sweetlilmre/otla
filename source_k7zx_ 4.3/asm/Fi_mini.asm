             
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
k1
        ld  hl,$3fff
        jp  pilot

pilot   
        dec hl       
        di
        ld  bc, $0efe
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

k0       ld  b, 8        

         ld d,$80
         
        jr  entra  


full     ld  (hl),d
         inc hl 
         ld  d,c         
                              ;13+10+17= 40
sigue    
HIGH     in  f , (c)
pe1      jp  pe , HIGH  
         
         
         ld  a,R
         sub e
         cp  e   
entra  
         ld  a,b;  8 
         ld  R,a            
         rl  d            ; 38

LOW      in  f,(c)
po1      jp  po, LOW

         ld  a,R
         ld  e,a          ; 13

         jp  NC , FULL

         bit  5,a
         jp   z, sigue
                      ; 13+10+18 =41

FIN      xor a
pos_EI 
         ei
         jp randomizeusr ;ret   z
                         ;rst 8
                         ;defb 26

randomizeusr nop

       

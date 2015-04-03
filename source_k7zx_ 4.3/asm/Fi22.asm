             
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
k1      ld  hl,$3fff
        jp  pilot

pilot   
        xor a
        ex  af,af'
        dec hl
       
        ld  ix , $cfc8 
       
        di
        ld  bc, $cefe
        ld  de, $10ea

pii  
         out (c),e
         xor a 

pi_LOW  inc a
        in  f,(c)
        jp  po, pi_LOW

         out (c),b

pi_HIGH sub c
        in  f,(c)
        jp  pe, pi_HIGH

SYNC    cp  75  
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

        ld  ix , $c8cf

        xor  8
noinv   ld  (pe1),a 
        xor 8 ;        
        ld  (po1),a  

        ld  d,$80
        jr  entra  


full     
         ex  af, af'
         xor d
         ex  af, af'

         ld  (hl),d 
         ld  d,c
         inc hl   
      
         ld  b , ixh 
                       ;9 + 12 + 12 +17 +8  = 56

sigue    

HIGH     in  f , (c)
pe1      jp  pe , HIGH          
         
         ld  e,a         
         ld  a, r 
         sub e
         cp  e                  

entra     
         rl  d          

K0       ld  a, 10 
         ld  r,a         ; 45               
       
LOW      in  f,(c)
po1      jp  po, LOW
 

         ld  a,r

         jr  NC , FULL
  
         out (c),b        
         res 7,b  ;bit 6,a
         cp  b    ;ld  b,b 
         ld  b, ixl 

         jp  c,sigue
                      ; 9 +7+ 4+ 12+ 8 + 8 +10 = 58

FIN    
         ex  af,af'
         or  a
pos_EI 
         ei
         jp randomizeusr ;ret   z
                         ;rst 8
                         ;defb 26

randomizeusr nop

       

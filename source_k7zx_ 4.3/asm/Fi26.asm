             
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
         xor a 
         out (c),e

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
        ld  a,e       ;10+7+8+7+7+4+7+7+4= 61

        jr  c, noinv

Ps      in  f,(c)
        jp  po, ps
        ld  ix , $c8cf

        xor  8

noinv   ld  (pe1),a 
        xor 8 ;       
        ld  (po1),a  
 
        xor a      
        scf 
        jr  entra    ;61 +12 + 13+7+13+4+12= 122


full     ex  af, af'
         xor d
         ex  af, af'
         ld  (hl),d
         inc hl    
         ld  d,c           
                       ;4+ 10 +12+ 17= 43

sigue               
HIGH     inc a
         in  f , (c)
pe1      jp  pe , HIGH       

         out (c),b 
         ld  b, ixh 
         sub  e
         cp   e
         rl   d 
                 

k0       ld  a,1  
                      ;  41
entra  
LOW      inc a
         in  f,(c)
po1      jp  po, LOW

         ld   e,a

         jp  NC , FULL
         ld  b, b
         ld  b, ixl 
         cp  $15
         jr  c , sigue
                   ; 4+ 10 +4+ 8 + 7 + 12 = 45

FIN    
         ex  af,af'
         or  a
pos_EI 
         ei
         jp randomizeusr ;ret   z
                         ;rst 8
                         ;defb 26

randomizeusr nop

       

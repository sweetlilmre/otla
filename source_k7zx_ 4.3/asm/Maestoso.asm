
        org 65320;  
  
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

        ld hl,$3fff
        jp pilot


pilot  
max     ld  a, $58
        ex  af, af'
        ld  ixh,$ff 
        dec hl
        di
        ld  bc, $0efe
        ld  de, $10ea

pii     xor a
        out (c),e
pi_LOW  inc a
        in  f,(c)
        jp  po, pi_LOW

        out (c),b
pi_HIGH inc a
        in  f,(c)
        jp  pe, pi_HIGH

SYNC    cp  72
        rl  d
        cp  55
        jr  nc, pii

        inc d
        jr  nz , pii   

        cp  32
        ld  a, $80
        jr  c , entra1

LOWs    in  f,(c)
        jp  po, LOWs 
 
        jr  entra2

sigue1
         out (c),b
k1       ld  b, 23 ;25
bu1      djnz bu1
         ld  b,$A

         in  f,(c)
         jp  pe , HIGH  

entra1   dec e
            
LOW      in  f,(c)
         jp  po, LOW  
     
         ld  ixl,sigue2-$ff00
      
         inc e
         add a,a
         jp  nc, FULL 
no_full
         xor e
         and c
         xor e
         ex  af, af'
         cp  h
         jr  z, fin
         ex  af, af'            
         ld  b,$e 
         jp (ix)  ; 38 +8 = 46

sigue2
          out (c),b
k0        ld  b, 23 ;25
bu2       djnz bu2
          ld  b,$a

          in  f,(c)
          jp  po, LOW  

entra2    dec e
         
HIGH      in  f , (c)
          jp  pe , HIGH         
       
          ld  ixl,sigue1-$ff00

          inc e
          add a,a
          jp  c, no_FULL   ; 22 + 11 +8 +10= 51
FULL     
          xor e
          and c
          xor e
          ld  (hl),a      
          inc hl          
          xor d            
          ld  d,a
          ld  a,c            
    
          jp (ix)  ;37 + 8 = 45
         
fin  
    
        ld  a,d                
        or  a
pos_EI 
        ei
        jp  randomizeusr ;ret   z
                         ;rst 8
                         ;defb 26

randomizeusr nop

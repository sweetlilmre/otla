
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
        ld  a, $80
        jr  c , entra1

LOWs    in  f,(c)
        jp  po, LOWs 
 
        jr  entra2


        ld  b,b
        ld  b,b
        ld  b,b
        ld  b,b
        ld  b,b
        ld  b,b
sigue1
        in  f,(c)
        jp  pe , HIGH  

entra1  dec e
            
LOW     in  f,(c)
        jp  po, LOW  
     
k0      ld  ixl,sigue2-$ff00
      
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
         
        out (c),b
        inc b

        jp (ix)  ; 12+19+16+8 = 55


        ld  b,b
        ld  b,b
        ld  b,b
        ld  b,b
        ld  b,b
        ld  b,b
sigue2
        in  f,(c)
        jp  po, LOW  

entra2  dec e
         
HIGH    in  f , (c)
        jp  pe , HIGH         
       
k1      ld  ixl,sigue1-$ff00

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
         
        ld bc, $08fe         
    
        jp (ix)  ; 12 + 25 + 10+ 8 = 55
         
fin  
    
        ld  a,d                
        or  a
pos_EI 
        ei
        jp  randomizeusr ;ret   z
                         ;rst 8
                         ;defb 26

randomizeusr nop

 
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
pilot   di 
        xor a
        ex af,af' 
        dec hl 
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
         ld  a,e       
         jr  c, noinv

Ps      in  f,(c)
        jp  po, ps

        xor 8
noinv   ld  (pe1),a 
        ld  (pe2),a 
        xor 8 ;       
        ld  (po1),a  
        ld  (po2),a  
         
         ld  b, $e0
         jr  sigue

                
full     ex af,af'
         xor b
         ex af,af'  
         ld   (hl), b 
         inc  hl 
         ld   b,c    ;12 +12 + 17 = 41
sigue 
entra 

LOW1     ld  a,c         

         in f,(c)        
po1      jp po, LOW1

k0       ld  e,$fe
         
HIGH1    inc  a
         inc  e
         in  f,(c)
pe1      jp  pe, HIGH1          
        
         ld  d,$fe

LOW2     inc a
         inc d 
         in f,(c)        
po2      jp po, LOW2 

         cp  0x2a

HIGH2    inc e
         inc d
         in  f,(c)
pe2      jp  pe, HIGH2 

         jr nc ,acaba    

         cp  d
         rl  b
         
         adc  a,d   ;add a,d
         rra
         cp  e
         rl  b

         ld  a,L    ;  43

         jr nc , FULL 

        and $e 
k1      or  $a 
        out ($fe),a       

         jp  sigue  ; 7 + 25 + 10 =  ; 42



acaba    ex af,af'
pos_EI 
fin      
         ei
         jp  randomizeusr ;ret   z
                         ;rst 8
                         ;defb 26

     

         
randomizeusr nop

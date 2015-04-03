 
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
pilot   
        dec hl
        xor a
        ex  af,af'
        di
        ld  bc, $eafe
        ld  de, $10ee


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
        cp 55
        jr  nc, pii

        inc d
        jr  nz , pii 

         cp  32
         ld  a,b       

         jr  c, noinv


Ps      in  f,(c)
        jp  po, ps

         xor 8
noinv    ld  (jf2),a 
         xor 8 ;       
         ld  (jf1),a  

         ld  de,$8060
         ld  a, $8e
         jr  entra
                
full     ex  af,af'
         xor d
         ex  af,af'
         ld  (hl),d     
         inc hl
         ld  d,c        ;12 + 29  = 41

sigue     
           

HIGH     inc de
         in  f,(c)
jf2      jp  pe, HIGH 

         cp  e 
         jr  c , fin   

         rl  e 
k1       ld  e,$73    ;(0x77+0x70)/2
entra    rl  d        ; 4 + 7+8+7+8=  34
                   
LOW      inc de
         in f,(c)        
jf1      jp po, LOW               


         jr nc , FULL 

       
         out ($fe),a     
         out  (c),b 
         jr sigue ;  7 + 12 + 11 +12 = 42

fin 
         ex  af,af'
         or  a  
pos_EI 
     
acaba    ei
         jp  randomizeusr      

         
randomizeusr nop

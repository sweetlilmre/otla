 
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
k0
        ld hl,$3fff
        jp pilot

tabla  

pilot   xor a
        ex af,af'   
        di 
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
         
deb
         ld  d, $e0
         scf
         jr  entra

                
full     ex af,af'
         xor  d
         ex af,af'
         ld   (hl), d   
         inc hl
         ld   d,$fe    ;4 + 12 +32 + 4 = 52
sigue 
 
         ld   a,c

LOW1     inc  a
         in f,(c)        
po1      jp po, LOW1
HIGH1    inc  a
         in  f,(c)
pe1      jp  pe, HIGH1          
        
         cp  40
         jr nc, acaba

         cp  e          
         rl d         
         
         add a,e         
         cp 0x24         
         ccf
         rl d
  
       
entra    ld e,c     ; 53
           
LOW2     inc e
         in f,(c)        
po2      jp po, LOW2 
HIGH2    inc e
         in  f,(c)
pe2      jp  pe, HIGH2       


        ld  a,L 

        jr nc , FULL 
         

        and $e 
k1      or  $a  
        out ($fe),a

        jr  sigue  ;  4 + 7 + 37 + 4=  ; 52

acaba   ex af,af'
        or  a  
pos_EI 
fin      
         ei
         jp  randomizeusr ;ret   z
                         ;rst 8
                         ;defb 26

     

         
randomizeusr nop

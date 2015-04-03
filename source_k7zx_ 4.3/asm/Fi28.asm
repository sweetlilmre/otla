             
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


full     xor d
         ld  (hl),d
         inc hl    
         ld  d,2         ; 10+ 4 + 20 +7 = 41
sigue 
entra  
k0       ld  e,$ff       
         

LOW      inc de
         in  f,(c)
po1      jp  po, LOW          

         
         bit  5,e
         jr nz, fin

         out (c),b 
         ld  b, ixh    
k1       ld  b,b        ;39 

HIGH     dec de
         in  f , (c)
pe1      jp  pe , HIGH    


         jp  C , FULL
         
         ld  b, ixl          
         rl  d   
         jp  sigue     ; 10 + 8 +8 + 10 + 7 = 43

FIN    
         or  a
pos_EI 
         ei
         jp randomizeusr ;ret   z
                         ;rst 8
                         ;defb 26

randomizeusr nop

       

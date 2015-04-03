
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
MAX 
        ld  hl,$3fff
        jp  pilot

pilot   
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
        ld  a,e
        jr  c, no_inv

        xor  8
no_inv  ld  (pe1),a 
        xor  8 ;       
        ld  (po1),a  

k0      ld  de, $801f; $80  
k1      ld b, $8a
 
        jr  entra  
  

full     ld  (hl),d
         inc hl
         ld  d, c   ;12  +10  +17  +4= 43
sigue              
             
         ld  a,e

LOW      inc a  
         in  f,(c)
po1      jp  po, LOW

         add  a,a
         rl  d        
         
         and  2
         xor   b

         out ($fe),a 

entra
         ld  a,e     ; 23  +11 +4= 38

HIGH     inc  a                 
         in  f , (c)
pe1      jp  pe , HIGH
         
         rlca
         rl  d 

         jp  nC , full ; 
          
         rrca
         cp  b         
         jp c,  sigue
                      ; 12 + 10 + 8+ 10+ 4 = 44 

FIN
pos_EI     
         ei
         jp  randomizeusr

randomizeusr nop

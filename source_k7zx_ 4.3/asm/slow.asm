    

        org 65320 ; org $ffff - 187 ;  139 - 42

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
MAX     ld  hl,$3fff
        jp  pilot

pilot   di
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

SYNC        cp  74;  29
        rl  d
        cp  54  ;19
        jr  nc, pii

        inc d
        jr  nz , pii  

        cp  34   ;8;  cp 11
        ld  a,e ; $ea
        jr  c, no_inv 
                    
syn_LOW in  f,(c)
        jp  po, syn_LOW
                
        xor 8
no_inv     
        ld  (pe1),a 
        ld  (pe0),a  ;ld  (jf0),a              
        xor 8
        ld  (po1),a  

        dec hl                
        
        ;//ld de,$40a3  ; x0x0x0x1 -> 10100011
        ld  de, $80d4    ; x1x1x1x0 -> 11010100  d4

K1      ld  b,11          
        xor a               
        ex  af, af'
        ld  a, -52 ; -11   ;  xor a     

syncro  in  f,(c)
;jf0     jp  po, HIGH
pe0     jp  pe, HIGH
        jr  syncro  

 
full    ex  af, af'
        xor d
        ex  af, af'
        ld  (hl),d
        inc hl
        ld  d, e;  //c ;   ; 24 + 12 + 29 + 4 = 69
        
sigue   xor a      
               
LOW     add a,b
        in  f,(c)
po1     jp  po, LOW  ;26  

       
        cp  $c0
        jr  nc , fin  


K0      add a,-11      
                                ;18
HIGH    add a,b
        in  f , (c)
pe1     jp  pe , HIGH       ;  69+26+18+26=

        rla  
        rl  d 
        rla   
        rl  d                                    
 
        jr  nC, full   

        or  c   
        xor e   
        out (c),a  
        jr  sigue 
                          ;24 + 7 + 32 + 4 = 67

fin     ex  af,af'
        or  a   
pos_EI 
        ei       
        jp randomizeusr ;ret   z
                        ;rst 8
                        ;defb 26
         
randomizeusr nop

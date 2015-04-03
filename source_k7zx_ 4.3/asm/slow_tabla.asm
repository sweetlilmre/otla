    

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

pilot   
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

SYNC        cp  74;  29
        rl  d
        cp  54  ;19
        jr  nc, pii

        inc d
        jr  nz , pii  


syn_LOW in  f,(c)
        jp  po, syn_LOW

        cp  34   ;8;  cp 11
        ld  a,e ; $ea


        jr  c, no_inv 

  
        xor 8

syn_hi in  f,(c)
        jp  pe, syn_hi                    
              
no_inv     
        ld  (pe1),a              
        xor 8
        ld  (po1),a  
              
          
        ld  ixl,d

        ex  de,hl


        ld  b,11          
        xor a    
        jr  entra
K1
tabla 
       defb $fc,$40,$40,$40,$40
       defb $40,$40,$40,$40,$40
       defb $40,$40,$40,$40,$40
       defb $40,$40,$40,$40,$40
       defb $40,$40,$40,$40,$40
       defb $40,$40,$40,$40
 
full   
        and  H
        ld   (de),a
        inc  de
        xor  ixl
        ld   ixl,a
        ld   a, c;      ;  10 + 37 = 47
sigue    

entra               
                               
HIGH    inc  hl
        in  f , (c)
pe1     jp  pe , HIGH      

        ld h,$ff
        ld h,(hl)


k0      ld l,tabla-$ff00    
       
        rlca
        rla     ;29

LOW     inc  hl
        in  f,(c)
po1     jp  po, LOW  ;28    
                      ;  48+28+29+28=133

        
        jp  nC, full   

        dec  b   
        set  1,b  
        out (c),b 
        and  H 
        jp  m, sigue 
                          ;10 + 28 +10= 48

fin     ld a,ixl
        or  a   
pos_EI 
        ei       
        jp randomizeusr ;ret   z
                        ;rst 8
                        ;defb 26

         
randomizeusr nop

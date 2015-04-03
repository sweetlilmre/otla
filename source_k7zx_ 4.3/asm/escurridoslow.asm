          

        org 65320

CTES
        defb MAX-CTES
        defb K0-CTES
        defb K1-CTES
        defb pos_EI -CTES
        defb SYNC -CTES


inicio  ld  hl, 23797
        ld  de,  pilot
        ld  b,1
        ldir

max
        ld  hl,$3fff
        jp  pilot

pilot   
        di

        ld  de, $10ea
        jr  pilot2
       
tabla   
k0
         defb $fd,$fd,$fd
         defb $fd,$fd,$fd,$fd,$fd
         defb $fd,$fd,$fd,$fd,$fd
         defb $fd,$fd,$fd,$fd,$fd
         defb $fd,$fd,$fd,$fd,$fd
         defb $fd,$fd,$fd,$fd,$fd
         defb $fd,$fd

pilot2   ld  bc, $cefe
 

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
        jr nz , pii 

        cp  34       

        ld  a,e
        jr  c, noinv

Ps      in  f,(c)
        jp  po, ps

        xor 8
noinv   
        ld  (pe1 ),a 
        xor 8      
        ld  (po1 ),a 

        ex de,hl
        ld a,c; 
        jr sigue_e  

incomp2 
        ld  b,  $c 
        jr  sigue_e            
                    ;  7  +8 +12  +7+12  +10+22 = 7 +71

        ld  b,b
escurre 
        ld  b,$40
        ld  b,b
        ld  b,5;x
pausaE  djnz  pausaE ; 13*x +2 
                           ;x= (156 -78  -2 -11)/13 = 5
                           ;x= (143 -78  -2 -11)/13 = 4

e244
        ld  b, $0d
e248
        add a,a
        add a,a
        jr  c, incomp2                    
full
        and  (hl)
        ld  (de),a
        inc de
        ld  a,c     ; 
                    ;  7 +8 +7 +24  +10+22 =  7 + 71
sigue_e              
        ld  hl,k0   ;-$ff00 
        in  f,(c)
k1      
pe1     jp  pe , escurre    ; 22
     
     
b2      inc l
        in  f ,(c)   ;4+12 
po1     jp  po, b2   ;10

        out (c),b    ; 12


        ld  b,11  ;x
pausa1  djnz  pausa1 ; p=13*x +2       
                       ; x= (240 - 97)/13  = 11
                       ; x= (219 - 97)/13  = 9.3

                       ; x= (197 - 97)/13  = 7.6
                       ; x= (179 - 97)/13  = 6.3
      
        ld   b,  $9            
        rlca
        rla          
                      ;  15
        jr  nc, full   
                      ; 10+12 + p + 15 + 12 + 24  + 10 + 12  = 95                                        
        dec  b 
        and  (hl)
        bit  5,L 
        jr  z,  sigue_e  ;31
                         ; 22 + p + 15 + 7 + 31 + 10 + 12  = 97
                         ;                  

pos_EI  ei
        jp randomizeusr


         

randomizeusr nop


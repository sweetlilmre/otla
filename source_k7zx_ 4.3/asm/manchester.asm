
         org 65320 ; org $ffff - 180 -1
  
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

        ld  hl,$3fff
        jp  pilot

pilot  
max     ld  a, $58
        ex  af, af'
        ld  ix , sigue0   
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

        inc  d
        jr  nz , pii   

        cp  34       
        ld  a,d ; ld a, $80

LOWsyn  in  f,(c)
        jp  po, LOWsyn  

        jr  c , entra1
        
        ld (ix+inibit0 - sigue0 ) , $b7
        ld (ix+inibit1 - sigue0 ) , $37
   
       ; ld a,$80  ; jr entra0
       
Highsyn  in  f,(c)
         jp  pe, highsyn        



sigue0
         in  f , (c)
         jp  pe , HIGH

entra0        
LOW      in  f,(c)
         jp  po, LOW  
     
inibit0  scf
         ld  ixl , sigue1 - $ff00
k0       jp  procede

sigue1
         in  f,(c)
         jp  po, LOW     ; 18+50+14+22= 104
entra1
HIGH     in  f , (c)
         jp  pe , HIGH  
     
inibit1  or  a
         ld  ixl , sigue0 -$ff00
k1       jp  procede
         
         

         ld  b,b
         ld  b,b
         ld  b,b
         ld  b,b
         ld  b,b ;ex (sp),hl
         ld  b,b ;ex (sp),hl
         ld  b,b ;ex (sp),hl
         ld  b,b ;ex (sp),hl
procede                 
         rla             ;4+4+10+4 =18
                     
         jr  c, noFULL
FULL 
         dec b
         set 3,b
         ld  (hl),a      
         inc hl          
         xor d            
         ld  d,a
         ld  a,c    ; 7 + 12 + 25 +8 = 52

         jp  (ix)

         ld  b,b
      
nofull   out (c), b 
         ex  af, af'
         cp  h
         jr  z, fin
         ex  af, af'   
         
         jp  (ix)  ; 12  +12 +  19 + 8 = 51

fin  
         ld  a,d       
         or  a
pos_EI 
         ei
         jp randomizeusr ;ret   z
                         ;rst 8
                         ;defb 26

randomizeusr nop

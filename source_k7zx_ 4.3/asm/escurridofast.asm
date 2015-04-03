          

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
        dec hl
        jr pilot2

tabla   
k0       defb $40,$40,$40
         defb $40,$40,$40
         defb $40,$40,$40
         defb $40,$40,$40
         defb $40,$40,$40
         defb $40,$40,$40
         defb $40,$40,$40
         defb $40,$40,$40
         defb $40,$40,$40
         defb $40,$40,$40

pausa  
      ld  b,b
      ld  b,b
      ld  b,b
      ld  b,b
       
       dec (HL)
       inc (HL)
       dec (HL)
       inc (HL)
       out (c),b        
       jr return  ; 56   +12+12= 80

pilot2        
        ld  bc, $0efe
        ld  de, $10ea

pii     xor  a   
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
         xor 8 ;     
        ld  (po1 ),a 

        ex de,hl
        ld hl , tabla
        ld a,c
        ex af,af' 
        ld a,c  
        jr entra

incomp2 
        ld   b,  $c 
        ret  nc
        jr   sigue_e            ;
                    ; 7 + 4+ 8 + 12 + 12 + 12 +  +22 = 77 
escurre        
        ld b,  $b 
        ld   b,b        
        add a,a
        add a,a
        jr   c, incomp2        

        ret  c            
full
        and  (hl)
        inc de
        ld  (de),a
        ld  a,c     ; 24
                    ; 7 + 4 + 8 + 12 + 24 +  22 =  77
sigue_e               

        in  f,(c)
k1     
pe1     jp  pe , escurre 

        ex  af,af'
entra   
        ld  R,a           

b2      in  f ,(c)    
po1     jp  po, b2   
       

        ld   a,R
        or   a
        rra   
        ld   l,a            

        jr  pausa ;   out (c),b 

return  ld  a,c
        inc  b        
        ex   af,af'         
        rlca
        rla          ;  9+ 12 +12 +20= 53
        
        jp  nc, full
                      ; 53 + 10 + 24  + 22  = 109
                      ;                   + 4+9 +22= 144
        ld   b,  $8 
        and  (hl)
        jp   m , sigue_e  

                       ; 53+ 34  + 22  =  109
                        ;                   +4+9+22 = 144

pos_EI  ei
        jp randomizeusr



randomizeusr nop



        org  65320

CTES
        defb MAX-CTES
        defb K0-CTES
        defb K1-CTES
        defb pos_EI -CTES
        defb SYNC -CTES

inicio  ld hl, 23797
        ld de, pilot
        ld b,1
        ldir
max
        ld hl,$3fff
        jp pilot

pilot   di
        ld bc, $eafe
        ld de, $10ee            

pii     xor a
        out (c),e

pi_LOW  inc a
        in f,(c)
        jp po, pi_LOW

        out (c),b

pi_HIGH sub c
        in f,(c)
        jp pe, pi_HIGH

SYNC    cp  75  
        rl d
        cp   54   
        jr nc, pii

        inc d
        jr nz , pii  

        cp  34 
        ld  a,b       

        jr  c, noinv

Ps      in  f,(c)
        jp  po, ps

         xor 8
noinv   
        ld  (pe1 ),a 
        ld  (pe2 ),a  
         xor 8 ;     
        ld  (po0 ),a 
        ld  (po1 ),a 
        ld  (po2 ),a 
      
fs      in  f,(c)
po0     jp  po, fs

        ex   de, hl

        dec  de    
        dec  hl    
        ld   h,$ff
        xor  a     
        ld   ixl,a   ; 35

sigue    
        ld l, (k1-1) - $ff00  ; ld l,b
k0
f1      inc hl
        in f,(c)
pe1     jp pe, f1
     
        add  a,a
        or   (hl)

        ld   (de),a 
        inc  de  
        xor  ixl       ;32                               
     
        ld l, (k1-1) - $ff00    ; ld hl, k1-1  ;ld l,b  
f2      inc hl
        in f,(c)
po1     jp po, f2
        
                    
        ld   ixl,a

        ld   a,(hl) 
        add  a,a    

        out  (c),b       ;31  

      
        ld l, (k1-1) - $ff00  ;ld l,b
f3      inc hl
        in f,(c)
pe2     jp pe, f3      

        add  a,a     
        or   (hl)   
        add  a,a     

        dec  b     
        out  (c),b     ;31  
    
       
        ld l, (k1-1) - $ff00  ;ld  l,b  ;       
f4      inc hl
        in f,(c)
po2     jp po, f4
            
 
        inc  b     

        add  a,a
        or   (hl)  
        add  a,a

        jr nc, sigue  ;  31   

                       
        ld  a,ixl
        or  a

pos_EI
fin     ei 
        jp randusr


k1
        defb 0x20,0x20

        defb 0x20,0x20,0x20,0x20,0x20         
        defb 0x20,0x20,0x20,0x20,0x20
        defb 0x20,0x20,0x20,0x20,0x20
        defb 0x20,0x20,0x20,0x20,0x20

        defb 0x20,0x20

        defb 0x20,0x20,0x20,0x20
       
randusr nop


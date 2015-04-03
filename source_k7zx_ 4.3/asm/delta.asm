       org  65320   
CTES   
        defb MAX-CTES
        defb K0-CTES
        defb K1-CTES
        defb pos_EI -CTES
        defb SYNC -CTES



inicio  ld hl, 23797
        ld de, ini1
        ld b,1
        ldir
max
k1
        ld hl,$3fff
        jp pilot

ini1
       jr pilot

       defb $40,$40,$40
       defb $40,$40,$40
       defb $40,$40


K0
tabla       
       defb $01,$01,$01
       defb $01,$01,$01
       defb $01,$01,$01
       defb $01,$01,$01
       defb $01,$01,$01

       defb $01,$01,$01
       defb $01,$01,$01
       defb $01,$01,$01
       defb $01,$01,$01
       defb $01,$01,$01

       defb $01,$01,$01
       defb $01,$01,$01
       defb $01,$01,$01
       defb $01,$01,$01
       defb $01,$01,$01


       defb $01,$01,$01
       defb $01,$01,$40
       defb $40,$40

pilot   
        ld de, $100a
        ld bc, $0efe
        di

pii     xor a
        out (c),b
pi_LOW  inc a
        in f,(c)
        jp po, pi_LOW

        out (c),e
pi_HIGH sub c
        in f,(c)
        jp pe, pi_HIGH

SYNC    cp  74   
        rl d
        cp 54   
        jr nc, pii

        inc d
        jr nz , pii  

        ex de , hl
        ld hl, $ff50 
        ld a,c
        ex af,af'
        jr entra 

full    and (hl)
        ld (de),a
        inc de
        ld a,c         ; 12 +12 + 24  = 48                         
sigue                    
HIGH    in f,(c)
        jp pe , HIGH

        dec  b
        ex   af,af'      
        ld   a,r  
        set  1,b

        rra
        ld   l ,a 
entra  
        or   $ff 
        ld r,a         ; 49
                   
LOW     in f,(c)
        jp po, LOW  

        ex af, af'
        rlca 
        rla           
     
        jr nc,full 
   
        out (c),b      
        and (hl)  

        jp m,sigue       ; 12 + 7 + 29  = 48       
                       
fin
pos_EI 
        ei
        jp   randusr ; 7997



randusr
   nop



       

    

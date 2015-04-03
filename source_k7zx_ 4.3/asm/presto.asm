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
    
pilot   dec  hl
        ld  a,$ca
        ex af,af' 
        ld de, $100a
        ld bc, $0efe
        di

pii     xor a
        out (c),b
pi_LOW  inc a
        in f,(c)
        jp po, pi_LOW

        out (c),e
pi_HIGH inc  a
        in f,(c)
        jp pe, pi_HIGH

SYNC    cp  72  
        rl d
        cp  55     
        jr nc, pii

        inc d
        jr nz , pii  

        cp  32            
        ld  a,$ea    ;e
        jr c,n_inv

pi_syn  in f,(c)
        jp po, pi_syn

        xor  8
n_inv   ld  (pe1),a 
        xor  8 ;       
        ld  (po1),a  

        ld ixl,d         
        ex de , hl
       ;ld  h,$ff  
        ld a,$78
        jr entra 




full    and (hl)
        ld (de),a
        inc de 
        xor ixl
        ld ixl,a
        ld a,c
                 ; 12 + 40 + 7 = 59
sigue   
entra         
               
        ld hl,k0 - 3  ;ld l,k0-$ff00 - 3

LOW     inc hl
        in f,(c)
po1     jp po, LOW                  
              
        ex  af,af'
        ld  b,a
        ld  a,$c4 
        cp  l
        jr  c,fin
        ld  a,b
        ex  af,af'

        rlca 
        rla           ;  42
                   
HIGH    inc  hl
        in f,(c)
pe1     jp pe , HIGH
     
        jr nc,full 
        
        and  (hl)   
        ex  af,af'
        xor  4
        out  ($fe),a
        ex  af,af'

        jr sigue
                        ;7 + 45 + 7  = 59

      defb 0xff
K0
tabla 

     defb   0xFc , 0xFc  
     defb   0xFC , 0xFC , 0xFC  
     defb   0xFc , 0xFc , 0xFd , 0xFd 
     defb   0xFD , 0xFD , 0xFD , 0xFD
     defb   0xFd , 0xFd , 0xFe , 0xFe 
     defb   0xFe , 0xFE , 0xFE , 0xFE 
     defb   0xFe , 0xFe , 0xFf 
     defb   0xFF , 0xFF , 0xFF , 0xFF , 0xFF  
     defb   0xFf , 0xFf , 0xFf , 0xFf


fin        
        ld a,ixl
        or a                

pos_EI 
     ei
     jp   randusr ; 7997 
 

randusr
   nop



       

    

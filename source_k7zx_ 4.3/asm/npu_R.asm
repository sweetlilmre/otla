
       
      org  $ff00 -15 -1

CTES
        defb MAX-CTES
        defb K0-CTES
        defb K1-CTES
        defb pos_EI -CTES
        defb SYNC -CTES


inicio  ld hl, 23797
        ld de, k0
        ld b,1
        ldir
max
        ld hl,$3fff
        jp k0  ;pilot


k0
k1
 


        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
         
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80

        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
         
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
         
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80
        defb 0x80,0x80,0x80,0x80



pilot   di
        ld bc, $eefe
        ld de, $90ea
pii     xor a
        out (c),e
pi_LOW  inc a
        in f,(c)
        jp po, pi_LOW

        out (c),b
pi_HIGH sub c
        in f,(c)
        jp pe, pi_HIGH

SYNC        cp 74  ;29
        rl d
        cp 54      ;19
        jr nc, pii

        inc d
        jr nz , pii  

        dec hl
        ex  de,hl

        ld  hl, k0+12
      
fs      in  f,(c)
        jp  po, fs

        jr  entra




sigue              ;         ((; 14
       

        add a,a

        ex af,af'
        ld a,R  
        ld l,a 
        ex af,af'        

        add a,(hl)       
                         ; 45

f1      in f,(c)
        jp po, f1
 
 
        add a,a  
        inc de  

        ex af,af'  
        ld a,R
        sub l
        ld l,a
entra
        xor a
        ld R,a 
        ex af,af'     
       
                            ;  46              
f2      in f,(c)
        jp pe, f2
    
         
        add a,a    
        add a,(hl)  
        add a,a     

        ex af,af'
        ld a,R
        ld l,a 
        ex af,af'
   
        add a,a  
        add a,(hl)           
               
       
                             ; 46
f3      in f,(c)
        jp po, f3

        ld  (de),a 
         

        ld a,R
        sub l
        ld l,a

        ld a,0 ;  xor a
        ld R,a   
  
        ld  a,(hl) 
                
                           ; 42
f4      in f,(c)                       
        jp pe, f4
 
        
        add a,a 

        jp nc, sigue       
        
        xor a                

pos_EI
fin     ei 
        jp randusr

randusr nop

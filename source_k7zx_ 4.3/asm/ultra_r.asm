
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
max
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

        ld  de, $c0e8   

        call   entra  


FIN
pos_EI     
         ei
         jp  randomizeusr
  

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

                        ;20
full     add a,e 
         rl  d
         ld  (hl),d
         ld d, e   ; e=1x1x1x0x ; 0xe8
         inc hl          ;29
                      ;    = 49


sigue               
LOW      in  f,(c)
po1      jp  po, LOW


         ld  b,a    
     
         ld  a, r  
         sub b
k1       add a, 212

entra    rl  d 

k0       ld  a, 12
         ld  r,a
                     ; 46

HIGH     in  f , (c)
pe1      jp  pe , HIGH

         ld  a,r
         
         jr  nC , full ; 
                       ; 15
         cp  50
         ret nc
         add a,e 
         rl  d      ; 
         jp  sigue  ;    34 
                       ;    = 49
     
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


randomizeusr nop

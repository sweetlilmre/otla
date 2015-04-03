       
          org  16444          


full    ld   (hl),b
        inc  hl
        ld   b, c;     
more    
cicle
pi_LOW  in    f,(c)
        jp    po, 16447 ;  pi_LOW

        ld    a,c

pi_HIGH dec   a
        in    f,(c)
        jp    pe, 16453  ; pi_HIGH       

        cp    d
        rl    b
xret    jr  nC, full  ; ret   /jr  nC, full ;$404e
                 
                  
        out  ($ff),a               
        cp  e       
        jr  nc, more   ; 
                                
                                  
        pop  hl;                  ;$4055
        pop  de
        pop  bc
        pop  af
                ld   sp,$0000
                out  ($fd),a
                ei
                jp   $0000
pila            defw $ffff
                defw $ffff
                defw $ffff
                defw $ffff 
         
start_load                       
         
        out  ($fd),a

 im  0 ; 1 2
 ld  a,0
 ld  r,a
 ld  a,0
 ld  i,a
 ld  iy,0
 ld  ix,0
 ld  hl,0
 ld  de,0
 ld  bc,0
 exx
 ld  bc,0
 push bc
 pop  af
 ex af,af'
                                       
        ld    bc, $00fe 
        ld    de, $f6e9    
        ld    hl, $4000  

pii     inc   b
        call  16447 ; cicle        
        djnz  pii


        ld    a,$30
        ld    (16462),a ;xret
        ld    sp, pila
        jp    16446  



               



  





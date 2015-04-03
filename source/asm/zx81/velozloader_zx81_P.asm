       
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
                                
                                  
        rst $08;                  ;$4055
        defb $ff

	out  ($fd),a              ;$4057= 16471        
                      
                                      
        ld    bc, $00fe 
        ld    de, $f6e9    
        ld    hl, $fffe   
        ld    ($4007) , hl  
        ld    hl, $4009  

pii     inc   b
        call  16447 ; cicle        
        djnz  pii

        ld    a,$30
        ld    (16462),a ;xret
        jp    16446  



 

Hdir    equ   $ff

        ORG    Hdir*$100  
         


        ld    hl , $800e 
        ld    de , $ff0e
        ld    bc , 250 - 14
        ldir
        jp    start_loading
         

inicio     

       
table
       defb  $40,$40              ;defb  &fc,&fc
       defb  $40,$40              ;defb  &fc,&fc
       defb  $40,$40,$40,$40,$40  ;defb  &fc,&fc,&fc,&fc,&fc
       defb  $41,$41,$41,$41,$41  ;defb  &fd,&fd,&fd,&fd,&fd
       defb  $42,$42,$42,$42,$42  ;defb  &fe,&fe,&fe,&fe,&fe,&fe
       defb  $43,$43,$43,$43,$43  ;defb  &ff,&ff,&ff,&ff,&ff,&ff
       defb  $43,$43,$43,$43,$43  ;defb  &ff,&ff

  
men
     defb  $0a
     defb  "Loading error"
     defb   0   


velozloader
pilot

        ld    d, (hl)
        dec   hl
        ld    e, (hl)
        push  de
        push  hl

        ex    de,hl

        di

        exx
        push  bc
        ld    bc,$7f10        
        out   (c),c         ;border
        ld    c , $44
        exx      

pii_0

        ld   bc, &f510     ; ear

pii     
        exx  
        out  (c), b    ; a  ;
        exx

        xor  a
         
pi_LOW  inc  a
        in   f,(c)
        jp   m, pi_LOW

        exx  
        out  (c),c  
        exx

pi_HIGH inc  a
        in   f,(c)
        jp   p , pi_HIGH


        cp   $39    
        rl   c   ;  d
        cp   $29   
        jr   nc, pii

        inc  c   ;  d
        jr   nz , pii_0
 
syn_LOW in   f,(c)
        jp   m, syn_LOW

        cp   $19       
        ld   a, $f2     ;e ; $fa 

        jr   c, no_inv

        xor  8

syn_hi  in   f,(c)
        jp   p , syn_hi

no_inv
        ld   (p1),a
        xor  8
        ld   (m1),a



        xor  a
        ld   c,a
        jr   enter


full    and  H
        ld   (de),a
        inc  de
        xor  c
        ld   c,a
        ld   a, $fe;      ; 1+2+2+1+1+2 =  9

enter   
more  

HIGH    inc  l
        in   f , (c)
p1      jp   p , HIGH

        
        exx
        inc  c
        res  3,c
        exx

        ld   h, Hdir    ;

        ld   h, (hl)

        ld   l, table - (Hdir  * $100) -2 
                     
                     ; 11


LOW     inc  l
        in   f,(c)
m1      jp   m, LOW  
         
        rlca
        rla       

        jr   nc, full      ;p   nC, full
        
        exx
        out  (c),c
        exx

        and  H
        jp   m, more   ; 1+4+1 +1+3 = 10

        exx
        pop  bc
        exx
  
        pop  hl
        pop  de
        ld   (hl),e
        inc  hl
        ld   (hl),d

        ld   a,  c
        or   a

        di  ;ei
        ret z

        ld   hl, men
wr_men        
        call  $bb5a

        ld   a,(hl)
        or   a
        inc  hl
        
        jr nz, wr_men

        ei 
        ret    


start_loading      

        ld    bc, inicio ; pila

        ld    bc,$f610
        out   (c),c       ;OUT &F600,&10 motor

        call  entra_multiblock


        ld    bc,$f6ef
        out   (c),c       ;OUT &F600,&10 motor off 

        ld    hl,(bu+1)
        ei  
        jp    (hl)    
dexec   
        ret



header  defw   $c020-1  ;$bfff
bu      defb   01  ; ret           ;ret/ld de,/jp/call   
        defw   dexec

entra_multiblock        
         
        ld     hl,  header -1 
        call   velozloader

        ld     hl,(header)
        call   velozloader        
        jr     bu 
              
        

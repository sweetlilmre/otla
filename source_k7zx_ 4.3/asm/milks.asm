

        org 65320;   org 65326 -4 -1

CTES
        defb MAX-CTES
        defb K0-CTES
        defb K0-CTES 
        defb pos_EI -CTES 
        defb SYNC -CTES 


        ld  hl, 23797
        ld  de, inicio  
        ld  b,1
        ldir
MAX
K0      ld  hl,$3fff
        jp  pilot


inicio  
fin_2
        pop HL
        exx 
        ei 
        rst 8        
tabla  
       defb 26 ,$ff,$fc
       defb $ff,$fc,$fc
       defb $ff,$fd,$fd
       defb $ff,$fd,$fd
       defb $ff,$fe,$fe
       defb $ff,$fe,$ff
       defb $ff,$ff,$ff

pilot     
       defb 01 ,$ff,$ff ; ld b

       ld e,$fe  ;contador 4  (d=chk) 
      ;   ld b,e   ; longitud
        ld ix, t00
        exx          
        push hl  

        di
        ld  bc, $0ffe
        ld  de , $1048
        ld  hl,  $ff0a

pii     xor a
        out (c), L

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

               
         ld a,e        ;dato ;a' 
         dec d;     d= $ff

         ex  af, af'
         ld  a,e     
         jr  entra2

t00      ld  a,d
         ld  ixl,t1-$ff00         
         jr  BYTE_OK
                                                         
t1       ld  h,a
         ld  ixl,T2-$ff00 
         jr  BYTE_OK 
                                
t2       ld  l,a
         ld  ixl,t3-$ff00
         jr  BYTE_OK ;
                                
t3       ld  b,a
         ld  ixl,t4-$ff00         
         jr  BYTE_OK ;
                                
t4       ld  c,a
         ld  ixl, t5-$ff00
         jr  BYTE_OK ;
                                
t5       xor d
         ld  ixl, t55-$ff00         
         jr  Z,  chk_ok
         jr  fin_2 

t55      ld  ixl, t6-$ff00
         jr  chk_ok ;


t6       ld  (hl),a       
         inc hl          
         dec bc          
BYTE_OK  xor d           
chk_ok   ld  d,a          
         ld  a,e    
entra    
         ex  af,af'
sigue    exx      
 
         or  d    
         ld  r,a             
HIGH     in  f,(c)
         jp  pe, HIGH 

         ld  a,r     
         rra
entra2
         ld  l,a
         ex  af, af'              
LOW      in  f,(c)        
         jp  po, LOW     

         rlca
         rla
         jr  c , falta 
         ;jr nc , FULL                    


FULL     and (hl)       
         exx         
         jp  (ix)

falta    and (hl)   
         ex  af,af'
           ;rra
         or  e
         and b  
         out ($fe),a
         exx            
         inc b          
         djnz sigue
fin      
acaba    LD  A,D
         pop HL
         exx 
         OR  A
pos_EI 
         ei
         jp  randomizeusr ;ret   z
                         ;rst 8
                         ;defb 26
         
randomizeusr nop

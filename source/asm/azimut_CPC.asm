 

         org  $8000  



azimut


       
        di    

        push  de
        push  bc
        push  af
        exx
        push  bc
        exx 


        ld    bc,$f610
        out   (c),c       ;OUT &F600,&10 motor

        
        call   pilot


        ld    bc,$f6ef
        out   (c),c       ;OUT &F600,&10 motor off 


        exx
        pop  bc
        exx 
        pop  af
        pop  bc
        pop  de
        
        ei  
      
        ret


pilot
        
        ld  de,$8100

        exx
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
        rl   c          ;  d
        cp   $29   
        jr   nc, pii

        inc  c          ;  d
        jr   nz , pii_0


        ld  (de),a
       
more    xor  a

LOW     inc  a
        in   f,(c)
        jp   m, LOW 

        inc  de
        ld  (de),a

        bit  0,d
        bit  0,d
        jp   z,fin        

        xor  a           ; 2+2+2+2+3+1= 12

HIGH    inc  a
        in   f , (c)
        jp   p , HIGH

        inc  de    
        ld  (de),a        
 
        bit  0,d
        bit  0,d
        jp   nz , more   ; 2+2+2+2+3+1= 12


fin

       

        ret    


        

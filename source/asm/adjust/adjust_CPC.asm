
        org   $8000
start           
	    di   		
		exx
		push  bc		
        ld    bc,$f610
        out   (c),c       ;OUT &F600,&10 
        ld    bc,$7f10        
		
        out   (c),c         ;border
        ld    c , $44
        exx      
		
pii_0  ld   bc, &f510     ; ear
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
 
 
        ld (s_width),a
                                
;;;;;;;;;;;;;;;;;;;;;;;;;;                      
        ld  de , buffer                    
        ld  hl , start +$04ff
       
more    ld  a, l  ; $fe

LOW     inc  a
        in   f,(c)
        jp   m, LOW 

        ld  (de),a
        inc  de 

        ld   a,d 
        cp   h               ;4+(start/256)
        jp   z , finmuestreo     

        ld  a, l      ;$fe     ;7+6+4+4+10+4= 35

HIGH    inc  a
        in   f , (c)
        jp   p , HIGH
 
        ld  (de),a 
        inc  de 
      
        ld   a,d 
        cp   h             ;4+(start/256)
        jp   nz , more  
                                
finmuestreo     
                
;;;;;;;;;;;;;;;;;  calcula nivel de continua
                                
        ld  hl , buffer +20
        ld  b, 32
        xor a
bcontinua                       
        add a,(hl)
        inc hl
        sub (hl)
        inc hl                          
        djnz  bcontinua         
		add a,$80                       
        ld (continua),a
                         
;;;;;;;       
        ld  hl, buffer  +20
        call  sumamuestras
        ld  (media1),de  
        call  sumacuadrados
        ld  (d1),ix
                        
        ld  hl, buffer +21
        call  sumamuestras
        ld  (media2),de 
        call  sumacuadrados
        ld  (d2),ix

;;;;;;;;; fin                   
        pop bc
        exx
        ei
        ret
  
;;-------------------------------------------------------                                  
sumamuestras
        ld  ix,0
        ld  de,0
        ld  b,32
bmuestras                       
        ld e,(hl)
        add ix,de
        xor  a
        rld            ; muestra= muestra*16
        inc hl
        inc hl
        djnz bmuestras
		push ix
		pop  de
		rr  d
		rr  e		
         ret nc
         inc de
         ret
   
;;-------------------------------------------------------  
sumacuadrados
        ld  ix,0
		ld  c,e  ; c= ; 16*media   
        ld  b,32
bcuadrados
        dec hl
        dec hl
        ld  a,(hl)
        sub c
        jr nc,positivo
        neg
positivo
        exx
        ld hl,0
        ld d,h
        ld e,a
        or a
        jr  z,nosuma
multiplica
        add hl,de
        dec a
        jr nz, multiplica
nosuma
        push hl
        exx
        pop de    ;(x-m)^2
        add ix,de
        djnz bcuadrados
        ret
                                
;;;;;;;;;;;;;;;;;;;,    
                 
                
s_width    EQU  start + 256
continua   EQU  start + 257
media1     EQU  start + 258
d1         EQU  start + 260
media2     EQU  start + 262
d2         EQU  start + 264
buffer     EQU  start + 266

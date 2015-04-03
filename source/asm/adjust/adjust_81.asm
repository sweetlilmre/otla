
                
s_width    EQU  16444
continua   EQU  16445
media1     EQU  16446
d1         EQU  16448
media2     EQU  16450
d2         EQU  16452

        org    16514
                
        ;di
		out  ($fd),a
		
		ld  c , $fe
                                
;;;;;;;;;;;;;;;;;;;;;;;;;;;
pii_0   ld  d,c        
pii     xor a
        out  ($ff),a 
pi_LOW  inc a
        in f,(c)
        jp po, pi_LOW

        out  ($ff),a 
pi_HIGH inc a
        in f,(c)
        jp pe, pi_HIGH

        cp $39 ;29
        rl d
        cp $29 ;19
        jr nc, pii
                                
        inc d
        jr nz, pii_0
                                
		ld (s_width),a                        
;;;;;;;;;;;;;;;;;;;;;;;;;;                      
        ld  de , buffer       
	    ld  b, 78  ;36 ;70
		
more    ld   a, c
        inc  a
LOW     inc  a
        in   f,(c)
        jp   pe, LOW 

        ld  (de),a
        inc  de   		
		dec  b
        jp   z , finmuestreo     

        ld   a, c          
	inc  a            ;7+6+4+10+4+4= 35
HIGH    inc  a
        in   f , (c)
        jp   po , HIGH
 
        ld  (de),a 
        inc  de   		
		dec  b
        jp   nz , more  
                                
finmuestreo             
        out  ($ff),a     ; hsync    
		
;;;;;;;;;;;;;;;;;  calcual nivel de continua
                                
        ld  hl , buffer + 12 +64
        ld  b, 32
		ld  a,$80 
bcontinua         
        dec hl   
        sub (hl)    
        dec hl              
        add a,(hl)                    
        djnz  bcontinua
                               
        ld (continua),a
                                
; calcula medias               
		;jr skip1 ;
		push  iy
		
    ;    ld  hl, buffer  +12
        call  sumamuestras
        ld  (media1),de                
; calcula medias cuadrado                                       
        ;ld  de,(media1  )
        call  sumacuadrados
        ld  (d1),iy
                                
; calcula medias                
        ld  hl, buffer +13
        call  sumamuestras
        ld  (media2),de 
; calcula medias cuadrado       
        ;ld  de,(media2  )
        call  sumacuadrados
        ld  (d2),iy
		
		pop  iy

skip1	
		out  ($fe),a  
tarda   djnz tarda		
        ;ei	
        ret
		
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;,,
							  
sumamuestras
        ld  iy,0
        ld  de,0
        ld  b,32
bmuestras                       
        ld e,(hl)
        add iy,de
        xor  a
        rld            ; muestra= muestra*16
        inc hl
        inc hl
        djnz bmuestras
		push iy
		pop  de
		rr  d
		rr  e		
         ret nc
         inc de
         ret

;;;;;;;;;;;;;;,
       
sumacuadrados
        ld  iy,0
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
        push  hl;  
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
        ex   de,hl  ; de=(x-m)^2
        pop  hl 
                   
        add iy,de
        djnz bcuadrados
        ret
buffer nop                                
;;;;;;;;;;;;;;;;;;;,
    
                 

           
        org  $c200                 
start         
         di
        exx
         push  hl                                
         ld  bc , $0efe
         ld  e , $0a
                                
;;;;;;;;;;;;;;;;;;;;;;;;;;;
pii_0   ld  d,e        
pii     xor a
        out (c),b
pi_LOW  inc a
        in f,(c)
        jp po, pi_LOW

        out (c),e
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
	ld  hl , start +$04ff
       
more    ld  a, l  ;$fe

LOW     inc  a
        in   f,(c)
        jp   pe, LOW 

        ld  (de),a
        inc  de 

        ld   a,d 
        cp   h               ;4+(start/256)
        jp   z , finmuestreo     

        ld  a, l   ;$fe       ;7+6+4+4+10+4= 35

HIGH    inc  a
        in   f , (c)
        jp   po , HIGH
 
        ld  (de),a 
        inc  de 
      
        ld   a,d 
        cp   h              ;4+(start/256)
        jp   nz , more  
                                
finmuestreo     
                
;;;;;;;;;;;;;;;;;  calcual nivel de continua
                                
        ld  hl , buffer +20
        ld  b, 32
		ld  a,$80 

bcontinua                       
        add a,(hl)
        inc hl
        sub (hl)
        inc hl                          
        djnz  bcontinua
                               
        ld (continua),a
                                
; calcula medias                                
        ld  hl, buffer  +20
        call  sumamuestras
        ld  (media1),de                
; calcula medias cuadrado                                       
        ;ld  de,(media1  )
        call  sumacuadrados
        ld  (d1),ix
                                
; calcula medias                
        ld  hl, buffer +21
        call  sumamuestras
        ld  (media2),de 
; calcula medias cuadrado       
        ;ld  de,(media2  )
        call  sumacuadrados
        ld  (d2),ix

;;;;;;;;; fin                   
        pop hl
        exx
        ei
        ret
                                
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

                                
;;;;;;;;;;;;;;,
       
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

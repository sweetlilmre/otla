
           
         org  $c600  -6
   defw   start
   defw   fin   
   defw   start
        org  $c600                 
start         

        di
                                
                                
;Port 0AAh READ = PPI port C (Keyboard line and Cassette):
;bit  Name   Expl.
;0-3  KB0-3  Keyboard line               (0-8 on SV738 X'Press)
;4    CASON  Cassette motor relay        (0=On, 1=Off)
;5    CASW   Cassette audio out          (Pulse)
;6    CAPS   CAPS-LOCK lamp              (0=On, 1=Off)
;7    SOUND  Keyboard klick bit          (Pulse)   
                                
        ld    a,$0f    ; motor ON cps ON
        out   ($aa),a  ; motor cassette on y caps on

        ld    a,14
        out  ($a0),a

                                
        out ($99),a   ; color  gray
                                
        ld  bc, $87a2 ;ld  bc, $f2a2 

pii_0   ld  de, $0810

pii     ld  a,b  ;xor  a               ;ld a,$87
                                out ($99),a    ;border      
                                
        ;inc hl ;
        ;dec hl ;     ;out ($99),a

pi_LOW  inc a
        in  f,(c)
        jp  m, pi_LOW

                                out ($99),a    ;color
        ;inc hl ;
        ;dec hl ;     ;out ($99),a

pi_HIGH inc a
        in  f,(c)
        jp  p, pi_HIGH

SYNC    cp  $33    + $87  ;  47 +$87   
        rl  e
        cp  $25    + $87  ;  34 +$87   
        jr  nc, pii

        inc e
        jr  nz , pii_0
         
                                sub b
        ld (s_width),a
                                
;;;;;;;;;;;;;;;;;;;;;;;;;;                      
        ld  de , buffer
        ld  hl , start +$04ff        
       
more    ld  a, l

LOW     inc  a
        in   f,(c)
        jp   pe, LOW 

        ld  (de),a
        inc  de 

        ld   a,d 
        cp   h                ;4+(start/256)
        jp   z , finmuestreo     

        ld  a, l  ;$ff          ;7+6+4+7+10+7= 41

HIGH    inc  a
        in   f , (c)
        jp   po , HIGH
 
        ld  (de),a 
        inc  de 
      
        ld   a,d 
        cp   h                ;4+(start/256)
        jp   nz , more  
                                
finmuestreo     
                
;;;;;;;;;;;;;;;;;  calcual nivel de continua
                                
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
                                
        ld    a,$4f    ; motor ON cps OFF
        out   ($aa),a  ; motor cassette on y caps on
     
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
fin     nop
                                
;;;;;;;;;;;;;;;;;;;,
    
                 
                
s_width    EQU  start + 256
continua   EQU  start + 257
media1     EQU  start + 258
d1         EQU  start + 260
media2     EQU  start + 262
d2         EQU  start + 264
buffer     EQU  start + 266
